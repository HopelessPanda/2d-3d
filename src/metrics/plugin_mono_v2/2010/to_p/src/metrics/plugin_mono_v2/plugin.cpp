#include "metric.h"


int CMonoPlugin::Measure(int framenum, CLinkedAnchor& request, IDataServer* data_manager, IResultsServer* results_server, IVisualizationServer* vis_server) 
{
	//load new frame
	UpdateSource(data_manager);
	//transform mono according to prev results
	bool wasdone = TransformMono(2); //apply old params only
	// try to analyse
	t_Mono.ConvertToType(YUV_P);
	m_analyser.Analyse();
	//bad result? calculate w/o transform
	if (!Check() && wasdone)
	{
		t_Mono = m_Mono;
		t_Mono.ConvertToType(YUV_P);
		m_analyser.Analyse();
		TransformMono(0); //without using old params
	}
	else
		TransformMono(1); //with using old params
	if(!MeasureConf())
	{
		m_position = -1;
		m_flat = -1;
		m_difference = 0;
		m_brightness = 0;
	}
	else
		MultiPosAnalysis();
	PostResult(framenum, results_server);
	return 0;
}



