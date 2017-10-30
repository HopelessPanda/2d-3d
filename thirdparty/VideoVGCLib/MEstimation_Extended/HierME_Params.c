#include "HierME_Params.h"
#include "ContrastCalculation.h"


void cCopyHierMEParams(Hier_ME_Params* dst, Hier_ME_Params* src)
{
	cMEParams temp_params;
	if (src == NULL) return;

	// copy ME params correctly
	cCopyMEParams(&dst->major_params, &src->major_params);

	// save ME params from rewriting
	temp_params = dst->major_params;
	// copy other params
	*dst = *src;
	//restore ME params
	dst->major_params = temp_params;
}

void cInitHierMEParams ( Hier_ME_Params* Params, MVFilter_type type, int frame_width, int frame_height )
{
	assert (type == MVFilter_Stereo || type == MVFilter_2D );
	Params->mvfilter_type = type;

	cInitMEParams(&Params->major_params);

	cSetMEAlgME			(&Params->major_params, MSU);
	cSetChromaSettingsME(&Params->major_params, TRUE, crNoReduction, crNoReduction);
	
	Params->additional_buffers_number = 0;
	//cSetAdditionalBuffersNumberME( &Params->major_params, Params->additional_buffers_number );
	
	if( Params->mvfilter_type == MVFilter_2D )
	{
        Params->major_params.m_MEMetricAlgorithm = METRIC_SAD;

		// motion area is square, not a rectangle
		cSetMotionAreaME	(&Params->major_params, frame_width / 12, frame_width / 12 ); 
	}
	else if ( Params->mvfilter_type == MVFilter_Stereo )
	{
        Params->major_params.m_MEMetricAlgorithm = METRIC_COLORINDEPENDENT;

		cSetMotionAreaME	(&Params->major_params, (int)CLIP(48 * (frame_width + 475.0) / 476.0, 64, 160), frame_height / 32 );
	}

	cSetPrecisionME		(&Params->major_params, QuarterPixel, QuarterPixel);
	cSetConsiderVector	(&Params->major_params.m_advMEParams, TRUE);
	cSetBlockSizeME		(&Params->major_params, cbt_16x16, cbt_4x4);

	Params->scale_num = 1;

	Params->candidates_num = 13;
	Params->MV_mult_koef = 4;

	Params->ConvertContrastFunc = Contrast_Convert;

	Params->use_uniform_filter = TRUE;
	Params->uniform_filter_strength = 0;

	Params->use_occlusion_processing = TRUE;
}
