#include "HierarchyME.h"
#include "HierME_Core.h"

#include "ContrastCalculation.h"
#include "MVF_Contrast_Filter.h"
#include "MExt_Metrics.h"
#include "MExt_MV_Visualization.h"

//#include "../FRCCore/FRCBase.h"

#include <VideoGMLib/VideoImage/VideoImage.h>

#include <math.h>

void Hierarchy_ME_Filter_MVF_TextureUniform ( Hierarchic_ME *MEstruct, int scale );
void Hierarchy_ME_Create_Refine_Masks(Hierarchic_ME * MEstruct, int scale);

gmcMotionEstimator * Hierarchy_ME_GetMEInstance(Hierarchic_ME *MEstruct, frame_id direction, int scale)
{
	if(scale >= MEstruct->hierarchy_parameters.scale_num)
		return NULL;

	return (direction == left) ? &MEstruct->left_frame_ME_instances[scale] : &MEstruct->right_frame_ME_instances[scale];
}

void Hierarchy_ME_Init(Hierarchic_ME* MEstruct, Hier_ME_Params* Params, int w, int h)
{
	int i, j, me_border;
	int scale = Params->scale_num;
		
	int max_mot_hor = Params->major_params.m_iMaxMotHor;
	int max_mot_vert = Params->major_params.m_iMaxMotVert;

	INIT_MEMORY_C(MEstruct->left_frame_ME_instances,			0, scale, gmcMotionEstimator);
	INIT_MEMORY_C(MEstruct->ME_Parameters,			0, scale, cMEParams);

	INIT_MEMORY_C(MEstruct->left_source_frames,		0, scale, ME_Frame);
	INIT_MEMORY_C(MEstruct->right_source_frames,		0, scale, ME_Frame);

	INIT_MEMORY_C(MEstruct->left_frame_contrast,		0, scale, int**);

	INIT_MEMORY_C(MEstruct->right_frame_ME_instances,		0, scale, gmcMotionEstimator);
	INIT_MEMORY_C(MEstruct->right_frame_contrast,	0, scale, int**);

	INIT_MEMORY_C(MEstruct->left_frame_buffers,		0, scale, Buffer_Storage);
	INIT_MEMORY_C(MEstruct->right_frame_buffers,	0, scale, Buffer_Storage);

	//INIT_MEMORY_C(MEstruct->confidence_data,		0, scale, FRCData);

	INIT_MEMORY_C(MEstruct->occlusion_data,			0, scale, DisparityOcclData);

	cInitHierMEParams(&MEstruct->hierarchy_parameters, Params->mvfilter_type, w, h );
	cCopyHierMEParams(&MEstruct->hierarchy_parameters, Params);

	MEstruct->ConvertContrastFunc = Params->ConvertContrastFunc;

	// dimensions of original frames

	for(i = 0; i < scale; i++)
	{
		int cur_w = w>>i, cur_h = h>>i;
		int frame_buffer_plane_size = 0;

		// me instance initialization
		cInitMEParams(&MEstruct->ME_Parameters[i]);
		cCopyMEParams(&MEstruct->ME_Parameters[i], &Params->major_params );
		//cSetMotionAreaME(&MEstruct->ME_Parameters[i], max_mot_hor >> i, max_mot_vert >> i);

		cInitME(&MEstruct->left_frame_ME_instances[i]);
		cStartME(&MEstruct->left_frame_ME_instances[i], Align16(cur_h), Align16(cur_w), &MEstruct->ME_Parameters[i]);
		
		cInitME(&MEstruct->right_frame_ME_instances[i]);
		cStartME(&MEstruct->right_frame_ME_instances[i], Align16(cur_h), Align16(cur_w), &MEstruct->ME_Parameters[i]);
	
		// big frame init
		me_border = cGetBorderSise(&MEstruct->left_frame_ME_instances[i]);

		ME_Frame_Init(&MEstruct->left_source_frames[i],  cur_w, cur_h, me_border, MEstruct->hierarchy_parameters.additional_buffers_number);
		ME_Frame_Init(&MEstruct->right_source_frames[i], cur_w, cur_h, me_border, MEstruct->hierarchy_parameters.additional_buffers_number);
		frame_buffer_plane_size = ME_Frame_Get_Area(&MEstruct->left_source_frames[0], align);

		INIT_MEMORY_C(MEstruct->left_frame_contrast[i], 0, 4, int*);
		INIT_MEMORY_C(MEstruct->right_frame_contrast[i], 0, 4, int*);

		for(j = 0; j < 4; j++)
		{
			INIT_MEMORY_C(MEstruct->left_frame_contrast[i][j],  0, ME_Frame_Get_Area(&MEstruct->left_source_frames[i], align) / GetBlockArea(j), int);
			INIT_MEMORY_C(MEstruct->right_frame_contrast[i][j],	0, ME_Frame_Get_Area(&MEstruct->left_source_frames[i], align) / GetBlockArea(j), int);
		}

		Buffer_Storage_Allocate(&MEstruct->left_frame_buffers[i],  Align16(cur_w), Align16(cur_h) );
		Buffer_Storage_Allocate(&MEstruct->right_frame_buffers[i], Align16(cur_w), Align16(cur_h) );

		// init FRC parameters
		//frcSetParams( &MEstruct->confidence_data[i].m_Options, Align16(cur_w), Align16(cur_h), &MEstruct->ME_Parameters[i]);

		//FRC_Set_External_ME(&MEstruct->confidence_data[i], 
	//		&MEstruct->left_frame_ME_instances[i], 
	//		&MEstruct->right_frame_ME_instances[i]);

	//	FRC_Init( &MEstruct->confidence_data[i] );

		DisparityOcclData_Init(&MEstruct->occlusion_data[i], Align16(cur_w), Align16(cur_h), 
			&MEstruct->left_frame_buffers[i], &MEstruct->right_frame_buffers[i]);
	}

	// average contrast values
	for(i = 0; i < 4; i++)
	{
		MEstruct->left_frame_average_contrast[i] = 0;
	}

	INIT_MEMORY_C(MEstruct->candidates_list, 0, MEstruct->hierarchy_parameters.candidates_num, MVCBlock);

    MEstruct->GetContrastErrorFuncs[B_4x4] = GetErrorSAD_4x4_MMX;
    MEstruct->GetContrastErrorFuncs[B_8x8] = GetErrorSAD_8x8_MMX;
    MEstruct->GetContrastErrorFuncs[B_16x16] = GetErrorSAD_16x16_MMX;
    MEstruct->GetContrastErrorFuncs[B_32x32] = GetErrorSAD_32x32_MMX;
}

