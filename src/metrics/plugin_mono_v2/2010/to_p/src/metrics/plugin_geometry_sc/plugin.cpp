#include "stdafx.h"
#include <metrics/common/custom_metric.h>
#include <metrics/common_algorithms/GeometryAnalyser/sanalyser.h>

#define _SC_OMP_

char *plugin_metric_names[] =		{ "rotate_distortion_2", "scale_distortion_2", "shift_distortion_2", "geometry_confidence_2" };
char *plugin_visualization_names[] =	{ "geometry_distortion" };
CMetricProperties plugin_properties = {
	0,			// m_src_backward_num
	0,			// m_src_fwd_num
	4,			// m_metric_num
	1,			// m_vis_num
	0,			// m_avg_vis_num

	// m_name
	"Geometry distortion measure",
	
	// m_interface_name
	"geometry_distortion_2",

	0,			// m_num_generated_data
	false,		// m_using_gpu
	false,		// m_is_configurable
	false,		// m_reusable 
	16,			// m_width_mul
	16,			// m_height_mul

	// m_metric_units_name
	"GEOMETRY"
};

class CGeometryPlugin: public CCustomMetric 
{
private:
	StereoAnalyser main_san;
	PNG_Image m_left, m_right;
	int m_width, m_height;
public:
    ~CGeometryPlugin() {};
	CGeometryPlugin(): CCustomMetric(plugin_properties, (char**)plugin_metric_names, (char**)plugin_visualization_names) {
	}
	void InitMetric();
	int Measure(int framenum, CLinkedAnchor& request, IDataServer* data_manager, IResultsServer* results_server, IVisualizationServer* vis_server);
	int CompareFramesLt(IResultsServer* results, int framenum1, int framenum2, int vis_id) 
	{
		return -1;
	}
};

int CGeometryPlugin::Measure(int framenum, CLinkedAnchor& request, IDataServer* data_manager, IResultsServer* results_server, IVisualizationServer* vis_server)
{
	ExtractLeftView(data_manager, m_left.GetData());
	m_left.Set(RGB_P);
	ExtractRightView(data_manager, m_right.GetData());
	m_right.Set(RGB_P);
	main_san.Analyse(); 
	aTransform atr = main_san.GetResults(G_LR);
	double conf = main_san.GetTotal(G_LR)*255;
	float rotation_value = float(- asin(atr.a) * 180.0f / M_PI);
	float scale_value = float(1 - atr.b);
	float shift_value = float(- atr.c / m_width * 1000);
	results_server->SaveResult(metric_ids[0], framenum, rotation_value);
	results_server->SaveResult(metric_ids[1], framenum, scale_value);
	results_server->SaveResult(metric_ids[2], framenum, shift_value);
	results_server->SaveResult(metric_ids[3], framenum, float(conf));
	return 0;
}

void CGeometryPlugin::InitMetric()
{
	m_width = m_w;
	m_height = m_h;
	m_left.Init(m_width, m_height, RGB_P);
	m_right.Init(m_width, m_height, RGB_P);
	main_san.Init(m_width, m_height);
	main_san.InitStereo(&m_left, &m_right);
}

extern "C" __declspec(dllexport) void CreateMetric(IMetric** metric) {
*metric = new CGeometryPlugin;
}

extern "C" __declspec(dllexport) void ReleaseMetric(IMetric** metric) {

	delete *metric;
}
