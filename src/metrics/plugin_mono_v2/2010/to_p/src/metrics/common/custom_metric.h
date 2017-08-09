/*
********************************************************************
(c) MSU Video Group 2003-2012, http://compression.ru/video/
This source code is property of MSU Graphics and Media Lab

This code may be distributed under LGPL
(see http://www.gnu.org/licenses/lgpl.html for more details).

E-mail: video-measure@compression.ru
********************************************************************
*/  

#pragma once

#include <vector>

// I'm too lazy including commontypes.h
#ifndef BYTE
/*! \var BYTE
*   \brief Defines unsigned char type as BYTE
*
*/	
typedef	unsigned char BYTE;
#endif

#include "../common/imetric.h"
#include "../common/dataanchor.h"
#include "../common/metricproperties.h"
 
// interfaces
#include "../common/imetricimage.h"
#include "../common/iframedata.h"
#include "../common/idataserver.h"
#include "../common/ivisserver.h"
#include "../common/iresultsserver.h"

#define CHESSBOARD_CELL_SIZE 32

class CCustomMetric: public IMetric {
private:
    bool m_bInited;
protected:
	// Static data
	CMetricProperties &m_properties;	// saved properties for fast retrieve, should be filled during construction
	char **m_metric_names;
	char **m_visualization_names;

	// Dynamic data
	std::vector<int> m_vis_ids;				// saved visualization ids
	std::vector<MetricID> metric_ids;		// saved metric IDs. this easily could be map[ metric_name -> metric_id ] 
	int m_frame_count;
	int m_sparsity;
	int m_num_of_measurement;

protected:
	void ExtractView(IDataServer* data_manager, CDataAnchor& anch, BYTE* dst) {
		int size = m_w * m_h;
		IMetricImage *im = data_manager->GetSource(&anch, IMetricImage::RRGB);
		float *src = im->GetR();
		for(int i=0; i<size; ++i)  *dst++ = (BYTE)(*src++ + 0.5f);
		src = im->GetG();
		for(int i=0; i<size; ++i)  *dst++ = (BYTE)(*src++ + 0.5f);
		src = im->GetB();
		for(int i=0; i<size; ++i)  *dst++ = (BYTE)(*src++ + 0.5f);
	}
	void ExtractLeftView(IDataServer* data_manager, BYTE* dst) {
		CDataAnchor lanch;
		lanch.view = 0; 
		ExtractView(data_manager, lanch, dst);
	}
	void ExtractRightView(IDataServer* data_manager, BYTE* dst) {
		CDataAnchor ranch;
		ranch.view = m_views - 1;
		ExtractView(data_manager, ranch, dst);
	}
	void PlaneRGBtoVis(BYTE* src, BYTE* dst) {
		int size = m_w * m_h;
		BYTE *src_r = src;
		BYTE *src_g = src_r + size;
		BYTE *src_b = src_g + size;
		for(int i=0; i<size; ++i) {
			*dst++ = *src_b++;
			*dst++ = *src_g++;
			*dst++ = *src_r++;
		}
	}
	void MakeDefVisualization(IImageProxy* vis, BYTE* left, BYTE* right) {
		int size = m_w * m_h;
		BYTE *buffer = (BYTE*)malloc(size*3);
		memcpy(buffer, left, size*3);
		
		for(int y=0; y<m_h; y+=CHESSBOARD_CELL_SIZE)
			for(int ly=0; ly<CHESSBOARD_CELL_SIZE && ly<m_h-y; ++ly)
				for(int x=(y % (CHESSBOARD_CELL_SIZE*2)); x<m_w; x+=CHESSBOARD_CELL_SIZE*2) {
					int index = (y+ly)*m_w + x;
					int scp = min(m_w-x, CHESSBOARD_CELL_SIZE);
					memcpy(buffer + index, right + index, scp);
					index += size;
					memcpy(buffer + index, right + index, scp);
					index += size;
					memcpy(buffer + index, right + index, scp);
				}
		vis->SetImageInfo(m_w, m_h);
		BYTE *raw_vis = vis->GetImagePtr();
		PlaneRGBtoVis(buffer, raw_vis);
		free(buffer);
	}
    void MakeDefVisualization(BYTE* left, BYTE* right, int visid, int frame_num, IDataServer* data_manager, IVisualizationServer* vis_server) {
        IImageProxy* vis = vis_server->GetVisualizationBuffer(visid);
        MakeDefVisualization(vis, left, right);
        vis_server->PushVisualization(visid, frame_num, vis);
    }
    CCustomMetric(const CCustomMetric&);

private:
    /* ------------------------------------------------------------------------------ */
    /*                            metric initialization                               */
    /* ------------------------------------------------------------------------------ */
    int Init( int width, int height, int num_views, int num_scales, int framenum ) {
        if (m_bInited)
            Deinit();
        m_w = width;
        m_h = height;
        m_scales = num_scales;
        m_views = num_views;
        m_frame_count = framenum;
        InitMetric();
        m_bInited = true;
        return true;
    }
    void Deinit() {
        if (m_bInited) {
//            DeinitMetric();
            m_bInited = false;
        }
    }
public:
    CCustomMetric(CMetricProperties& m_properties, char **m_metric_names, char **m_visualization_names): 
        m_properties(m_properties), m_metric_names(m_metric_names), m_visualization_names(m_visualization_names),
        m_frame_count(0),
        m_sparsity(1),
        m_num_of_measurement(1),
        m_bInited(false)
	{
		m_w = m_h = 0;
	}
    ~CCustomMetric() {
        Deinit();
    }
    virtual void InitMetric() {}
	__declspec(deprecated("Use your class destructor instead! CCustomMetric::IsInited() can be used to determine whether the Deinit is necessary."))
	virtual void DeinitMetric() {}

