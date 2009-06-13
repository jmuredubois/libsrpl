/*
 * libSRPLalign.h
 * Header of the SR ALIGNMENT method
 *
 * @author: James Mure-Dubois
 * @version: 2009.06.13
 */

#pragma once
// #define _SRPL_API_DLL  // in project properties
#ifdef _MSC_VER
	#ifdef _SRPL_API_DLL      //using file camSRavg.h, camSRavg.cpp to generate libSRPLavg.dll
		#define SRPLALI_API __declspec(dllexport)
	#else              //using file libSRPLavg.h with libSRPLavg.dll
		#define SRPLALI_API __declspec(dllimport)
	#endif
	#define SRPLALI_LOC
#else
  #ifdef HAVE_GCCVISIBILITYPATCH
    #define SRPLALI_API __attribute__ ((visibility("default")))
    #define SRPLALI_LOC __attribute__ ((visibility("hidden")))
  #else
    #define SRPLALI_API
    #define SRPLALI_LOC
  #endif
#endif
#ifdef __cplusplus
extern "C" {
#endif


class CamSRalign;
typedef CamSRalign* SRPLALI;
SRPLALI_API int PLALI_Open(SRPLALI* srPLALI);
SRPLALI_API int PLALI_Close(SRPLALI srPLALI);
SRPLALI_API int PLALI_align3plans(SRPLALI srPLALI, double mat[16], double n0[12], double n1[12]);


#ifdef __cplusplus
}
#endif