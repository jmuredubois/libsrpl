/*
 * CamFlagNaN.h
 * Header of the NaN flagging methods
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

