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
