/*
 * libSRPLsegm.h
 * Header of the SR buffer segmentation method
 *
 *    Copyright 2009 James Mure-Dubois, Heinz Hügli and Institute of Microtechnology of EPFL.
 *      http://parlab.epfl.ch/
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $Rev$:
 * $Author$:
 * $Date$:
 */

#pragma once
// #define _SRPL_API_DLL  // in project properties
#ifdef _MSC_VER
	#ifdef _SRPL_API_DLL      //using file camSRsegm.h, camSRsegm.cpp to generate libSRPLsegm.dll
		#define SRPLSEGM_API __declspec(dllexport)
	#else              //using file libSRPLsegm.h with libSRPLsegm.dll
		#define SRPLSEGM_API __declspec(dllimport)
	#endif
	#define SRPLSEGM_LOC
#else
  #ifdef HAVE_GCCVISIBILITYPATCH
    #define SRPLSEGM_API __attribute__ ((visibility("default")))
    #define SRPLSEGM_LOC __attribute__ ((visibility("hidden")))
  #else
    #define SRPLSEGM_API
    #define SRPLSEGM_LOC
  #endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "srBuf.h"
#include "srVarBuf.h"
#include "srSegmBuf.h"

#ifndef NANBUFHEADER
#define NANBUFHEADER
typedef struct nanVar {
    bool* nanBool;
    int nCols;
    int nRows;
    int bufferSizeInBytes;
} NANBUF;
#endif

class SRPLSEGM_LOC CamSRsegm;
typedef CamSRsegm* SRPLSEGM;
SRPLSEGM_API int PLSEGM_Open(SRPLSEGM* srplSegm, SRBUF srBuf );
SRPLSEGM_API int PLSEGM_Close(SRPLSEGM srplSegm);
SRPLSEGM_API int PLSEGM_LoadSegmSettings(SRPLSEGM srplSegm, const char* fn);
SRPLSEGM_API int PLSEGM_Segment(SRPLSEGM srplSegm, SRBUF srBuf, NANBUF nanBuf, SRBUF srBG, NANBUF nanBG, SRVARBUF srVar);
SRPLSEGM_API int PLSEGM_SegmentXYZ(SRPLSEGM srplSegm, SRBUF srBuf, NANBUF nanBuf, SRBUF srBG, NANBUF nanBG, SRVARBUF srVar, unsigned short* z, unsigned short* zBG);
SRPLSEGM_API SRSEGMBUF PLSEGM_GetSegmBuf(SRPLSEGM srplSegm);

#ifdef __cplusplus
}
#endif