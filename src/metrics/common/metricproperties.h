#ifndef  __METRIC_SETTINGS_H__
#define  __METRIC_SETTINGS_H__

// its actually plugin properties, but who cares?
struct _CMetricProperties
{
    // request this number of backward frames 
    int         m_src_backward_num;

    // request this number of forward frames
    int         m_src_fwd_num;

    // number of metrics in the plugin
    int         m_metric_num;

    // number of generated perframe visualizations 
    int         m_vis_num;

    // number of overall vis
    int         m_avg_vis_num;

    // plugin short name 
    char*       m_name;
    
    // plugin full name
    char*       m_interface_name;
    
    // number of generated data
    int         m_num_generated_data;
    
    // whether metric will use CUDA
    bool        m_using_gpu;

    // whether metric is configurable
    bool        m_is_configurable;

    // IMPORTANT! set this property to true if one instance could be used several times 
    // for the one frame. Generally, algorithms could be reused ( we could calculate ME from left to right 
    // and from the right to the left with only one instance ), and metrics are not.
    bool        m_reusable; 

    // desired source images dimensions alignment
    int         m_width_mul;
    int         m_height_mul;

	// metric units name
	char*		m_metric_units_name;
};

typedef struct _CMetricProperties CMetricProperties;
#endif