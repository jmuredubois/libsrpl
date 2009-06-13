/*
 * CamSRalign.cpp
 * Implementation of the ALIGNMENT methods
 *
 * @author: James Mure-Dubois
 * @version: 2009.06.13
 */

#include "CamSRalign.h" //!< SR avg header file
#include "libSRPLalign.h" //!< library header file

/**
 *SR buffer ransac class constructor \n
 *  \n
 */
CamSRalign::CamSRalign()
{	
}

/**
 *SR buffer align class destructor \n
 */
CamSRalign::~CamSRalign()
{
}


//! ALIGN CALL.
int CamSRalign::align3plans(double mat[16], double n0[12], double n1[12])
{
  int res = 0;
  Vector4d n00, n01, n02, n10, n11, n12;
  // fold first vector
  n00(0) = n0[ 0]; n00(1) = n0[ 1]; n00(2) = n0[ 2]; n00(3) = n0[ 3];
  n01(0) = n0[ 4]; n01(1) = n0[ 5]; n01(2) = n0[ 6]; n01(3) = n0[ 7];
  n02(0) = n0[ 8]; n02(1) = n0[ 9]; n02(2) = n0[10]; n02(3) = n0[11];
  // fold second vector
  n10(0) = n1[ 0]; n10(1) = n1[ 1]; n10(2) = n1[ 2]; n10(3) = n0[ 3];
  n11(0) = n1[ 4]; n11(1) = n1[ 5]; n11(2) = n1[ 6]; n11(3) = n0[ 7];
  n12(0) = n1[ 8]; n12(1) = n1[ 9]; n12(2) = n1[10]; n12(3) = n0[11];
  return res;
}

Matrix4d CamSRalign::hebAmat(Vector3d n0, Vector3d n1)
{
	Matrix4d res; res.setZero();
	return res;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
/////////////////////// API FUNCTIONS //////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

SRPLALI_API int PLALI_Open(SRPLALI* srPLALI)
{
  //if(!srBuf)return -1;
  *srPLALI= new CamSRalign();
  return 0;
}
SRPLALI_API int PLALI_Close(SRPLALI srPLALI)
{
  if(!srPLALI)return -1;
  delete(srPLALI);
  return 0;
}
SRPLALI_API int PLALI_align3plans(SRPLALI srPLALI, double mat[16], double n0[12], double n1[12])
{
  if(!srPLALI)return -1;
  return  srPLALI->align3plans( mat, n0, n1);
}
