/*
********************************************************************
(c) MSU Graphics and Multimedia Laboratory, Video Group 2003-2009
http://compression.ru/video/
********************************************************************
*/

/* 
Interface to the general data passed between metrics.
*/

#pragma once

// если хотеть пользоваться общими данными без знания конкретного класса - void* к сожалению - единственный выход,
// а о конвеншнах прийдется договариваться голосом. вообще я сейчас понял что ставить зависимость плагинов друг от друга нехорошо.
// при этом, в любом случае, плагин должен решать проблему создания объектов framedata - для загрузки из стрима, например.

class IFrameData
{
public:

    // retrieve short interface name 
    virtual char*       GetName() = 0;

    // get full data name
    virtual char*       GetLongName() = 0;

    // stores element by coordinates to the pointer
    virtual void        GetElement( int x, int y, void* elem ) = 0;

    // sets element by coordinates to the new value 
	virtual int         SetElement( int x, int y, void* val ) = 0;

    virtual int         Allocate(int w, int h, int elemsize, int memsize /* this will usually equals to w*h*elemsize*/) = 0;

    // data should be able to save itself to the disk
	// virtual int SaveDataToFile( const char* filename) = 0;

    // and load from the disk
	// virtual int LoadDataFromFile ( const char* filename) = 0;

    // retrieve data info
    virtual int GetWidth() = 0;
    virtual int GetHeight() = 0;

    // retrieve name of the generator plugin
    virtual int GetParentID() = 0;

    int m_elemsize;
    int m_w;
    int m_h;

};