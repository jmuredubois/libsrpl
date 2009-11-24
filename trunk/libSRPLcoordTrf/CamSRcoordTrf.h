/*
 * CamSRcoordTrf.h
 * Header of the SR coordinate transformation method
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
#include "srBuf.h" // defines SRBUF
#include "srCtrParams.h" // define SRCTR (for coordinates transform

/**
 * Camera average class \n
 * This class: \n
 * - averages SR buffers (s) \n
 */
class CamSRcoordTrf //!< Camera frame class
{
public:
	//! constructor
    CamSRcoordTrf(SRBUF srBuf);
	//! destructor
	~CamSRcoordTrf();
	//! coordinate transform  method
	int CoordTrf(SRBUF srBuf, SRCTR pa);
	unsigned short* GetZ(){return _z;};
	short* GetY(){return _y;};
	short* GetX(){return _x;};
	float GetAvgZ(){return _avgZ;};
	float GetAvgY(){return _avgY;};
	float GetAvgX(){return _avgX;};
	float GetMinZ(){return _minZ;};
	float GetMinY(){return _minY;};
	float GetMinX(){return _minX;};
	float GetMaxZ(){return _maxZ;};
	float GetMaxY(){return _maxY;};
	float GetMaxX(){return _maxX;};

private:
	SRBUF   _srBuf;
	unsigned short* _z;
	short* _y;
	short* _x;
	float _avgX, _minX, _maxX, _avgY, _minY, _maxY;
	float _avgZ, _minZ, _maxZ ;

#ifdef AVGTIMER
  CPreciseTimer _ctrTimer;	//!< timer for coordTrf operation
  float			_timeCtr_s;	//!< variable to hold coordTrf. time in seconds
#endif

private:
	void CtrBufAlloc();
	void CtrBufFree();
};