void Hierarchy_ME_MainProcess(Hierarchic_ME* MEstruct, const BYTE *cur_frame, const BYTE *ref_frame)
{	
	int i;
	int ali_w, ali_h;
	int w = ME_Frame_Get_Width(&MEstruct->left_source_frames[0], align);
	int h = ME_Frame_Get_Height(&MEstruct->left_source_frames[0], align);

	int scale = MEstruct->hierarchy_parameters.scale_num;

	ME_Frame_Fill_From_Original(&MEstruct->left_source_frames[0] , cur_frame);
	ME_Frame_Fill_From_Original(&MEstruct->right_source_frames[0], ref_frame);

	ME_Frame_Fill_Additional_Buffer( &MEstruct->left_source_frames[0],  GETSTORAGEBUF_LEFT_BYTE(0, BYTE_depth_map),  0 );
	ME_Frame_Fill_Additional_Buffer( &MEstruct->right_source_frames[0], GETSTORAGEBUF_RIGHT_BYTE(0, BYTE_depth_map), 0 );

	// everything works fine with down-scaling and ME instances, the problem is - there is no correct merge
	for(i = 0; i < scale; i++)
	{
		ali_h = ME_Frame_Get_Height(&MEstruct->left_source_frames[i], align);
		ali_w = ME_Frame_Get_Width(&MEstruct->left_source_frames[i], align);

		if(i != 0)
		{
			// downscaling from the previous scale
			ME_Frame_DownScale_by_two(&MEstruct->left_source_frames[i-1],  &MEstruct->left_source_frames[i], orig );
			ME_Frame_DownScale_by_two(&MEstruct->right_source_frames[i-1], &MEstruct->right_source_frames[i], orig );

			ME_Frame_Fill_From_Original(&MEstruct->left_source_frames[i],  ME_Frame_Get_Frame_Buf_Pointer(&MEstruct->left_source_frames[i] , orig) );
			ME_Frame_Fill_From_Original(&MEstruct->right_source_frames[i], ME_Frame_Get_Frame_Buf_Pointer(&MEstruct->right_source_frames[i], orig) );
		}

		Calculate_GradientMask_3Planes ( ME_Frame_Get_Frame_Buf_Pointer(&MEstruct->right_source_frames[i] , align), 
			GETSTORAGEBUF_RIGHT_BYTE (i, BYTE3_compensated_frame_dil), ali_w, ali_h, 4  );

		//ME_Frame_Fill_From_Original( &MEstruct->right_source_frames[i], GETSTORAGEBUF_RIGHT_BYTE (i, BYTE3_compensated_frame_dil) );
		ME_Frame_Fill_Additional_Buffer( &MEstruct->right_source_frames[i], GETSTORAGEBUF_RIGHT_BYTE(i, BYTE3_compensated_frame_dil)				, 0 );
		ME_Frame_Fill_Additional_Buffer( &MEstruct->right_source_frames[i], GETSTORAGEBUF_RIGHT_BYTE(i, BYTE3_compensated_frame_dil)+ali_w*ali_h	, 1 );
		ME_Frame_Fill_Additional_Buffer( &MEstruct->right_source_frames[i], GETSTORAGEBUF_RIGHT_BYTE(i, BYTE3_compensated_frame_dil)+ali_w*ali_h*2	, 2 );
		
		Calculate_GradientMask_3Planes ( ME_Frame_Get_Frame_Buf_Pointer(&MEstruct->left_source_frames[i] , align), 
			GETSTORAGEBUF_LEFT_BYTE (i, BYTE3_compensated_frame_dil), ali_w, ali_h, 4 );

		//ME_Frame_Fill_From_Original( &MEstruct->left_source_frames[i], GETSTORAGEBUF_LEFT_BYTE (i, BYTE3_compensated_frame_dil) );
		ME_Frame_Fill_Additional_Buffer( &MEstruct->left_source_frames[i],  GETSTORAGEBUF_LEFT_BYTE(i, BYTE3_compensated_frame_dil)					,  0 );
		ME_Frame_Fill_Additional_Buffer( &MEstruct->left_source_frames[i],  GETSTORAGEBUF_LEFT_BYTE(i, BYTE3_compensated_frame_dil)+ali_w*ali_h		,  1 );
		ME_Frame_Fill_Additional_Buffer( &MEstruct->left_source_frames[i],  GETSTORAGEBUF_LEFT_BYTE(i, BYTE3_compensated_frame_dil)+ali_w*ali_h*2	,  2 );

		// processing is parallel for both frames simultaneously
		// first parallel section is ME and contrast calculation (because other things depend on them)
		#pragma omp parallel
		{
			#pragma omp sections
			{
				#pragma omp section
				{
					//cSetAdditionalMatchingBuffers( &MEstruct->left_frame_ME_instances[i], 
					//								ME_Frame_Get_AddBufferPointer(&MEstruct->left_source_frames [i]),
					//								ME_Frame_Get_AddBufferPointer(&MEstruct->right_source_frames[i]) );

					cEstimate(&MEstruct->left_frame_ME_instances[i],	ME_Frame_Get_Frame_Buf_Pointer(&MEstruct->left_source_frames[i] , extend), 
						ME_Frame_Get_Frame_Buf_Pointer(&MEstruct->right_source_frames[i] , extend), 0);

					cCalculateAdditionalBuffer(MEstruct, (void*)GETSTORAGEBUF_LEFT_cMV (i, cMV_field),			left, cMV_field,			i, FALSE);
					cCalculateAdditionalBuffer(MEstruct, (void*)GETSTORAGEBUF_LEFT_BYTE(i, BYTE_MVdispersion),	left, BYTE_MVdispersion,	i, FALSE);
					cCalculateAdditionalBuffer(MEstruct, (void*)GETSTORAGEBUF_LEFT_BYTE(i, BYTE_disparity),		left, BYTE_disparity,		i, FALSE);

					Buffer_Storage_InternalCopy(&MEstruct->left_frame_buffers[i], BYTE_disparity, BYTE_disparity_filtered);
				}
				#pragma omp section
				{
					//cSetAdditionalMatchingBuffers( &MEstruct->right_frame_ME_instances[i], 
					//								ME_Frame_Get_AddBufferPointer(&MEstruct->right_source_frames[i]),
					//								ME_Frame_Get_AddBufferPointer(&MEstruct->left_source_frames [i]) );

					cEstimate(&MEstruct->right_frame_ME_instances[i],	ME_Frame_Get_Frame_Buf_Pointer(&MEstruct->right_source_frames[i] , extend),
						ME_Frame_Get_Frame_Buf_Pointer(&MEstruct->left_source_frames[i] , extend),  0);

					cCalculateAdditionalBuffer(MEstruct, (void*)GETSTORAGEBUF_RIGHT_cMV (i, cMV_field),			right, cMV_field,			i, FALSE);
					cCalculateAdditionalBuffer(MEstruct, (void*)GETSTORAGEBUF_RIGHT_BYTE(i, BYTE_MVdispersion),	right, BYTE_MVdispersion,	i, FALSE);
					cCalculateAdditionalBuffer(MEstruct, (void*)GETSTORAGEBUF_RIGHT_BYTE(i, BYTE_disparity),	right, BYTE_disparity,		i, FALSE);

					Buffer_Storage_InternalCopy(&MEstruct->right_frame_buffers[i], BYTE_disparity, BYTE_disparity_filtered);
				}
				#pragma omp section
				{
					// contrast calculation
					Hierarchy_ME_Calc_Frame_Contrast(MEstruct, left, i, 4);
					cCalculateAdditionalBuffer(MEstruct, (void*)GETSTORAGEBUF_LEFT_BYTE(i, BYTE_contrast),		left, BYTE_contrast,		i, FALSE);

					Calculate_FrameColorDispersion( ME_Frame_Get_Frame_Buf_Pointer(&MEstruct->left_source_frames[i] , align), 
						GETSTORAGEBUF_LEFT_BYTE(i, BYTE_frame_dispersion), ali_w, ali_h, 16);

					Calculate_Frames_Difference_BlockBased( ME_Frame_Get_Frame_Buf_Pointer( &MEstruct->left_source_frames[i] , align ),
						ME_Frame_Get_Frame_Buf_Pointer( &MEstruct->right_source_frames[i], align ),
						GETSTORAGEBUF_LEFT_BYTE(i, BYTE_SAD_contrast), 
						ali_w, ali_h, 3, 16);

				}
				#pragma omp section
				{
					// contrast calculation
					Hierarchy_ME_Calc_Frame_Contrast(MEstruct, right, i, 4);
					cCalculateAdditionalBuffer(MEstruct, (void*)GETSTORAGEBUF_RIGHT_BYTE(i, BYTE_contrast),		right, BYTE_contrast,		i, FALSE);

					Calculate_FrameColorDispersion( ME_Frame_Get_Frame_Buf_Pointer(&MEstruct->right_source_frames[i] , align), 
						GETSTORAGEBUF_RIGHT_BYTE(i, BYTE_frame_dispersion), ali_w, ali_h, 16);

					Calculate_Frames_Difference_BlockBased( ME_Frame_Get_Frame_Buf_Pointer( &MEstruct->left_source_frames[i] , align ),
						ME_Frame_Get_Frame_Buf_Pointer( &MEstruct->right_source_frames[i], align ),
						GETSTORAGEBUF_RIGHT_BYTE(i, BYTE_SAD_contrast), 
						ali_w, ali_h, 3, 16);
				}
			}		
		}

		Hierarchy_ME_RecalculateMetrics( MEstruct, i );

		if (MEstruct->hierarchy_parameters.use_uniform_filter)
		{
			if ( MEstruct->hierarchy_parameters.mvfilter_type == MVFilter_Stereo )
			{
				Hierarchy_ME_Filter_MVF(MEstruct, NULL, NULL, cMV_field_filtered, i);
			}
			else if ( MEstruct->hierarchy_parameters.mvfilter_type == MVFilter_2D )
			{
				Hierarchy_ME_Filter_MVF_TextureUniform ( MEstruct, i );
			}
		}
		else
		{
			if ( MEstruct->hierarchy_parameters.mvfilter_type == MVFilter_Stereo )
			{
				Buffer_Storage_InternalCopy( &MEstruct->left_frame_buffers[i],  cMV_field, cMV_field_filtered );
				Buffer_Storage_InternalCopy( &MEstruct->right_frame_buffers[i], cMV_field, cMV_field_filtered );
			}
			else if ( MEstruct->hierarchy_parameters.mvfilter_type == MVFilter_2D )
			{
				Buffer_Storage_InternalCopy( &MEstruct->left_frame_buffers[i],  cMV_field, cMV_uniform_filtered );
				Buffer_Storage_InternalCopy( &MEstruct->right_frame_buffers[i], cMV_field, cMV_uniform_filtered );

				Buffer_Storage_InternalCopy( &MEstruct->left_frame_buffers[i],  cMV_field, cMV_texture_filtered );
				Buffer_Storage_InternalCopy( &MEstruct->right_frame_buffers[i], cMV_field, cMV_texture_filtered );
			}
		}

		if (MEstruct->hierarchy_parameters.use_occlusion_processing)
		{
			if (MEstruct->hierarchy_parameters.mvfilter_type == MVFilter_Stereo )
			{
				Hierarchy_ME_OcclusionRefinement(MEstruct, i, MEstruct->hierarchy_parameters.use_uniform_filter);
			}
			else if ( MEstruct->hierarchy_parameters.mvfilter_type == MVFilter_2D )
			{
				EstimateOcclusions2D ( &MEstruct->left_frame_buffers [i] , 
										ME_Frame_Get_Frame_Buf_Pointer( &MEstruct->left_source_frames[i],  align ),
										ME_Frame_Get_Frame_Buf_Pointer( &MEstruct->right_source_frames[i],  align ) );

				EstimateOcclusions2D ( &MEstruct->right_frame_buffers[i] , 
										ME_Frame_Get_Frame_Buf_Pointer( &MEstruct->right_source_frames[i], align ),
										ME_Frame_Get_Frame_Buf_Pointer( &MEstruct->left_source_frames[i],  align ) );
			}
		}

	// end of scale iteration
	}

}

