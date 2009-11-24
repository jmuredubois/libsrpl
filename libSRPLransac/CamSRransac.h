/*
 * CamSRransac.h
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
#include <vector>	//for std::vector
#include <math.h>
#include <ctime>	// for time
#include <complex> // for ANSI C99 complex numbers; WATCH OUT, this seems to be C++'s  complex<T>
#include "srBuf.h" // defines SRBUF
#include "srSegmBuf.h" // defines SRBUF
#include "srRscPlan.h" // defines SRBUF
#include <Eigen/Core>
#include <Eigen/SVD> // for SVD matrix decomp
#include <Eigen/LU>	 // for LU matrix decomp
#include <Eigen/Geometry>	 // for geometry transf (quaternions, etc)


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
	int ransac(SRBUF srBuf, unsigned short* z, short* y, short* x, bool* isNaN, unsigned char segIdx);
	int SetSegmMap(SRBUF srBuf, unsigned char* segmMap);
	int GetIter(){return _nIter;};
	int GetIterMax(){return _nIterMax;};
	int SetIterMax(int iterMax);
	double  GetDistPla(){return _dist2pla;}; //!< threshold for distance to plane in mm
	double  SetDistPla(double distPla);	
	double SetProjZRotMat(RSCPLAN* plan);
	RSCPLAN GetPlaBest(){return _plaBst;};
	RSCPLAN GetPlaCurr(){return _plaCur;};
	RSCPLAN GetBestPla(unsigned int lev);

private:
	SRBUF   _srBuf;
	int	   _nIter;
	double  _dist2pla; 
	int	   _nInliers;
	//std::vector<int> _inliers;
	std::vector<int> _perms;
	SRSEGMBUF _inBuf;
	int    _nIterMax;
	int    _inliersStop;
	//double _nVec[4];
	double* _poDist;
	double* _sgDist;
	double _avgSqDist;
	RSCPLAN _plaCur;
	RSCPLAN _plaBst;
	std::vector<RSCPLAN> _bestPlanes;


#ifdef AVGTIMER
  CPreciseTimer _ctrTimer;	//!< timer for ransac operation
  float			_timeCtr_s;	//!< variable to hold ransac. time in seconds
#endif

private:
	void RscBufAlloc();
	void RscBufFree();
	int CheckSrBufInput(SRBUF srBuf);
	int RansacIter(SRBUF srBuf, std::vector<int> *outliers, unsigned short* z, short* y, short* x, bool* isNaN, unsigned char segIdx);
	int SetDists(SRBUF srBuf, double nVec[4], unsigned short* z, short* y, short* x);
	int SetPlaCurNvec(Eigen::MatrixXd &A);
	int ResetPlane(RSCPLAN* plan);
	unsigned int time_seed();
};
