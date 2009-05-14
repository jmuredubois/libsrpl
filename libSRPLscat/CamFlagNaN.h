/*
 * CamFlagNaN.h
 * Header of the NaN flagging methods
 *
 * @author: James Mure-Dubois
 * @version: 2008.10.30
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


/**
 * NaN flag class \n
 * This class: \n
 * - flags Not a Number (NaN) values  \n
 */
class CamFlagNaN //!< NaN flag class
{
public:
	//! constructor
    CamFlagNaN( SRBUF srBuf);
	//! destructor
	~CamFlagNaN();
	//! FlagNaN method
	int FlagNaN(SRBUF nanBuf);		//!< computes NaNs
	NANBUF GetNaNbuf(){return _imgNaN;};			//!< Returns the NaN buffer (full obj wiht hRows and nCols)
	bool* GetBoolBuf(){return _imgNaN.nanBool;};	//!< Returns the NaN buffer (bool)	

private:
	SRBUF savBuf;
	NANBUF _imgNaN;
	
#ifdef FLAGNTIMER
  CPreciseTimer _flagNTimer;	//!< timer for FlagNaN
  float			_timeFlagN_s;	//!< variable to hold FlagNaN time in seconds
#endif

};

