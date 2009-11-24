/*
 * libSRPLfile.h
 * Header of the SR file handling library
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
  * THIS LIBRARY IS NOT COMPLETE YET, AND CURRENTLY NOT NEEDED
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