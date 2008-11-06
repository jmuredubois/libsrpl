/*
 * CamSRavg.h
 * Header of the SR buffer averaging method
 *
 * @author: James Mure-Dubois
 * @version: 2008.11.06
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

// #define _SRPL_API_DLL  // in project properties
#ifdef _SRPL_API_DLL      //using file libSRparlab.h, libSRparlab.cpp to generate libSRparlab.dll
#define SRPLAVG_API __declspec(dllexport)
#else              //using file libSRparlab.h with libSRparlab.dll
#define SRPLAVG_API __declspec(dllimport)
#endif

/**
 * Camera average class \n
 * This class: \n
 * - averages SR buffers (s) \n
 */
class CamSRavg //!< Camera frame class
{
public:
	//! constructor
    CamSRavg(SRBUF srBuf);
	//! destructor
	~CamSRavg();
	//! init Learn background method
	int LearnBackgroundInitReset(SRBUF srBuf);
	//! Learn background method
	int LearnBackground(SRBUF srBuf);
	//! End Learn background method
	//int LearnBackgroundEnd();
	SRBUF   GetAvgBuf();		//!< Returns the currebnt average buffer
	int	   SetAvgAlpha(double alpha); //!< Sets Alpha for IIR learning (alpha = 0.0 disables IIR)
	double GetAvgAlpha(); //!< retrieves alaph for IIR learning (alpha==0.0 means IIR is disabled)

private:
	SRBUF   _avgBuf;
	double	_avgAlpha;		//!< alpha factor for adaptive background learning
	double	*_avgImgPha, *_avgImgAmp;		//!< average image buffer (double)
							/**< contains iterated average data of distance image
							 and amplitude image (not coord transformed)
							(same dimensions as _srbuf) */
	double	*_avgVarPha, *_avgVarAmp;		//!< average variance image buffer (double)
							/**< contains iterated variance data of distance image
							 and amplitude image (not coord transformed)
							(same dimensions as _srbuf) */
	double	*_avgSquPha, *_avgSquAmp;		//!< average squared sum image buffer (double)
							/**< contains squared sum of distance image
							 and amplitude image (not coord transformed)
							(same dimensions as _srbuf) */
	double	*_avgPhiPha, *_avgPhiAmp;		//!< average average sum image buffer (double)
							/**< contains average of distance image
							 and amplitude image (separate rather than vector addition)
							(same dimensions as _srbuf) */
	int		_avgCnt;			//!< count of average images

#ifdef AVGTIMER
  CPreciseTimer _avgTimer;	//!< timer for averaging operation
  float			_timeAvg_s;	//!< variable to hold averaging. time in seconds
#endif

private:
	void AvgBufAlloc();
	void AvgBufFree();
  
};
