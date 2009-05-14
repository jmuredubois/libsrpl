/*
 * CamSRransac.h
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
#include <vector>	//for std::vector
#include <math.h>
#include <ctime>	// for time
#include <complex> // for ANSI C99 complex numbers; WATCH OUT, this seems to be C++'s  complex<T>
#include "srBuf.h" // defines SRBUF
#include "srSegmBuf.h" // defines SRBUF
#include <Eigen/Core>
#include <Eigen/SVD>

/**
 * Camera average class \n
 * This class: \n
 * - averages SR buffers (s) \n
 */
class CamSRransac //!< Camera frame class
{
public:
	//! constructor
    CamSRransac(SRBUF srBuf);
	//! destructor
	~CamSRransac();
	//! coordinate transform  method
	int ransac(SRBUF srBuf, unsigned short* z, short* y, short* x, bool* isNaN, unsigned char* segmMap, unsigned char segIdx);
	int GetIter(){return _nIter;};

private:
	SRBUF   _srBuf;
	int	   _nIter;
	float  _dist2pla; 
	int	   _nInliers;
	std::vector<int> _inliers;
	std::vector<int> _perms;
	SRSEGMBUF _inBuf;
	int    _nIterMax;
	int    _inliersStop;
	double _nVec[4];


#ifdef AVGTIMER
  CPreciseTimer _ctrTimer;	//!< timer for ransac operation
  float			_timeCtr_s;	//!< variable to hold ransac. time in seconds
#endif

private:
	void RscBufAlloc();
	void RscBufFree();
	int GenPerms(bool* isNaN, unsigned char* segmMap); //!< generate permutations
	unsigned int time_seed();
};
