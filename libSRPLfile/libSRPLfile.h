/*
 * libSRPLfile.h
 * Header of the SR file handling library
 *
 * @author: James Mure-Dubois
 * @version: 2008.12.11
 */

#pragma once
// #define _SRPL_FLE_DLL  // in project properties
#ifdef _MSC_VER
	#ifdef _SRPL_FLE_DLL      //using file camSRavg.h, camSRavg.cpp to generate libSRPLavg.dll
		#define SRPLFILE_API __declspec(dllexport)
	#else              //using file libSRPLavg.h with libSRPLavg.dll
		#define SRPLFILE_API __declspec(dllimport)
	#endif
	#define SRPLFILE_LOC
#else
  #ifdef HAVE_GCCVISIBILITYPATCH
    #define SRPLFILE_API __attribute__ ((visibility("default")))
    #define SRPLCTR_LOC __attribute__ ((visibility("hidden")))
  #else
    #define SRPLFILE_API
    #define SRPLFILE_LOC
  #endif
#endif
#ifdef __cplusplus
extern "C" {
#endif


#define wxUSE_GUI 1		//!< using wxWidgets GUI components

/** the environment variable WXWIN should point to a valid wxWidget
 *  installation \n
 *  - "$(WXWIN)/include"  should be added to the include path \n
 *  - config specific: "$(WXWIN)/include/vc_lib/msvc/" should
 *    be added to the include path for "setup.h"
 */
//#include "wx/wxprec.h"
#ifndef  WX_PRECOMP
  #include "wx/wx.h"
#endif //precompiled headers
#include "wx/wx.h"			//!< base for wxWindows
#include "wx/ffile.h"		//!< for files
#include "srBuf.h"

class CamFileSRPL;
typedef CamFileSRPL* SRPLFILE;
SRPLFILE_API int PLFL_Open(SRPLFILE* srplFile, SRBUF srBuf, bool readOnly );
SRPLFILE_API int PLFL_Close(SRPLFILE srplFile);
SRPLFILE_API int PLFL_Read(SRPLFILE srplFile, SRBUF srBuf);
SRPLFILE_API int PLFL_Write(SRPLFILE srplFile, SRBUF srBuf);
SRPLFILE_API int PLFL_SetFileNameAmp(SRPLFILE srplFile, const char* fn);
SRPLFILE_API int PLFL_SetFileNamePha(SRPLFILE srplFile, const char* fn);

#ifdef __cplusplus
}
#endif