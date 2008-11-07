/*
 * libSRPLcoordTrf.h
 * Header of the SR coordinate transformation method
 *
 * @author: James Mure-Dubois
 * @version: 2008.11.06
 */

#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include "srBuf.h" // defines SRBUF
#include "srCtrParams.h" // define SRCTR (for coordinates transform

// #define _SRPL_API_DLL  // in project properties
#ifdef _SRPL_API_DLL      //using file camSRavg.h, camSRavg.cpp to generate libSRPLavg.dll
#define SRPLCTR_API __declspec(dllexport)
#else              //using file libSRPLavg.h with libSRPLavg.dll
#define SRPLCTR_API __declspec(dllimport)
#endif

class CamSRcoordTrf;
typedef CamSRcoordTrf* SRPLCTR;
SRPLCTR_API int PLCTR_Open(SRPLCTR* srplCtr, SRBUF srBuf );
SRPLCTR_API int PLCTR_Close(SRPLCTR srplCtr);
SRPLCTR_API int PLCTR_CoordTrf(SRPLCTR srplCtr, SRBUF srBuf, SRCTR srCtr);
SRPLCTR_API unsigned short* PLCTR_GetZ(SRPLCTR srplCtr);
SRPLCTR_API short* PLCTR_GetY(SRPLCTR srplCtr);
SRPLCTR_API short* PLCTR_GetX(SRPLCTR srplCtr);

#ifdef __cplusplus
}
#endif