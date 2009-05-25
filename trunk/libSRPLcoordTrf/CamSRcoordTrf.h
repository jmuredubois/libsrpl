/*
 * CamSRcoordTrf.h
 * Header of the SR coordinate transformation method
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
