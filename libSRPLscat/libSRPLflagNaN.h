/*
 * libSRPLflagNaN.h
 * Header of the NaN flagging methods
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
	#ifdef _SRPL_API_DLL      //using file camSRavg.h, camSRavg.cpp to generate libSRPLavg.dll
		#define SRPLSCAT_API __declspec(dllexport)
	#else              //using file libSRPLavg.h with libSRPLavg.dll
		#define SRPLSCAT_API __declspec(dllimport)
	#endif
	#define SRPLSCAT_LOC
#else
  #ifdef HAVE_GCCVISIBILITYPATCH
    #define SRPLSCAT_API __attribute__ ((visibility("default")))
    #define SRPLCTR_LOC __attribute__ ((visibility("hidden")))
  #else
    #define SRPLSCAT_API
    #define SRPLSCAT_LOC
  #endif
#endif
#ifdef __cplusplus
extern "C" {
#endif

#include "srBuf.h"

#ifndef NANBUFHEADER
#define NANBUFHEADER
typedef struct nanVar {
    bool* nanBool;
    int nCols;
    int nRows;
    int bufferSizeInBytes;
} NANBUF;
#endif

class CamFlagNaN;
typedef CamFlagNaN* SRPLNAN;
SRPLSCAT_API int PLNN_Open(SRPLNAN* srplNaN, SRBUF srBuf );
SRPLSCAT_API int PLNN_Close(SRPLNAN srplNaN);
SRPLSCAT_API int PLNN_FlagNaN(SRPLNAN srplNaN, SRBUF srBuf);
SRPLSCAT_API NANBUF PLNN_GetNaNbuf(SRPLNAN srplNaN);
SRPLSCAT_API bool* PLNN_GetBoolBuf(SRPLNAN srplNaN);

#ifdef __cplusplus
}
#endif