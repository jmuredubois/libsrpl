/*
 * libSRPLavg.h
 * Header of the SR buffer averaging method
 *
 * @author: James Mure-Dubois
 * @version: 2008.11.06
 */

#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include "srBuf.h"

// #define _SRPL_API_DLL  // in project properties
#ifdef _SRPL_API_DLL      //using file camSRavg.h, camSRavg.cpp to generate libSRPLavg.dll
#define SRPLAVG_API __declspec(dllexport)
#else              //using file libSRPLavg.h with libSRPLavg.dll
#define SRPLAVG_API __declspec(dllimport)
#endif

class CamSRavg;
typedef CamSRavg* SRPLAVG;
SRPLAVG_API int PLAVG_Open(SRPLAVG* srplAvg, SRBUF srBuf );
SRPLAVG_API int PLAVG_Close(SRPLAVG srplAvg);
SRPLAVG_API int PLAVG_LearnBackgroundInitReset(SRPLAVG srplAvg, SRBUF srBuf);
SRPLAVG_API int PLAVG_LearnBackground(SRPLAVG srplAvg, SRBUF srBuf);
//SRPLAVG_API int PLAVG_LearnBackgroundEnd(SRPLAVG srplAvg);
SRPLAVG_API SRBUF PLAVG_GetAvgBuf(SRPLAVG srplAvg);
SRPLAVG_API int PLAVG_SetAvgAlpha(SRPLAVG srplAvg, double alpha);
SRPLAVG_API double PLAVG_GetAvgAlpha(SRPLAVG srplAvg);


#ifdef __cplusplus
}
#endif