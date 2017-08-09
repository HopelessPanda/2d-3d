#include "stdafx.h"
#include <metrics/common/custom_metric.h>
#include <metrics/common_algorithms/GeometryAnalyser/sanalyser.h>

#define _SC_OMP_

//здесь все названия метрик, что видятся в samplehost и могут быть вызваны для анализа
char *plugin_metric_names[] =		{ "rotate_distortion_2", "scale_distortion_2", "shift_distortion_2", "geometry_confidence_2" };

//имя для сохранения картинок в samplehost, не юзаем
char *plugin_visualization_names[] =	{ "geometry_distortion" };
CMetricProperties plugin_properties = {
	0,			// m_src_backward_num
	0,			// m_src_fwd_num
	4,			// сколько метрик в плагине
	1,			// сколько визуализаций делает samplehost (можно ноль поставить, но тогда могут быть лаги в хосте)
	0,			// m_avg_vis_num

	// будет видно в текстовике с результатами
	"Geometry distortion measure",
	
	// тож самое
	"geometry_distortion_2",

	0,			// m_num_generated_data
	false,		// m_using_gpu
	false,		// m_is_configurable
	false,		// m_reusable 
	16,			// m_width_mul
	16,			// m_height_mul

	// в чем измеряется результат. вписал хрень
	"GEOMETRY"
};

// класс по шаблону плагина для хоста
class CGeometryPlugin: public CCustomMetric 
{
private:
	StereoAnalyser main_san; //главная шняга, анализирует всё и вся
	PNG_Image m_left, m_right; //картинки для левого и правого ракурса
	int m_width, m_height; //ширина и высота картинки для анализа. должны быть кратны 16
public:
    ~CGeometryPlugin() {};
	CGeometryPlugin(): CCustomMetric(plugin_properties, (char**)plugin_metric_names, (char**)plugin_visualization_names) {
	}
	void InitMetric();
	// по шаблону - вызывается при анализе кадра
	int Measure(int framenum, CLinkedAnchor& request, IDataServer* data_manager, IResultsServer* results_server, IVisualizationServer* vis_server);
	// по шаблону - вызывается для отбора "самой-самой" ужасной визуализации. давно не юзаем, так что там у меня чушь
	int CompareFramesLt(IResultsServer* results, int framenum1, int framenum2, int vis_id) 
	{
		return -1;
	}
};

//типа анализ текущих кадров
int CGeometryPlugin::Measure(int framenum, CLinkedAnchor& request, IDataServer* data_manager, IResultsServer* results_server, IVisualizationServer* vis_server)
{
	ExtractLeftView(data_manager, m_left.GetData()); //читаем левый кадр в буфер картинки
	m_left.Set(RGB_P); //он у нас в формате RGB_P  (RRRGGGBBB)
	ExtractRightView(data_manager, m_right.GetData()); //аналогично для правого
	m_right.Set(RGB_P);
	main_san.Analyse(); //запускаем анализ!
	aTransform atr = main_san.GetResults(G_LR); //получаем результаты анализа!
	double conf = main_san.GetTotal(G_LR)*255; //доверие = процент векторов, прошедших отсев (0..255)
	float rotation_value = float(- asin(atr.a) * 180.0f / M_PI); //превращаем коэффициент вращения (примерно равен радианам) в градусы
	float scale_value = float(1 - atr.b); //превращаем коэффициент зума в процент зума
	float shift_value = float(- atr.c / m_width * 1000); //коэффициент сдвига (в пикселах) делаем независимым от ширины анализируемой картинки
	results_server->SaveResult(metric_ids[0], framenum, rotation_value); //ну и сохраняем всё
	results_server->SaveResult(metric_ids[1], framenum, scale_value);
	results_server->SaveResult(metric_ids[2], framenum, shift_value);
	results_server->SaveResult(metric_ids[3], framenum, float(conf));
	return 0;
}

//вызывается один раз при начале анализа
void CGeometryPlugin::InitMetric()
{
	m_width = m_w; //m_w и m_h нам передаются. запомним их
	m_height = m_h;
	m_left.Init(m_width, m_height, RGB_P); //инициализируем кастомные буферы кадров
	m_right.Init(m_width, m_height, RGB_P);
	main_san.Init(m_width, m_height); //запускаем инициализацию внутренной структуры анализатора
	main_san.InitStereo(&m_left, &m_right); //запускаем всё, связанное с ME и стерео
}

//вот так она удаляется
extern "C" __declspec(dllexport) void CreateMetric(IMetric** metric) {
*metric = new CGeometryPlugin;
}

extern "C" __declspec(dllexport) void ReleaseMetric(IMetric** metric) {

	delete *metric;
}
