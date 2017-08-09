/*
********************************************************************
(c) MSU Graphics and Multimedia Laboratory, Video Group 2003-2009
http://compression.ru/video/
********************************************************************
*/

#pragma once

/*!\brief Interface to visualization image that is given to metric.
*
*/
class IImageProxy
{
public:
    // returns pointer to RGB24 data. 
    // data layout is:  rgbrgbrgb...
    //                  rgbrgbrgb...
    //                  ...
    virtual BYTE* GetImagePtr() = 0;

    // set visualization info
    virtual void  SetImageInfo ( int w, int h ) = 0;

    virtual int  SetRegionConstraints( int top, int left, int bottom, int right) = 0;

    virtual int  GetPitch() = 0;
};

// process-time interface for retrieving buffers for visualization saving
class IVisualizationServer 
{
public:

    // returns pointer to visualization buffer
    virtual IImageProxy* GetVisualizationBuffer ( int vis_id ) = 0;

    //common for avg and perframe - call this to save visualization on the server
    virtual int PushVisualization ( int vis_id, int framenum, IImageProxy* vis_buffers ) = 0;

};
