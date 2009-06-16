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
//  // fold first vector
//  n00(0) = n0[ 0]; n00(1) = n0[ 1]; n00(2) = n0[ 2]; n00(3) = n0[ 3];
//  n01(0) = n0[ 4]; n01(1) = n0[ 5]; n01(2) = n0[ 6]; n01(3) = n0[ 7];
//  n02(0) = n0[ 8]; n02(1) = n0[ 9]; n02(2) = n0[10]; n02(3) = n0[11];
//  // fold second vector
//  n10(0) = n1[ 0]; n10(1) = n1[ 1]; n10(2) = n1[ 2]; n10(3) = n0[ 3];
//  n11(0) = n1[ 4]; n11(1) = n1[ 5]; n11(2) = n1[ 6]; n11(3) = n0[ 7];
//  n12(0) = n1[ 8]; n12(1) = n1[ 9]; n12(2) = n1[10]; n12(3) = n0[11];
  n00=Map<Vector4d>(&(n0[0])); n01=Map<Vector4d>(&(n0[4])); n02=Map<Vector4d>(&(n0[8]));
  n10=Map<Vector4d>(&(n1[0])); n11=Map<Vector4d>(&(n1[4])); n12=Map<Vector4d>(&(n1[8]));
  Vector3d N00, N01, N02, N10, N11, N12;
  N00=Map<Vector3d>(&(n0[0])); N01=Map<Vector3d>(&(n0[4])); N02=Map<Vector3d>(&(n0[8]));
  N10=Map<Vector3d>(&(n1[0])); N11=Map<Vector3d>(&(n1[4])); N12=Map<Vector3d>(&(n1[8]));
  
  Matrix4d A0, A1, A2;
  A0 = hebAmat(N00, N10);
  A1 = hebAmat(N01, N11);
  A2 = hebAmat(N02, N12);
  
  Matrix4d B = ((A0.transpose())*A0) + ((A1.transpose())*A1) + ((A2.transpose())*A2);
  SVD<MatrixXd> svd(B); //!< perform SVD decomposition of A
  // std::cout << "B" << std::endl << B << std::endl;
  //Eigen::MatrixXd U = svd.matrixU(); //Eigen::
  Vector4d S = svd.singularValues();
  // std::cout << "S" << std::endl << S << std::endl;
  Matrix4d V = svd.matrixV();  // only V matrix is needed
  // std::cout << "V" << std::endl << V << std::endl;
  Vector4d quat = V.col(3); //!< last column of V (corresponding to smallest S) is singular vector
  //for(int k=0; k < 4; k++)
  //{ // trying qucikfix for -0 coeffs possibly spoiling rotations -> did no work :-/
	 // if( abs(quat(k)) < 1.0e-10)
	 // {
		//  quat(k) = 0.0;
	 // }
  //}
  // std::cout << "quat" << std::endl << quat << std::endl;
  //http://eigen.tuxfamily.org/dox/classEigen_1_1Quaternion.html#073d5be0431b9af3750e52c92b3fd754
  // Note the order of the arguments: the real w coefficient first, while internally the coefficients are stored in the following order: [x, y, z, w] 
  Quaterniond qrot = Quaterniond(quat(3), quat(0), quat(1), quat(2)); //!< construct quaternion from coeffs w, x, y, z
  Matrix3d mrot = qrot.toRotationMatrix();
  // std::cout << "mrot" << std::endl << mrot << std::endl;

  //! compute the crossings to find the translations (rotations should be done around origin)
  Vector3d xing0 = this->crossing( &n00, &n01, &n02);
  Vector3d xing1 = this->crossing( &n10, &n11, &n12);
  Vector3d zero3; zero3.setZero();
  xing0 =  zero3 - xing0; // xing1 =  zero3 - xing1; // apparently, ist xing0 that must be inverted
													 // I don't understand :'(
  Translation3d tran0 = Translation3d(xing0);
  Translation3d tran1 = Translation3d(xing1);

  /** Total transform : \n
   * - translate Pts1 to origin, \n
   * - rotate Pts1 to be parallel to Pts0, \n 
   * - translate Pts1 to Pts0.
   */
  Eigen::Transform3d trf1to0; trf1to0.setIdentity();
  Matrix4d resEig = trf1to0.matrix();
  // std::cout << "matId\n" << resEig << std::endl;
  trf1to0.pretranslate(xing1);
  // resEig = trf1to0.matrix(); std::cout << "matTrl1\n" << resEig << std::endl;
  trf1to0.prerotate(qrot);
  // resEig = trf1to0.matrix(); std::cout << "matTrl1Rot\n" << resEig << std::endl;
  trf1to0.pretranslate(xing0);
  // resEig = trf1to0.matrix(); std::cout << "matTrl0\n" << resEig << std::endl;

  resEig = trf1to0.matrix();

  int k=0; // cMatrix col major
  for(int col = 0; col < 4; col++)
  {
	  for(int row = 0; row < 4; row++)
	  {
		  mat[k] = resEig(row,col); //
		  k++;
	  }
  }

  // std::cout << "mat\n" << resEig << std::endl;

  
  return res;
}

Matrix4d CamSRalign::hebAmat(Vector3d n0, Vector3d n1)
{ /*  [ (n0+n1)° (n1-n0) ]
   *  [ (n0-n1)t      0  ]
   */
	Matrix4d res; res.setZero();
	res.corner<3,3>(TopLeft)  = heb0mat(n0+n1);
	res.corner<3,1>(TopRight) = (n1-n0);
	res.corner<1,3>(BottomLeft) = (n0.transpose()-n1.transpose()); 
	return res;
}

Matrix3d CamSRalign::heb0mat(Vector3d v)
{ /*  [   0 +v3 -v2 ]
   *  [ -v3   0 +v1 ]
   *  [ +v2 -v1   0 ]
   */
	Matrix3d res; res.setZero();
	res(0,1) =  v(2); res(0,2) = -v(1);
	res(1,0) = -v(2); res(1,2) =  v(0);
	res(2,0) =  v(1); res(2,1) = -v(0);
	return res;
}

Vector3d CamSRalign::crossing(Vector4d *n0, Vector4d *n1, Vector4d *n2)
{ /*  [ n00 n01 n02 ]    [ x ]   [ n03 ]
   *  [ n10 n11 n12 ] *  [ y ] = [ n13 ]  // find (x,y,z)
   *  [ n20 n21 n22 ]    [ z ]   [ n23 ]
   */
	Vector3d res; res.setZero();
	Matrix3d A; A.setZero();
	A.row(0) = n0->start<3>();
	A.row(1) = n1->start<3>();
	A.row(2) = n2->start<3>();
	Vector3d b; b.setZero();
	b(0) = n0->w(); b(1) = n1->w(); b(2) = n2->w();
	Matrix3d Ainv = A.inverse();
	res = Ainv * b;
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
