/*
 * libSRPLflagNaN.h
 * Header of the NaN flagging methods
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
SRPLSCAT_API bool* PLNN_FlagNaN(SRPLNAN srplNaN, SRBUF srBuf);
//SRPLSCAT_API int PLNN_GetNaNs(SRPLNAN srplNaN, SRBUF srBuf); // NOT YET IMPLEMENTED

#ifdef __cplusplus
}
#endif