// should be called after process call
void Hierarchy_ME_RecalculateMetrics( Hierarchic_ME* MEstruct, int scale )
{
	int ali_h = ME_Frame_Get_Height(&MEstruct->left_source_frames[scale], align);
	int ali_w = ME_Frame_Get_Width(&MEstruct->left_source_frames[scale], align);
	int i = scale;

	// confidence and LRC are not frame-independent, so they are not in omp sections (but they have internal parallelism)
	/*FRC_Process_Frame(&MEstruct->confidence_data[i], 
		Get_Frame_Buf_Pointer(&MEstruct->right_source_frames[i]  , align),
		Get_Frame_Buf_Pointer(&MEstruct->left_source_frames[i]   , align) );*/

	BufferStorage_LeftRightConsistency ( &MEstruct->left_frame_buffers[scale], &MEstruct->right_frame_buffers[scale], cMV_field, BYTE_lrc_mask );

	//////////////////////////////////////////////////////////////////////////
	// second section is calculation of all other stuff
	#pragma omp parallel
	{
		#pragma omp sections
		{
			#pragma omp section
			{
				cCalculateAdditionalBuffer(MEstruct, (void*)GETSTORAGEBUF_LEFT_BYTE(i, BYTE_confidence),	left,  BYTE_confidence,	i, FALSE);
				//Merge_Filtering_Mask(MEstruct, left, i);
			}
			#pragma omp section
			{
				cCalculateAdditionalBuffer(MEstruct, (void*)GETSTORAGEBUF_RIGHT_BYTE(i, BYTE_confidence),	right, BYTE_confidence,	i, FALSE);
				//Merge_Filtering_Mask(MEstruct, right, i);
			}
			#pragma omp section
			{
				Create_Refine_Mask( GETSTORAGEBUF_LEFT_BYTE(i, BYTE_contrast),
					GETSTORAGEBUF_LEFT_BYTE(i, BYTE_SAD_contrast),
					GETSTORAGEBUF_LEFT_BYTE(i, BYTE_frame_dispersion),
					GETSTORAGEBUF_LEFT_BYTE(i, BYTE_lrc_mask),
					GETSTORAGEBUF_LEFT_BYTE(i, BYTE_filter_mask_prepared),
					ali_w, ali_h, MEstruct->hierarchy_parameters.uniform_filter_strength);

				Create_Refine_NonTexture_Mask( GETSTORAGEBUF_LEFT_BYTE(i, BYTE_contrast),
					GETSTORAGEBUF_LEFT_BYTE(i, BYTE_frame_dispersion),
					GETSTORAGEBUF_LEFT_BYTE(i, BYTE_lrc_mask),
					GETSTORAGEBUF_LEFT_BYTE(i, BYTE_filter_mask_uniform),
					ali_w, ali_h );

				Create_Refine_Texture_Mask( GETSTORAGEBUF_LEFT_BYTE(i, BYTE_contrast),
					GETSTORAGEBUF_LEFT_BYTE(i, BYTE_frame_dispersion),
					GETSTORAGEBUF_LEFT_BYTE(i, BYTE_lrc_mask),
					GETSTORAGEBUF_LEFT_BYTE(i, BYTE_filter_mask_texture),
					ali_w, ali_h );
			}
			#pragma omp section
			{
				Create_Refine_Mask( GETSTORAGEBUF_RIGHT_BYTE(i, BYTE_contrast),
					GETSTORAGEBUF_RIGHT_BYTE(i, BYTE_SAD_contrast),
					GETSTORAGEBUF_RIGHT_BYTE(i, BYTE_frame_dispersion),
					GETSTORAGEBUF_RIGHT_BYTE(i, BYTE_lrc_mask),
					GETSTORAGEBUF_RIGHT_BYTE(i, BYTE_filter_mask_prepared),
					ali_w, ali_h, MEstruct->hierarchy_parameters.uniform_filter_strength);

				Create_Refine_NonTexture_Mask( GETSTORAGEBUF_RIGHT_BYTE(i, BYTE_contrast),
					GETSTORAGEBUF_RIGHT_BYTE(i, BYTE_frame_dispersion),
					GETSTORAGEBUF_RIGHT_BYTE(i, BYTE_lrc_mask),
					GETSTORAGEBUF_RIGHT_BYTE(i, BYTE_filter_mask_uniform),
					ali_w, ali_h);

				Create_Refine_Texture_Mask( GETSTORAGEBUF_RIGHT_BYTE(i, BYTE_contrast),
					GETSTORAGEBUF_RIGHT_BYTE(i, BYTE_frame_dispersion),
					GETSTORAGEBUF_RIGHT_BYTE(i, BYTE_lrc_mask),
					GETSTORAGEBUF_RIGHT_BYTE(i, BYTE_filter_mask_texture),
					ali_w, ali_h);
			}
		}
	}
}
void Hierarchy_ME_DeInit(Hierarchic_ME* MEstruct)
{
	int i, j;
	int scale = MEstruct->hierarchy_parameters.scale_num;
	for(i = 0; i < scale; i++)
	{
		cStopME(&MEstruct->right_frame_ME_instances[i]);
		cStopME(&MEstruct->left_frame_ME_instances[i]);
		cDeInitMEParams(&MEstruct->ME_Parameters[i]);
		
		ME_Frame_DeInit(&MEstruct->left_source_frames[i]);
		ME_Frame_DeInit(&MEstruct->right_source_frames[i]);

		for(j = 0; j < 4; j++)
		{
			DEINIT_MEMORY_C(MEstruct->left_frame_contrast[i][j]);
			DEINIT_MEMORY_C(MEstruct->right_frame_contrast[i][j]);
		}

		DEINIT_MEMORY_C(MEstruct->left_frame_contrast[i]);
		DEINIT_MEMORY_C(MEstruct->right_frame_contrast[i]);

		Buffer_Storage_Deallocate(&MEstruct->left_frame_buffers[i]  );
		Buffer_Storage_Deallocate(&MEstruct->right_frame_buffers[i] );

		//FRC_DeInit( &MEstruct->confidence_data[i] );
		DisparityOcclData_Deinit(&MEstruct->occlusion_data[i]);
	}

	DEINIT_MEMORY_C(MEstruct->left_frame_contrast);
	DEINIT_MEMORY_C(MEstruct->left_frame_ME_instances);
	DEINIT_MEMORY_C(MEstruct->ME_Parameters);
	DEINIT_MEMORY_C(MEstruct->left_source_frames);
	DEINIT_MEMORY_C(MEstruct->right_source_frames);

	DEINIT_MEMORY_C(MEstruct->right_frame_contrast);
	DEINIT_MEMORY_C(MEstruct->right_frame_ME_instances);

	DEINIT_MEMORY_C(MEstruct->candidates_list);
	DEINIT_MEMORY_C(MEstruct->left_frame_buffers);
	DEINIT_MEMORY_C(MEstruct->right_frame_buffers);

	//DEINIT_MEMORY_C(MEstruct->confidence_data);
	DEINIT_MEMORY_C(MEstruct->occlusion_data);
}

