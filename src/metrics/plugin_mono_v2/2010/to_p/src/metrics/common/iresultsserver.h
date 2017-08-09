/*
********************************************************************
(c) MSU Graphics and Multimedia Laboratory, Video Group 2003-2009
http://compression.ru/video/
********************************************************************
*/

#pragma once

class IResultsServer 
{
public:
    // COMMENTS
    virtual int                     SaveResult ( MetricID metric, int framenum, float metr_res ) = 0; // returns 0 if ok
    // pass a pointer to your custom string with the result in the "custom_result"
    virtual int                     SaveAvgResult ( MetricID metric, float metr_res, const char* custom_result ) = 0;

    virtual float                   GetFrameResult ( MetricID metric, int framenum ) const = 0;
    virtual int                     GetMetricResults ( MetricID metric, int** frame_numbers, float** results ) const = 0; // returns number of entries

    virtual int                     FrameWasMeasured( MetricID metric, int framenum ) const = 0;
};