/*
 * libSRPLcoordTrf.h
 * Header of the SR coordinate transformation method
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
		#define SRPLCTR_API __declspec(dllexport)
	#else              //using file libSRPLavg.h with libSRPLavg.dll
		#define SRPLCTR_API __declspec(dllimport)
	#endif
	#define SRPLCTR_LOC
#else
  #ifdef HAVE_GCCVISIBILITYPATCH
    #define SRPLCTR_API __attribute__ ((visibility("default")))
    #define SRPLCTR_LOC __attribute__ ((visibility("hidden")))
  #else
    #define SRPLCTR_API
    #define SRPLCTR_LOC
  #endif
#endif
#ifdef __cplusplus
extern "C" {
#endif

#include "srBuf.h" // defines SRBUF
#include "srCtrParams.h" // define SRCTR (for coordinates transform

class CamSRcoordTrf;
typedef CamSRcoordTrf* SRPLCTR;
SRPLCTR_API int PLCTR_Open(SRPLCTR* srplCtr, SRBUF srBuf );
SRPLCTR_API int PLCTR_Close(SRPLCTR srplCtr);
SRPLCTR_API int PLCTR_CoordTrf(SRPLCTR srplCtr, SRBUF srBuf, SRCTR srCtr);
SRPLCTR_API unsigned short* PLCTR_GetZ(SRPLCTR srplCtr);
SRPLCTR_API short* PLCTR_GetY(SRPLCTR srplCtr);
SRPLCTR_API short* PLCTR_GetX(SRPLCTR srplCtr);

SRPLCTR_API float PLCTR_GetAvgZ(SRPLCTR srplCtr);
SRPLCTR_API float PLCTR_GetAvgY(SRPLCTR srplCtr);
SRPLCTR_API float PLCTR_GetAvgX(SRPLCTR srplCtr);
SRPLCTR_API float PLCTR_GetMinZ(SRPLCTR srplCtr);
SRPLCTR_API float PLCTR_GetMinY(SRPLCTR srplCtr);
SRPLCTR_API float PLCTR_GetMinX(SRPLCTR srplCtr);
SRPLCTR_API float PLCTR_GetMaxZ(SRPLCTR srplCtr);
SRPLCTR_API float PLCTR_GetMaxY(SRPLCTR srplCtr);
SRPLCTR_API float PLCTR_GetMaxX(SRPLCTR srplCtr);
#ifdef __cplusplus
}
#endif