// dimensions are align16(). It is important.

void Hierarchy_ME_Filter_MVF(Hierarchic_ME *MEstruct, void *buffer_left, void *buffer_right, frame_type type_of_output, int scale)
{
	int height = ME_Frame_Get_Height(&MEstruct->left_source_frames[scale], align);
	int width  = ME_Frame_Get_Width(&MEstruct->left_source_frames[scale], align);

	gmcMotionEstimator *ME_left = &MEstruct->left_frame_ME_instances[scale];
	gmcMotionEstimator *ME_right = &MEstruct->right_frame_ME_instances[scale];

	BYTE *depth_left  = GETSTORAGEBUF_LEFT_BYTE(scale, BYTE_disparity);
	BYTE *depth_right = GETSTORAGEBUF_RIGHT_BYTE(scale, BYTE_disparity);

	cMV *MVField_left  = GETSTORAGEBUF_LEFT_cMV(scale, cMV_field);
	cMV *MVField_right = GETSTORAGEBUF_RIGHT_cMV(scale, cMV_field);

	BYTE *mask_left  = GETSTORAGEBUF_LEFT_BYTE(scale, BYTE_filter_mask_prepared);
	BYTE *mask_right = GETSTORAGEBUF_RIGHT_BYTE(scale, BYTE_filter_mask_prepared);

	BYTE *src_left = ME_Frame_Get_Frame_Buf_Pointer( &MEstruct->left_source_frames[scale], align );
	BYTE *src_right = ME_Frame_Get_Frame_Buf_Pointer( &MEstruct->right_source_frames[scale], align );

	BYTE *dst_left  = (buffer_left)  ? buffer_left  : GETSTORAGEBUF_LEFT_BYTE(scale, BYTE_disparity_filtered);
	BYTE *dst_right = (buffer_right) ? buffer_right : GETSTORAGEBUF_RIGHT_BYTE(scale, BYTE_disparity_filtered);

	cMV *dst_left_mv  = (buffer_left)  ? buffer_left  : GETSTORAGEBUF_LEFT_cMV(scale, cMV_field_filtered);
	cMV *dst_right_mv = (buffer_right) ? buffer_right : GETSTORAGEBUF_RIGHT_cMV(scale, cMV_field_filtered);

	int cands_num = MEstruct->hierarchy_parameters.candidates_num;

	switch (type_of_output)
	{
	case BYTE_disparity_filtered:

		#pragma omp parallel
		{
			#pragma omp sections
			{
				#pragma omp section
				{
					Prefill_Refinement_Mask( depth_left, mask_left, depth_left, width, height, 8);
					MV_Depth_Filter(ME_left, mask_left, src_left, depth_left, dst_left, cands_num, 8);
				}
				#pragma omp section
				{
					Prefill_Refinement_Mask( depth_right, mask_right, depth_right, width, height, 8 );
					MV_Depth_Filter(ME_right, mask_right, src_right, depth_right, dst_right, cands_num, 8);
				}
			}
		}

		break;
	case cMV_field_filtered:

		#pragma omp parallel
		{
			#pragma omp sections
			{
				#pragma omp section
				{
					Prefill_Refinement_Mask_cMV( MVField_left, mask_left, MVField_left, width, height, 8 );

					MV_Filter( mask_left, src_left, NULL, MVField_left, dst_left_mv, cands_num, 8, width, height );

					Calculate_Disparity_From_MotionField( dst_left_mv, GETSTORAGEBUF_LEFT_BYTE(scale, BYTE_disparity_filtered),
													width, height, 0.25/*MEstruct->hierarchy_parameters.MV_mult_koef / 4*/ );
				}
				#pragma omp section
				{
					Prefill_Refinement_Mask_cMV( MVField_right, mask_right, MVField_right, width, height, 8 );

					MV_Filter(mask_right, src_right, NULL, MVField_right, dst_right_mv, cands_num, 8, width, height );

					Calculate_Disparity_From_MotionField( dst_right_mv, GETSTORAGEBUF_RIGHT_BYTE(scale, BYTE_disparity_filtered),
													width, height, 0.25/*MEstruct->hierarchy_parameters.MV_mult_koef / 4*/);
				}
			}
		}
		break;

	default:
		return;
	}
}

