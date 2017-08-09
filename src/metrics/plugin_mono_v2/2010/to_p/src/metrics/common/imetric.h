// Interface to be supported by all metrics
#ifndef __IMETRIC_H__
#define __IMETRIC_H__

#include "dataanchor.h"
#include "metricproperties.h"

using namespace std;

class IDataServer;
class IVisualizationServer;
class IResultsServer;
class IFrameData;

typedef int MetricID;

class IMetric
{
    public:

/* ------------------------------------------------------------------------------ */
/*                            metric initialization                               */
/* ------------------------------------------------------------------------------ */

        virtual ~IMetric() {};    // must be overridden by descendants

        // main metric internal memory initialization function
        virtual int                             Init( int width, int height, int num_views, int num_scales, int framenum) = 0;
        // if metric is configurable, then metric configures itself and returns true if settings are correct (it MUST return true for empty string)
        virtual bool                            Config( const char** configuration_line) = 0;

        // this function will be called once before the whole sequence measurement. 
        // метрика должна записывать в frameslist номера кадров которые она хочет замерять 
        // num_of_measurement - номер замера, начиная с 0
        // sparsity - параметр прореженности. пусть для определенности будет так - 1 = замерять каждый кадр, больше - реже
        virtual int                             UpdateMetricModel( const IResultsServer* results_server, int num_of_measurement, int sparsity ) = 0;

/* ------------------------------------------------------------------------------ */
/*                            measuring functions                                 */
/* ------------------------------------------------------------------------------ */

        // calls before every frame processing
        virtual bool                            FrameMeasurementNeeded( int framenum ) = 0;

        // describe this function, Marat!
        // request [in] - thing to resolve. Обычно ваша метрика должна игнорировать этот параметр
        // Нужно это для того, если например ваш плагин различает замеры между текущим кадром и кадром вперед для левого и правого видов.
        // В этом случае в должны указать флаг m_reusable = false и в этой переменной будут записаны координаты целей для метрики
        // Иначе флаг должен быть true и вы должны генерировать 2 метрики (см. RegisterProvidedMetrics) и этот параметр вам не пригодится
        // data_manager  [in\out] - for retrieving framedata and source images  
        virtual int                             Measure( int framenum, CLinkedAnchor& request, IDataServer* data_manager, IResultsServer* results_server, IVisualizationServer* vis_server ) = 0;

        // this function is called after whole sequence processing. use resultsserver and visualization server for the sequence results updating
        virtual int                             CalculateAverage( IResultsServer* results_server, IVisualizationServer* vis_server ) = 0;

        // this functions uses to compare metrics results for two metrics while selecting bad frames
        // Lt = less than. lt returns frame which has lower results and which bad frame should be saved
        virtual int                             CompareFramesLt( IResultsServer* results, int framenum1, int framenum2, int vis_id ) = 0;

        // save visualization for the specified frame
        virtual int                             VisualizeFrame( int framenum, IDataServer* data_manager, IVisualizationServer* vis_server ) = 0;

        // dobavit 


/* ------------------------------------------------------------------------------ */
/*                            support functions for metrics                       */
/*                those functions should work without calling Init()!             */
/* ------------------------------------------------------------------------------ */
/**************   always called in the following order   ****************/

        // metric should fill CMetricProperties struct
        virtual CMetricProperties               GetMetricProperties( const char* config_line, int nscales, int nviews, int nframes ) = 0;

        // put here a list of pairs < dataname, anchor_list >
        // anchor_list is a vector of consecutive data anchors that defines a set of data
        // i.e. it could be [ < "YYUV_OF_X", [CDataAnchor(0), CDataAnchor(max_views) ] > ] to request 
        // optical flow for Y component from the left view to the rightmost view
        // return number of dependencies (could be zero)
        virtual int                             GetMetricDependencies ( CLinkedAnchor* dependencies ) = 0;
        //@TODO:


        // list of data that is generated during metric calculation. note that there is only one 
        // calculation per metric is available, but several data buffers could be generated
        // return number of data provided
        virtual int                             GetGeneratedDataNames ( char*** m_generated_data ) = 0;

        // after retrieving names, plugin should initialize buffer for provided data
        virtual int                             InitializeData(IFrameData* buffer, char* data_name) = 0;

        // put here a list of metric values names provided by this plugin
        // return a number of metrics provided
        virtual int                             RegisterProvidedMetrics( char*** metric_names ) = 0;
        // callback function that provides MetricID's for all values names form RegisterProvidedMetrics() call
        // number of ids corresponds to the number in the "RegisterProvidedMetrics" call
        virtual bool                            SetProvidedMetricIDs( const MetricID* ids ) = 0;

        // put here a list of visualization names provided by this metric
        // return a number of visualizations provided
        virtual int                             RegisterProvidedVisualizations( char*** vis_names ) = 0;
        // use those id's to retrieve visualization buffers
        virtual bool                            SetProvidedVisIDs( const int* ids ) = 0;
        
        // this is called only if metric is configurable
        virtual const char*                     GetDefaultConfiguration( ) = 0;

        virtual const char*                     GetMetricUnitName() = 0;

        bool                            m_initialized;    // metric is initialized and ready for actions ( optional configure and launch )
        bool                            m_configured;     // metric is directly configured

        // base sequence parameters. don't use these as the general width, always take input IFrameData or IMetricImage parameters!!
        int                             m_w;              // base working width
        int                             m_h;              // base working height

        // permanent sequence parameters. those surely wont change during processing!
        int                             m_views;
        int                             m_scales;

};

#endif