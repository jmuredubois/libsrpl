/*
 * libSRPLscat.h
 * Header of the scattering compensation method
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

#include "CamFlagNaN.h" //
#include "srBuf.h"

class CamScattering;
typedef CamScattering* SRPLSCAT;
SRPLSCAT_API int PLSC_Open(SRPLSCAT* srplScat, SRBUF srBuf );
SRPLSCAT_API int PLSC_Close(SRPLSCAT srplScat);
SRPLSCAT_API int PLSC_Compensate(SRPLSCAT srplScat, SRBUF srBuf, NANBUF nanBuf);
SRPLSCAT_API int PLSC_SetCompensationTarget(SRPLSCAT srplScat, SRBUF srBuf, NANBUF nanBuf);
//SRPLSCAT_API int PLSC_CompensateAgain(SRPLSCAT srplScat);  // NOT YET IMPLEMENTED
SRPLSCAT_API int PLSC_LoadScatSettings(SRPLSCAT srplScat, const char* fn);

#ifdef __cplusplus
}
#endif