void Hierarchy_ME_Filter_MVF_TextureUniform ( Hierarchic_ME *MEstruct, int scale )
{
	int height = ME_Frame_Get_Height(&MEstruct->left_source_frames[scale], align);
	int width  = ME_Frame_Get_Width(&MEstruct->left_source_frames[scale], align);

	gmcMotionEstimator *ME_left = &MEstruct->left_frame_ME_instances[scale];
	gmcMotionEstimator *ME_right = &MEstruct->right_frame_ME_instances[scale];

	cMV *MVField_left  = GETSTORAGEBUF_LEFT_cMV(scale, cMV_field);
	cMV *MVField_right = GETSTORAGEBUF_RIGHT_cMV(scale, cMV_field);

	BYTE *depth_left  = GETSTORAGEBUF_LEFT_BYTE( scale, BYTE_depth_map );
	BYTE *depth_right = GETSTORAGEBUF_RIGHT_BYTE( scale, BYTE_depth_map );

	BYTE *mask_left_texture  = GETSTORAGEBUF_LEFT_BYTE(scale, BYTE_filter_mask_texture);
	BYTE *mask_right_texture = GETSTORAGEBUF_RIGHT_BYTE(scale, BYTE_filter_mask_texture);

	BYTE *mask_left_uniform  = GETSTORAGEBUF_LEFT_BYTE(scale, BYTE_filter_mask_uniform);
	BYTE *mask_right_uniform = GETSTORAGEBUF_RIGHT_BYTE(scale, BYTE_filter_mask_uniform);

	BYTE *src_left = ME_Frame_Get_Frame_Buf_Pointer( &MEstruct->left_source_frames[scale], align );
	BYTE *src_right = ME_Frame_Get_Frame_Buf_Pointer( &MEstruct->right_source_frames[scale], align );

	int cands_num = MEstruct->hierarchy_parameters.candidates_num;

	#pragma omp parallel
	{
		#pragma omp sections
		{
			#pragma omp section
			{
				Prefill_Refinement_Mask_cMV( MVField_left, mask_left_texture, GETSTORAGEBUF_LEFT_cMV(scale, cMV_texture_filtered), width, height, 4 );
				Prefill_Refinement_Mask_cMV( MVField_left, mask_left_uniform, GETSTORAGEBUF_LEFT_cMV(scale, cMV_uniform_filtered), width, height, 16);

				MV_Filter	(			mask_left_texture, src_left, depth_left,	GETSTORAGEBUF_LEFT_cMV(scale, cMV_texture_filtered), GETSTORAGEBUF_LEFT_cMV(scale, cMV_texture_filtered), 24, 4, width, height);
				//MV_Filter				( mask_left_uniform, src_left,				GETSTORAGEBUF_LEFT_cMV(scale, cMV_texture_filtered/*cMV_uniform_filtered*/), GETSTORAGEBUF_LEFT_cMV(scale, cMV_uniform_filtered), 13, 8);
				MV_Filter	( 			mask_left_uniform, src_left, depth_left,	GETSTORAGEBUF_LEFT_cMV(scale, cMV_texture_filtered/*cMV_uniform_filtered*/), GETSTORAGEBUF_LEFT_cMV(scale, cMV_uniform_filtered), 13, 8, width, height);
			}
			#pragma omp section
			{
				Prefill_Refinement_Mask_cMV( MVField_right, mask_right_texture, GETSTORAGEBUF_RIGHT_cMV(scale, cMV_texture_filtered), width, height, 4 );
				Prefill_Refinement_Mask_cMV( MVField_right, mask_right_uniform, GETSTORAGEBUF_RIGHT_cMV(scale, cMV_uniform_filtered), width, height, 16 );

				MV_Filter	(			mask_right_texture, src_right, depth_right, GETSTORAGEBUF_RIGHT_cMV(scale, cMV_texture_filtered), GETSTORAGEBUF_RIGHT_cMV(scale, cMV_texture_filtered), 24, 4, width, height);
				//MV_Filter				( ME_right,	mask_right_uniform, src_right,				GETSTORAGEBUF_RIGHT_cMV(scale, cMV_texture_filtered/*cMV_uniform_filtered*/), GETSTORAGEBUF_RIGHT_cMV(scale, cMV_uniform_filtered), 13, 8);
				MV_Filter	( 			mask_right_uniform, src_right, depth_right,	GETSTORAGEBUF_RIGHT_cMV(scale, cMV_texture_filtered/*cMV_uniform_filtered*/), GETSTORAGEBUF_RIGHT_cMV(scale, cMV_uniform_filtered), 13, 8, width, height);
			}
		}
	}

	LeftRightConsistency(
		GETSTORAGEBUF_LEFT_cMV(scale, cMV_uniform_filtered),	GETSTORAGEBUF_RIGHT_cMV (scale, cMV_uniform_filtered), 
		NULL, NULL,
		GETSTORAGEBUF_LEFT_BYTE(scale, BYTE_lrc_uniform_filter),		GETSTORAGEBUF_RIGHT_BYTE(scale, BYTE_lrc_uniform_filter),
		NULL, NULL,
		width, height);

	LeftRightConsistency(
		GETSTORAGEBUF_LEFT_cMV(scale, cMV_texture_filtered),	GETSTORAGEBUF_RIGHT_cMV (scale, cMV_texture_filtered), 
		NULL, NULL,
		GETSTORAGEBUF_LEFT_BYTE(scale, BYTE_lrc_texture_filter),		GETSTORAGEBUF_RIGHT_BYTE(scale, BYTE_lrc_texture_filter),
		NULL, NULL,
		width, height);

	//Create_Invert_LRC_Mask (GETSTORAGEBUF_LEFT_BYTE(scale, BYTE_lrc_uniform_filter),  GETSTORAGEBUF_LEFT_BYTE(scale, BYTE_filter_mask_uniform),  width, height );
	//Create_Invert_LRC_Mask (GETSTORAGEBUF_RIGHT_BYTE(scale, BYTE_lrc_uniform_filter), GETSTORAGEBUF_RIGHT_BYTE(scale, BYTE_filter_mask_uniform), width, height );

	//MV_Iterative_Filter_DepthBased	(	GETSTORAGEBUF_LEFT_BYTE(scale, BYTE_filter_mask_uniform), src_left, depth_left,	
	//									GETSTORAGEBUF_LEFT_cMV(scale, cMV_uniform_filtered), GETSTORAGEBUF_LEFT_cMV(scale, cMV_uniform_filtered), 21, 4, width, height);

	//Build_ShiftVectorField (  GETSTORAGEBUF_LEFT_cMV(scale, cMV_uniform_filtered),  GETSTORAGEBUF_LEFT_cMV(scale, cMV_texture_filtered), width, height );
	//Build_ShiftVectorField (  GETSTORAGEBUF_RIGHT_cMV(scale, cMV_uniform_filtered), GETSTORAGEBUF_RIGHT_cMV(scale, cMV_texture_filtered), width, height );
}

