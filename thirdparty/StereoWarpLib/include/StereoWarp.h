/*
*********************************************************************
(c) YUVsoft Corp., 2010 www.yuvsoft.com                              
This source code is a property of YUVsoft Corp.                      
                                                                     
This code can be distributed and used ONLY under a proper licensing  
agreement with YUVsoft                                               
*********************************************************************
*/

#pragma once

int MakeWarp(const BYTE* src, const BYTE* dep, BYTE* dst, BYTE* dst_holes_mask, int w, int h, double px, double zero);
