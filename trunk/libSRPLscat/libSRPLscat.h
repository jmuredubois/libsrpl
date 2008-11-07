/*
 * libSRPLscat.h
 * Header of the scattering compensation method
 *
 * @author: James Mure-Dubois
 * @version: 2008.10.30
 */

#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include "CamFlagNaN.h" //
#include "srBuf.h"

// #define _SRPL_API_DLL  // in project properties
#ifdef _SRPL_API_DLL      //using file CamScattering.h, CamScattering.cpp to generate libSRPLscat.dll
#define SRPLSCAT_API __declspec(dllexport)
#else              //using file libSRPLscat.h with libSRPLscat.dll
#define SRPLSCAT_API __declspec(dllimport)
#endif

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