void Hierarchy_ME_SetExternalDepth( Hierarchic_ME * MEstruct, BYTE * left_depth, BYTE * right_depth)
{
	int i;

	if ( left_depth )
	{
		memcpy( GETSTORAGEBUF_LEFT_BYTE(0, BYTE_depth_map ), left_depth, ME_Frame_Get_Area( &MEstruct->left_source_frames[0], align ) );
	}
	else
	{
		memset( GETSTORAGEBUF_LEFT_BYTE(0, BYTE_depth_map ), 0, ME_Frame_Get_Area( &MEstruct->left_source_frames[0], align ) );
	}
	if ( right_depth )
	{
		memcpy( GETSTORAGEBUF_RIGHT_BYTE(0, BYTE_depth_map ), right_depth, ME_Frame_Get_Area( &MEstruct->left_source_frames[0], align ) );
	}
	else
	{
		memset( GETSTORAGEBUF_RIGHT_BYTE(0, BYTE_depth_map ), 0, ME_Frame_Get_Area( &MEstruct->left_source_frames[0], align ) );
	}

	for(i = 1; i < MEstruct->hierarchy_parameters.scale_num; ++i)
	{
		int ali_h = ME_Frame_Get_Height(&MEstruct->left_source_frames[i], align);
		int ali_w = ME_Frame_Get_Width(&MEstruct->left_source_frames[i], align);

		// downscaling from the previous scale
		DownScale_frame_by_two(GETSTORAGEBUF_LEFT_BYTE (i-1, BYTE_depth_map ),  GETSTORAGEBUF_LEFT_BYTE (i, BYTE_depth_map ), ali_w, ali_h );
		DownScale_frame_by_two(GETSTORAGEBUF_RIGHT_BYTE(i-1, BYTE_depth_map ),  GETSTORAGEBUF_RIGHT_BYTE(i, BYTE_depth_map ), ali_w, ali_h );
	}
}


