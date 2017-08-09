/*
********************************************************************
(c) MSU Graphics and Multimedia Laboratory, Video Group 2003-2009
http://compression.ru/video/
********************************************************************
*/

#pragma once

class IMetricImage;
class IFrameData;
class ICoreSupported;
class INamesSupported;
class CDataAnchor;

class IDataManager :    public INamesSupported,
                        public ICoreSupported,
                        public IBaseClass
{
public:

    IMetricImage* GetSource     ( CDataAnchor &anchor, IMetricImage::COLORCOMPONENT cc             ) = 0;
    IMetricImage* GetSourceSlice( CDataAnchor &anchor, IMetricImage::COLORCOMPONENT cc, int offset ) = 0;
    IFrameData* GetData     ( std::vector<CDataAnchor> &anchors             ) = 0;
    IFrameData* GetDataSlice( std::vector<CDataAnchor> &anchors, int offset ) = 0;
    int AddData     ( std::vector<CDataAnchor> &anchors, IFrameData *data );
    int AddDataSlice( std::vector<CDataAnchor> &anchors, IFrameData *data, int offset ) = 0;
};