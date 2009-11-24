/*
 *  myConvolveHfloat.h
 * Header of the custom convolution method (for large scat kernels)
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


#ifndef IPLENG

//---------------------------------------------------
/*!
	Structure for 1D kernel \n
*/
//! 1D Kernel
typedef struct _myKernel1D
{
  int		_kSize;	//!< kernel size (number of coeffs)
  int		_kA;	//!< kernel anchor point
  float*	_coeffs;//!< kernel coefficients
}myKernel1D;

//---------------------------------------------------
/*!
	Structure for convolution images \n
*/
//! float image structure
typedef struct _myConvImg
{
  int		_imgSize;	//!< size of image
  int		_step;		//!< image width
  float*	_data;		//!< image data
}myConvImg;

  //! Create myKernel1D object
myKernel1D* CreateMyKernel1D(int size, int anchor, float* coeffs);
  //! Deallocate myKernel1D object
void    DeallocateMyKernel1D(myKernel1D* myKern1D);

  //! Create myConvImg object
myConvImg* AllocMyConvImg( int size, int step);
  //! Deallocate myConvImg object
void    DeallocateMyConvImg(myConvImg* myImg);

  //! convolution with horizontal 1D kernel
int myConvolveHfloat( myConvImg* myImg,		//!< input image
					  myKernel1D* myKern1D, //!< input kernel
					  myConvImg* imgOut);	//!< output image
  //! convolution with vertical 1D kernel
int myConvolveVfloat( myConvImg* myImg,		//!< input image
					  myKernel1D* myKern1D,	//!< input kernel
					  myConvImg* imgOut);	//!< output image
  //! 2D convolution separated in two 1D convolutions
int myConvolveHVfloat( myConvImg* myImg,	//!< input image
					  myKernel1D* myKernH,	//!< input kernel (horizontal)
					  myKernel1D* myKernV,	//!< input kernel (vertical)
					  myConvImg*  imgOut);	//!< output image
  //! 2D convolution NOT SEPARATED 
int myConvolve2Dfloat( myConvImg* myImg,	//!< input image
					  myKernel1D* myKernH,	//!< input kernel (horizontal)
					  myKernel1D* myKernV,	//!< input kernel (vertical)
					  myConvImg*  imgOut);	//!< output image

#endif   // !IPLENG