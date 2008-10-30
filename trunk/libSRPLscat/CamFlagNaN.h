/*
 * CamFlagNaN.h
 * Header of the NaN flagging methods
 *
 * @author: James Mure-Dubois
 * @version: 2008.10.30
 */

#pragma once
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

// #define _SRPL_API_DLL  // in project properties
#ifdef _SRPL_API_DLL      //using file libSRparlab.h, libSRparlab.cpp to generate libSRparlab.dll
#define SRPLSCAT_API __declspec(dllexport)
#else              //using file libSRparlab.h with libSRparlab.dll
#define SRPLSCAT_API __declspec(dllimport)
#endif

class CamFlagNaN;
typedef CamFlagNaN* SRPLNAN;
SRPLSCAT_API int PLNN_Open(SRPLNAN* srplNaN, SRBUF srBuf );
SRPLSCAT_API int PLNN_Close(SRPLNAN srplNaN);
SRPLSCAT_API bool* PLNN_FlagNaN(SRPLNAN srplNaN, SRBUF srBuf);
SRPLSCAT_API int PLNN_GetNaNs(SRPLNAN srplNaN, SRBUF srBuf); // NOT YET IMPLEMENTED

#ifdef __cplusplus
}
#endif