/*
********************************************************************
(c) MSU Graphics and Multimedia Laboratory, Video Group 2003-2009
http://compression.ru/video/
********************************************************************
*/

/**
*  \file launchmetricbaseunit.h
*  \brief Declaration of class CLaunchMetricBaseUnit
*
*/
#pragma once

#include <string>
#include <deque>

using namespace std;

struct _anchor 
{
public:
    _anchor(): view(0), time(0), scale(0){};
    _anchor( int nview ): view(nview), time(0), scale(0){};
    _anchor( int nview, int ntime ): view(nview), time(ntime), scale(0){};
    _anchor( int nview, int ntime, int nscale ): view(nview), time(ntime), scale(nscale){};

    _anchor( const _anchor& coord )
    {
        view = coord.view;
        time = coord.time;
        scale = coord.scale;
    }

    int view;
    int time;
    int scale;
};

typedef struct _anchor CDataAnchor;

typedef std::vector< CDataAnchor > CAnchorVector;

typedef std::deque <std::pair< std::string, CAnchorVector >> CLinkedAnchor;