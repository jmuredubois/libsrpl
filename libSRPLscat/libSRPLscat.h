/*
 * libSRPLscat.h
 * Header of the scattering compensation method
 *
 * @author: James Mure-Dubois
 * @version: 2008.10.30
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

