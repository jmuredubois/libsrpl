/*
 * libSRPLransac.h
 * Header of the SR ransac method
 *
 * @author: James Mure-Dubois
 * @version: 2009.05.13
 */

#pragma once
// #define _SRPL_API_DLL  // in project properties
#ifdef _MSC_VER
	#ifdef _SRPL_API_DLL      //using file camSRavg.h, camSRavg.cpp to generate libSRPLavg.dll
		#define SRPLRSC_API __declspec(dllexport)
	#else              //using file libSRPLavg.h with libSRPLavg.dll
		#define SRPLRSC_API __declspec(dllimport)
	#endif
	#define SRPLRSC_LOC
#else
  #ifdef HAVE_GCCVISIBILITYPATCH
    #define SRPLRSC_API __attribute__ ((visibility("default")))
    #define SRPLRSC_LOC __attribute__ ((visibility("hidden")))
  #else
    #define SRPLRSC_API
    #define SRPLRSC_LOC
  #endif
#endif
//#ifdef __cplusplus
//extern "C" {
//#endif

#include "srBuf.h" // defines SRBUF
#include "srSegmBuf.h" // defines SRSEGMBUF
#include "srRscPlan.h" // defines SRSEGMBUF

class CamSRransac;
typedef CamSRransac* SRPLRSC;
SRPLRSC_API int PLRSC_Open(SRPLRSC* srPLRSC, SRBUF srBuf );
SRPLRSC_API int PLRSC_Close(SRPLRSC srPLRSC);
SRPLRSC_API int PLRSC_ransac(SRPLRSC srPLRSC, SRBUF srBuf, unsigned short* z, short* y, short* x, bool* isNaN, unsigned char segIdx);
SRPLRSC_API int PLRSC_SetSegmMap(SRPLRSC srPLRSC, SRBUF srBuf, unsigned char* segmMap);
SRPLRSC_API int PLRSC_GetIter(SRPLRSC srPLRSC);
SRPLRSC_API int PLRSC_GetIterMax(SRPLRSC srPLRSC);
SRPLRSC_API int PLRSC_SetIterMax(SRPLRSC srPLRSC, int iterMax);
SRPLRSC_API double PLRSC_GetDistPla(SRPLRSC srPLRSC);
SRPLRSC_API double PLRSC_SetDistPla(SRPLRSC srPLRSC, double distPla); // distance in mm
SRPLRSC_API double PLRSC_SetProjZRotMat(SRPLRSC srPLRSC, RSCPLAN *plan); // get projection matrix that turns best plane to z normal
SRPLRSC_API RSCPLAN PLRSC_GetPlaBest(SRPLRSC srPLRSC);
SRPLRSC_API RSCPLAN PLRSC_GetPlaCurr(SRPLRSC srPLRSC);
SRPLRSC_API RSCPLAN PLRSC_GetBestPla(SRPLRSC srPLRSC, unsigned int lev);

//#ifdef __cplusplus
//}
//#endif