	bool IsInited() const { return m_bInited; }

    const char* GetDefaultConfiguration() {
		return NULL;
	}
	bool Config(const char** configuration_line) {
		return true;
	}
	int UpdateMetricModel(const IResultsServer* results_server, int num_of_measurement, int sparsity) {
		m_sparsity = sparsity;
		m_num_of_measurement = num_of_measurement;
		return 0;
	}

    /* ------------------------------------------------------------------------------ */
    /*                            measuring functions                                 */
    /* ------------------------------------------------------------------------------ */
	bool FrameMeasurementNeeded(int framenum) {
		return !(framenum%m_sparsity);
	}
	int VisualizeFrame(int framenum, IDataServer* data_manager, IVisualizationServer* vis_server) {
		// not used
		return 0;
	}
	int CalculateAverage(IResultsServer* results_server, IVisualizationServer* vis_server) {
		if (m_properties.m_metric_num != metric_ids.size())
			return 1;

		for(int i=0; i < m_properties.m_metric_num; ++i) {
			int frame_count, *frame_numbers;
			float *metric_values;
			float metric_avg = .0f;
			frame_count = results_server->GetMetricResults(metric_ids[i], &frame_numbers, &metric_values);
			if (frame_count == -1 /*|| frame_count != m_frame_count*/)
				return 1;
			for(int j=0; j<frame_count; ++j)
				metric_avg += metric_values[j];
			if (frame_count > 0)
				metric_avg /= frame_count;
			results_server->SaveAvgResult(metric_ids[i], metric_avg, "");
		}
		return 0;
	}


    /* ------------------------------------------------------------------------------ */
    /*                            retrieving metric info                              */
    /* ------------------------------------------------------------------------------ */
	char* GetMetrInfoURL() {
		return "http://compression.ru"; // insert link to the metric description here
	}
	int InitializeData(IFrameData* buffer, char* data_name) {
		return 0;
	}

	int RegisterProvidedMetrics(char*** metric_names) {
		*metric_names = m_metric_names;
		return m_properties.m_metric_num;
	}
	bool SetProvidedMetricIDs(const MetricID* ids) {
		metric_ids = std::vector<MetricID>(ids, ids + m_properties.m_metric_num);
		return true;
	}
	int RegisterProvidedVisualizations(char*** vis_names) {
		*vis_names = m_visualization_names;
		return m_properties.m_vis_num;
	}
	bool SetProvidedVisIDs(const int* ids) {
		m_vis_ids = std::vector<int>(ids, ids + m_properties.m_vis_num);
		return true;
	}
	int GetMetricDependencies(CLinkedAnchor* dependencies) {
		// no dependecies on external data
		return false;
	}
	CMetricProperties GetMetricProperties(const char* config_line, int nscales, int nviews, int nframes) {
		return m_properties;
	}
	bool IsGPU() {
		return m_properties.m_using_gpu;
	}
	bool IsConfigurable() {
		return m_properties.m_is_configurable;
	}
	int GetGeneratedDataNames(char*** m_generated_data) {
		return m_properties.m_num_generated_data;
	}
	char* GetName() {
		return m_properties.m_name;
	}
	char* GetInterfaceName() {
		return m_properties.m_interface_name;
	}
	const char* GetMetricUnitName() {
		return m_properties.m_metric_units_name;
	}
	int GetWidthMultiply() {
		return m_properties.m_width_mul;
	}
	int GetHeightMultiply() {
		return m_properties.m_height_mul;
	}
};
