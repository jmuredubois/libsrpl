/*
 * CamSRsegm.h
 * Header of the SR buffer segmentation method
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

#define TIXML_USE_TICPP
#include "ticpp.h" //Open source XML parser
#include <list>		//for std::list
#include <math.h>
#include <complex> // for ANSI C99 complex numbers; WATCH OUT, this seems to be C++'s  complex<T>
#include "srBuf.h"
#include "srSegmBuf.h"
#include "libSRPLsegm.h"

//! class for kernels used in scattering compensation
class SrSegm
{
public:
  enum Type{ST_KSTD, ST_AMPSTD, ST_ZCAM, ST_ZDIFF, ST_IDIFF, ST_SURFCAM, ST_SURFDIFF}; //!< enum for type of kernel
  SrSegm();	//!< constructor
  SrSegm(Type type, float thresh, unsigned char val):
						_type(type),
						_thresh(thresh), _val(val){}
  Type  _type;			//!< type of kernel (preferably ST_KSTD)
  float   _thresh;			//!< threshold for segmentation
  unsigned char _val;	//!< value to paint in segmentation map
};

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
	int Segment(SRBUF srBuf, NANBUF nanBuf, SRBUF srBG, NANBUF nanBG, SRVARBUF srVar);
	int SegmentXYZ(SRBUF srBuf, NANBUF nanBuf, SRBUF srBG, NANBUF nanBG, SRVARBUF srVar, unsigned short* z, unsigned short* zBG);
	SRSEGMBUF   GetSegmBuf();	//!< Returns the current average buffer
	

private:
	SRSEGMBUF   _segmBuf;
	SRSEGMBUF   _segmNonBayesBuf;
	std::list<SrSegm> _segParaList;	//!< segmentation parameter list
	unsigned short	_histZ_c[256];	//!< histogram of zCam distances
	float			_histS_c[256];	//!< histogram of surfCam surfaces
	unsigned short	_histZ_d[256];	//!< histogram of zDiff distances
	float			_histS_d[256];	//! histogram of surfDiff surfaces
	unsigned short	_histI_d[256];	//!< histogram of intensity differences

#ifdef SEGMTIMER
  CPreciseTimer _segmTimer;	    //!< timer for segmentation operation
  float			_timeSegm_s;	//!< variable to hold segmentation time in seconds
#endif

private:
	void SegmBufAlloc();
	void SegmBufFree();
  
};
