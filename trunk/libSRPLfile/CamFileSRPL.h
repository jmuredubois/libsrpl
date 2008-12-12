/*
 * CamFileSRPL.h
 * Header of the file handling method
 *
 * @author: James Mure-Dubois
 * @version: 2008.12.11
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