// logic is simple. If dst != NULL - copy there and returns NULL. 
// If dst == NULL - returns pointer.
// we suppose, that dst buffer has original dimensions
// type of dst is void, because there can be cMV or BYTE buffer, for example
// 
void * Hierarchy_ME_GetPrecalcBuffer(Hierarchic_ME *MEstruct, void *dst, int type_of_output, 
								  frame_id direction, int scale)
{
	BOOL copy_buffer =   dst != NULL;

	Size ali_dim  = ME_Frame_Get_Dims(&MEstruct->left_source_frames[scale], align);
	ME_Frame* frame_info = &MEstruct->left_source_frames[scale];

	// this is frame size of aligned frame, it is important.
	void *internal_buffer = Hierarchy_ME_GetPrecalculatedBufPointer(MEstruct, type_of_output, direction, scale);
	
	if(copy_buffer)
	{
		if(type_of_output < BYTE_buffer_type_capacity)
		{
			cDelBordersY( (BYTE*) dst, (BYTE*)internal_buffer, ali_dim.m_iW, ali_dim.m_iH, 0, 0,
				frame_info->align_w_diff, frame_info->align_h_diff);
			return NULL;
		}
		else if ( type_of_output >= BYTE3_buffer_type_shift  &&  type_of_output < BYTE3_buffer_type_shift + BYTE3_buffer_type_capacity )
		{
			cDelBordersYUV( (BYTE*) dst, (BYTE*) internal_buffer, ali_dim.m_iW, ali_dim.m_iH, 0, 0,
				frame_info->align_w_diff, frame_info->align_h_diff);
		}

		else if ( type_of_output >= cMV_buffer_type_shift  &&  type_of_output < cMV_buffer_type_shift + cMV_buffer_type_capacity )
		{
			cDelBorders_cMV((cMV*) dst, (cMV*) internal_buffer, ali_dim.m_iW, ali_dim.m_iH, 0, 0,
				frame_info->align_w_diff, frame_info->align_h_diff);
		}

		return NULL;
	}

	return internal_buffer;
}

void Hierarchy_ME_Confidence_Based_Filter(Hierarchic_ME * MEstruct, BYTE *dst_disparity, frame_id direction, int scale, int filter_radius)
{
	int ali_h = ME_Frame_Get_Height(&MEstruct->left_source_frames[scale], align);
	int ali_w = ME_Frame_Get_Width(&MEstruct->left_source_frames[scale],  align);

	if(direction == left)
	{
		ConfidenceBased_Disparity_Filter( &MEstruct->left_frame_ME_instances[scale], GETSTORAGEBUF_LEFT_BYTE(scale, BYTE_disparity), 
								GETSTORAGEBUF_LEFT_BYTE(scale, BYTE_confidence), dst_disparity, ali_w, ali_h, filter_radius );
	}
	else if(direction == right)
	{
		ConfidenceBased_Disparity_Filter(&MEstruct->right_frame_ME_instances[scale], GETSTORAGEBUF_RIGHT_BYTE(scale, BYTE_disparity), 
								GETSTORAGEBUF_RIGHT_BYTE(scale, BYTE_confidence), dst_disparity, ali_w, ali_h, filter_radius );
	}
}

