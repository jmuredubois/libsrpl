/*
 * CamSRalign.h
 * Header of of the ALIGNMENT methods
 *
 * @author: James Mure-Dubois
 * @version: 2009.06.13
 */

#pragma once
#ifndef _CRT_SECURE_NO_DEPRECATE
  #define _CRT_SECURE_NO_DEPRECATE // used to remove MS C4996 warning for fopen
								 // fopen is used only in raw binary PSF read
#endif
#ifndef SAFE_FREE
	#define SAFE_FREE(p)       { if(p) { delete (p);     (p)=0; } }
#endif

#ifndef JMUPLAN3DHEADER
#define JMUPLAN3DHEADER
typedef struct plan3D{
    double n[4]; // for nx, ny, nz and d
} JMUPLAN3D;
#endif // 

#include <Eigen/Core>
#include <Eigen/SVD> // for SVD matrix decomp
#include <Eigen/LU>	 // for LU matrix decomp
#include <Eigen/Geometry>	 // for geometry transf (quaternions, etc)
#include <Eigen/StdVector> // Eigen needs special care with STL containers : 
                           // http://eigen.tuxfamily.org/dox/StlContainers.html
#include <string>
#include <list>		//for std::list
#include <vector>	//for std::vector
#include <math.h>
#include <ctime>	// for time
#include <complex> // for ANSI C99 complex numbers; WATCH OUT, this seems to be C++'s  complex<T>

#define TIXML_USE_TICPP
#include "ticpp.h" //Open source XML parser

using namespace Eigen;
// look at http://eigen.tuxfamily.org/dox/TutorialCore.html

/**
 * Camera align class \n
 * This class: \n
 * - computes transformation matrices to align plans \n
 */
class CamSRalign //!< Main class
{
public:
	//! constructor
    CamSRalign();
	//! destructor
	~CamSRalign();
	//! coordinate transform  method
	int align3plans(double mat[16], double n0[12], double n1[12]);
	int alignNplans(double mat[16], int np, JMUPLAN3D* plans0, JMUPLAN3D* plans1);
	int align2dNpoints(double mat[16], int npts, double* x0, double* y0, double* x1, double* y1);
	int align1plan2dNpoints(double mat[16], JMUPLAN3D* plan0, JMUPLAN3D* plan1, int npts, double* xyz0, double* xyz1);
	int WriteCamTrfMat4(std::string fn, Matrix4d &mat4, std::string comments);

private:
	Matrix4d hebAmat(Vector3d &n0, Vector3d &n1);
	Matrix3d heb0mat(Vector3d &v);
	Vector3d crossing(Vector4d *n0, Vector4d *n1, Vector4d *n2);
	Vector3d tranHebert(Vector4d *n00, Vector4d *n01, Vector4d *n02,
		               Vector4d *n10, Vector4d *n11, Vector4d *n12);
	Vector3d tranHebert(int np, JMUPLAN3D* plans0, JMUPLAN3D* plans1);
	Vector2d tranHebXY(int np, JMUPLAN3D* plans0, JMUPLAN3D* plans1);
	Transform3d getTrfminZalign(JMUPLAN3D* plan);
	Transform3d alignNplans(int np, JMUPLAN3D* plans0, JMUPLAN3D* plans1);
	Vector3d getRefPoint(JMUPLAN3D* plan);
	
#ifdef AVGTIMER
  CPreciseTimer _ctrTimer;	//!< timer for align operation
  float			_timeCtr_s;	//!< variable to hold align. time in seconds
#endif

};
