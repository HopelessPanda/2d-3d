/*
********************************************************************
(c) MSU Graphics and Multimedia Laboratory, Video Group 2003-2009
http://compression.ru/video/
********************************************************************
*/

#pragma once

#include "dataanchor.h"

class IFrameData;
class IMetricImage;

class IDataServer 
{
public:
// framenumbers are always relative! dataserver has it's own frame window to be managed from the host
    virtual IMetricImage*     GetSource ( CDataAnchor* anchor, IMetricImage::COLORCOMPONENT cc ) const = 0;
    virtual IFrameData*       GetData   ( const CLinkedAnchor* anchors, int anchornum) const = 0;
    virtual int               SaveData  ( const CLinkedAnchor* anchors, int anchornum, IFrameData *data ) = 0;
    virtual const char*       GetOutputFolder() = 0;
};