/**
**************************************************************************
* \brief Occlusion refinement in motion fields
*
* This function calculates occlusion masks from original or filtered motion field,
* depending on passed parameters. It also refines occlusion areas in filtered disparity maps.
* Function changes the following buffers in MEstruct data:
* BYTE_disparity_filtered and buffers, which are connected to OcclusionData.
* 
* \param MEstruct			[IN/OUT]	-  Hierarchic Motion Estimation instance
* \param scale				[IN]		-  required scale of calculation
* \param uniform_filter		[IN]		-  Flag indicates, that function will use filtered motion field as basics
* 
* \return None
*/

void Hierarchy_ME_OcclusionRefinement(Hierarchic_ME * MEstruct, int scale, BOOL uniform_filter)
{
	int width = MEstruct->occlusion_data[scale].width ;
	int height = MEstruct->occlusion_data[scale].height;

	Process_Occlusions_Upd(&MEstruct->occlusion_data[scale],	GETSTORAGEBUF_LEFT_BYTE(scale, BYTE_disparity_filtered), 
														GETSTORAGEBUF_RIGHT_BYTE(scale, BYTE_disparity_filtered), NULL, NULL, cMV_field );
	/*if(uniform_filter)
	{
		Correct_MVOcclusions_From_Disparity(GETSTORAGEBUF_LEFT_cMV(scale, cMV_field_filtered), 
											GETSTORAGEBUF_LEFT_BYTE(scale, BYTE_occlusion_mask), 
											GETSTORAGEBUF_LEFT_BYTE(scale, BYTE_disparity_filtered),
											width, height);
		
		Correct_MVOcclusions_From_Disparity(GETSTORAGEBUF_RIGHT_cMV(scale, cMV_field_filtered),
											GETSTORAGEBUF_RIGHT_BYTE(scale, BYTE_occlusion_mask), 
											GETSTORAGEBUF_RIGHT_BYTE(scale, BYTE_disparity_filtered),
											width, height);
	}
	else
	{
		Correct_MVOcclusions_From_Disparity(GETSTORAGEBUF_LEFT_cMV(scale, cMV_field), 
											GETSTORAGEBUF_LEFT_BYTE(scale, BYTE_occlusion_mask), 
											GETSTORAGEBUF_LEFT_BYTE(scale, BYTE_disparity_filtered),
											width, height);
		
		Correct_MVOcclusions_From_Disparity(GETSTORAGEBUF_RIGHT_cMV(scale, cMV_field),
											GETSTORAGEBUF_RIGHT_BYTE(scale, BYTE_occlusion_mask), 
											GETSTORAGEBUF_RIGHT_BYTE(scale, BYTE_disparity_filtered),
											width, height);
	}*/

}

void Hierarchy_ME_GetDisparity(Hierarchic_ME * MEstruct, short * dst, int scale, frame_id direction)
{
	int frame_size	= ME_Frame_Get_Area(&MEstruct->left_source_frames[scale], orig);
	int width		= ME_Frame_Get_Width(&MEstruct->left_source_frames[scale], orig);
	int height		= ME_Frame_Get_Height(&MEstruct->left_source_frames[scale], orig);

	int ali_width	= ME_Frame_Get_Width(&MEstruct->left_source_frames[scale], align);
	int x, y;

	cMV * motion;

	if(MEstruct->hierarchy_parameters.use_uniform_filter)
	{
		motion = Hierarchy_ME_GetPrecalculatedBufPointer(MEstruct, cMV_field_filtered, direction, scale);
	}
	else
	{
		motion = Hierarchy_ME_GetPrecalculatedBufPointer(MEstruct, cMV_field, direction, scale);
	}

	for(y = 0; y < height; ++ y)
	{
		for (x = 0; x < width; ++ x)
		{
			int index_mv = y * ali_width + x;
			int index = y * width + x;
			
			dst[index] = motion[index_mv].x;
		}
	}
}

void Hierarchy_ME_Draw_MV_Visualization( Hierarchic_ME * MEstruct, BYTE * rgb_output, cMV_buffer_type field, frame_id direction, int scale)
{
	cMV* motion_field = (direction == left) ? GETSTORAGEBUF_LEFT_cMV(scale, field) : GETSTORAGEBUF_RIGHT_cMV(scale, field);

	int width		= ME_Frame_Get_Width(&MEstruct->left_source_frames[scale], align);
	int height		= ME_Frame_Get_Height(&MEstruct->left_source_frames[scale], align);

	DrawMotionField ( motion_field, rgb_output, width, height, 0 );
}

void Hierarchy_ME_get_vector(Hierarchic_ME * MEstruct,int x,int y,int *vx,int *vy)
{
	int MAX_RADIUS = 10;//MEstruct->hierarchy_parameters.uniform_filter_strength * 5;

	cMV* motion_field = GETSTORAGEBUF_LEFT_cMV(0, cMV_uniform_filtered) ;

	int width		= ME_Frame_Get_Width(&MEstruct->left_source_frames[0], align);
	int height		= ME_Frame_Get_Height(&MEstruct->left_source_frames[0], align);
	int ind_x=x, ind_y=y;
	int plane_size = width * height;

	int index = ind_y * width + ind_x;
	//int coord;

	*vx = motion_field[index].x;
	*vy = motion_field[index].y;
}
int Hierarchy_ME_Params_size()
{
	return sizeof( Hier_ME_Params);
}
int Hierarchy_ME_Instance_size()
{
	return sizeof( Hierarchic_ME);
}
