/*
 * imgPadder.h
 * Header of the image padding method (for large scat kernels)
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

#include "fftw3.h"  // included for type fftw_complex
#include <math.h>	// included for type complex
#include <complex>	// included for type complex


#ifndef MYIMGSZ
typedef struct IMGSIZE{ 
  long cx; 
  long cy; 
} IMGSIZE; 
#define MYIMGSZ
#endif

class CImgPadder
{
public:
	CImgPadder(){};	// constructor
	~CImgPadder(){};	// destructor

	//int imgPad(double* dbIn, long dbLen, double padFact);
	//int imgPad(fftw_complex* cxIn, long cxLen, double padFact);
	//int imgPad(fftw_complex* cxIn, long cxLin, long inCx, long inCy, fftw_complex* cxOut, long cxLout, long outCx, long outCy);
	int imgPad(fftw_complex* cxIn, long cxLin, IMGSIZE szIn, fftw_complex* cxOut, long cxLout, IMGSIZE szOut);
	int imgUnPad(fftw_complex* cxIn, long cxLin, IMGSIZE szIn, fftw_complex* cxOut, long cxLout, IMGSIZE szOut);
	int imgUnPad(double* dbIn, long cxLin, IMGSIZE szIn, double* dbOut, long cxLout, IMGSIZE szOut);

private:
	int gaussRamp(double* wgtIn, long wgLen, int nRamp);
	int linGaussRamp(double* wgtIn, long wgLen, int nRamp);
	int zeroRamp(double* wgtIn, long wgLen, int nRamp);
};