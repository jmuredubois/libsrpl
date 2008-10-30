/*
 * CamFlagNaN.h
 * Header of the NaN flagging methods for wxSRparlab
 *
 * @author: James Mure-Dubois
 * @version: 2008.10.23
 */

#pragma once
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

class CamFrame;

/**
 * Camera frame class \n
 * This class: \n
 * - computes scattering compensation (s) \n
 */
class CamFlagNaN //!< Camera frame class
{
public:
	//! constructor
    CamFlagNaN( SRBUF srBuf);
	//! destructor
	~CamFlagNaN();
	//! FlagNaN method
	bool* FlagNaN(SRBUF nanBuf);
	bool* GetNaNs();		//!< Returns the PSF image (double)
	

private:
	CamFrame	*_camFrm; // parent main wnd
	//bool*	_imgNaN; // NaN image
	SRBUF savBuf;
	NANBUF _imgNaN;
	
#ifdef FLAGNTIMER
  CPreciseTimer _flagNTimer;	//!< timer for FlagNaN
  float			_timeFlagN_s;	//!< variable to hold FlagNaN time in seconds
#endif

};

