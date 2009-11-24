/*
 * CamFileSRPL.h
 * Header of the file handling method
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
#ifndef _CRT_SECURE_NO_DEPRECATE
  #define _CRT_SECURE_NO_DEPRECATE // used to remove MS C4996 warning for fopen
								 // fopen is used only in raw binary PSF read
#endif
#ifndef SAFE_FREE
	#define SAFE_FREE(p)       { if(p) { delete (p);     (p)=0; } }
#endif

#include "srBuf.h"

/**
 * Camera frame class \n
 * This class: \n
 * - computes scattering compensation (s) \n
 */
class CamFileSRPL //!< Camera frame class
{
public:
	//! constructor
    CamFileSRPL(SRBUF srBuf, bool readOnly);
	//! destructor
	~CamFileSRPL();
	//!  Read method
	int Read(SRBUF rdBuf);
	//!  Write method
	int Write(SRBUF wrBuf);
	

private:
	SRBUF       _curBuf;
    IMGSIZE _imgSz;						//!< size of original image
  
#ifdef FILEOTIMER
  CPreciseTimer _fileOTimer;	//!< timer for file operations
  float			_timefileO_s;	//!< variable to hold File IO time in seconds
#endif

public:
  int SetFileNameAmp(const char* fn);	//!< load scattering correction parameters
  int SetFileNamePha(const char* fn);	//!< load scattering correction parameters
  
private:

};
