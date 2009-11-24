/*
 * libSRPLavg.h
 * Header of the SR buffer averaging method
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
		#define SRPLAVG_API __declspec(dllexport)
	#else              //using file libSRPLavg.h with libSRPLavg.dll
		#define SRPLAVG_API __declspec(dllimport)
	#endif
	#define SRPLAVG_LOC
#else
  #ifdef HAVE_GCCVISIBILITYPATCH
    #define SRPLAVG_API __attribute__ ((visibility("default")))
    #define SRPLAVG_LOC __attribute__ ((visibility("hidden")))
  #else
    #define SRPLAVG_API
    #define SRPLAVG_LOC
  #endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "srBuf.h"
#include "srVarBuf.h"

class SRPLAVG_LOC CamSRavg;
typedef CamSRavg* SRPLAVG;
SRPLAVG_API int PLAVG_Open(SRPLAVG* srplAvg, SRBUF srBuf );
SRPLAVG_API int PLAVG_Close(SRPLAVG srplAvg);
SRPLAVG_API int PLAVG_LearnBackgroundInitReset(SRPLAVG srplAvg, SRBUF srBuf);
SRPLAVG_API int PLAVG_LearnBackground(SRPLAVG srplAvg, SRBUF srBuf);
//SRPLAVG_API int PLAVG_LearnBackgroundEnd(SRPLAVG srplAvg);
SRPLAVG_API SRBUF PLAVG_GetAvgBuf(SRPLAVG srplAvg);
SRPLAVG_API SRVARBUF PLAVG_GetAvgVar(SRPLAVG srplAvg);
SRPLAVG_API int PLAVG_GetAvgCnt(SRPLAVG srplAvg);
SRPLAVG_API int PLAVG_SetAvgAlpha(SRPLAVG srplAvg, double alpha);
SRPLAVG_API double PLAVG_GetAvgAlpha(SRPLAVG srplAvg);


#ifdef __cplusplus
}
#endif