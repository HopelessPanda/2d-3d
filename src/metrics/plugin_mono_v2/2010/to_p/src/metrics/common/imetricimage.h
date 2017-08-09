/**
*  \file imetricimage.h
*   \brief Interface to access metric image
*/

#pragma once

/*!\brief Interface to image that is given to metric.
*
*    All color planes are stored without alignment.
*    Do not get pointer to color planes that differ from the one your metric was configured for, 
*    this may lead to unexpected results.
*/

class IMetricImage
{
public:

    enum COLORCOMPONENT {YYUV = 0, UYUV = 1, VYUV = 2, LLUV = 3, RRGB = 4, GRGB = 5, BRGB = 6};    // numeric values must match those in IMetricPlugin

    /**
    **************************************************************************
    *  \brief Get R color plane from image in RGB
    */
    virtual float*        GetR() = 0;

    /**
    **************************************************************************
    * \brief Get G color plane from image in RGB
    */
    virtual float*        GetG() = 0;

    /**
    **************************************************************************
    * \brief Get B color plane from image in RGB
    */
    virtual float*        GetB() = 0;

    /**
    **************************************************************************
    * \brief Get Y color plane from image in YUV
    */
    virtual float*        GetY() = 0;

    /**
    **************************************************************************
    * \brief Get U color plane from image in YUV or LUV
    */
    virtual float*        GetU() = 0;

    /**
    **************************************************************************
    * \brief Get V color plane from image in YUV or LUV
    */
    virtual float*        GetV() = 0;

    
    /**
    **************************************************************************
    * \brief Get L color plane from image in LUV
    */
    virtual float*        GetL() = 0;

    /**
    **************************************************************************
    * \brief Get width of the stored image
    */
    virtual int            GetWidth() = 0;

    /**
    **************************************************************************
    * \brief Get height of the stored image
    */
    virtual int            GetHeigth() = 0;
};