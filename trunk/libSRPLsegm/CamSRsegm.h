/*
 * CamSRsegm.h
 * Header of the SR buffer segmentation method
 *
 * @author: James Mure-Dubois
 * @version: 2009.05.11
 */

#pragma once
#ifndef _CRT_SECURE_NO_DEPRECATE
  #define _CRT_SECURE_NO_DEPRECATE // used to remove MS C4996 warning for fopen
								 // fopen is used only in raw binary PSF read
#endif
#ifndef SAFE_FREE
	#define SAFE_FREE(p)       { if(p) { delete (p);     (p)=0; } }
#endif


#include <list>		//for std::list
#include <math.h>
#include <complex> // for ANSI C99 complex numbers; WATCH OUT, this seems to be C++'s  complex<T>
#include "srBuf.h"
#include "srSegmBuf.h"
#include "libSRPLsegm.h"

/**
 * Camera average class \n
 * This class: \n
 * - averages SR buffers (s) \n
 */
class SRPLSEGM_LOC CamSRsegm //!< Camera frame class
{
public:
	//! constructor
    CamSRsegm(SRBUF srBuf);
	//! destructor
	~CamSRsegm();
	int LoadSegmSettings(const char* fn);	//!< load segmentation parameters
	//! Learn background method
	int Segment(SRBUF srBuf, SRBUF srBG, SRVARBUF srVar);
	SRSEGMBUF   GetSegmBuf();	//!< Returns the current average buffer
	

private:
	SRSEGMBUF   _segmBuf;
	SRSEGMBUF   _segmNonBayesBuf;

#ifdef SEGMTIMER
  CPreciseTimer _segmTimer;	    //!< timer for segmentation operation
  float			_timeSegm_s;	//!< variable to hold segmentation time in seconds
#endif

private:
	void SegmBufAlloc();
	void SegmBufFree();
  
};
