/*
 * CamSRalign.cpp
 * Implementation of the ALIGNMENT methods
 *
 * @author: James Mure-Dubois
 * @version: 2009.06.13
 */

#include "CamSRalign.h" //!< SR avg header file
#include "libSRPLalign.h" //!< library header file

int factorial (int m) {return (m > 1 ? m * factorial(m - 1) : 1);}

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

  // Declare transformation for storage
  Eigen::Transform3d trf1to0; trf1to0.setIdentity();
  Matrix4d resEig = trf1to0.matrix();


  ///** Total transform : \n
  // * - translate Pts1 to origin, \n
  // * - rotate Pts1 to be parallel to Pts0, \n 
  // * - translate Pts1 to Pts0.
  // */
  ////! compute the crossings to find the translations (rotations should be done around origin)
  //Vector3d xing0 = this->crossing( &n00, &n01, &n02);
  //Vector3d xing1 = this->crossing( &n10, &n11, &n12);
  //Vector3d zero3; zero3.setZero();
  //xing0 =  zero3 - xing0; // xing1 =  zero3 - xing1; // apparently, ist xing0 that must be inverted
		//											 // I don't understand :'(
  //Translation3d tran0 = Translation3d(xing0);
  //Translation3d tran1 = Translation3d(xing1);
  ////// std::cout << "matId\n" << resEig << std::endl;
  ////trf1to0.pretranslate(xing1);
  ////// resEig = trf1to0.matrix(); std::cout << "matTrl1\n" << resEig << std::endl;
  ////trf1to0.prerotate(qrot);
  ////// resEig = trf1to0.matrix(); std::cout << "matTrl1Rot\n" << resEig << std::endl;
  ////trf1to0.pretranslate(xing0);
  ////// resEig = trf1to0.matrix(); std::cout << "matTrl0\n" << resEig << std::endl;

  // following Hebert recipe for translation
  Vector3d tHeb; tHeb.setZero();
	  tHeb = this->tranHebert( &n00, &n01, &n02, &n10, &n11, &n12);
  trf1to0.rotate(qrot);
  trf1to0.pretranslate(tHeb);


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

//! ALIGN CALL.
int CamSRalign::alignNplans(double mat[16], int np, JMUPLAN3D* plans0, JMUPLAN3D* plans1)
{
  int res = 0;
  
  // Declare transformation for storage
  Eigen::Transform3d trf1to0; trf1to0.setIdentity();
  Matrix4d resEig = trf1to0.matrix();

  /****************************************/
  /* CALL WORKER FUNCTION *****************/
  trf1to0 = alignNplans(np, plans0, plans1);
  /****************************************/
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

//! ALIGN CALL.
Transform3d CamSRalign::alignNplans(int np, JMUPLAN3D* plans0, JMUPLAN3D* plans1)
{
  // Declare transformation for storage
  Transform3d trf1to0; trf1to0.setIdentity();
  if((np <3) || (plans0 ==NULL) || (plans1==NULL)){ return trf1to0;};
  Matrix4d B; B.setZero();
  Vector4d n0, n1;
  Vector3d N0, N1;
  Matrix4d Ai;
  for(int pl = 0; pl < np; pl++)
  {
	  n0.setZero(); n1.setZero();
	  N0.setZero(); N1.setZero();
	  n0 = Map<Vector4d>((plans0[pl]).n);
	  n1 = Map<Vector4d>((plans1[pl]).n);
	  N0 = Map<Vector3d>((plans0[pl]).n);
	  N1 = Map<Vector3d>((plans1[pl]).n);
	
	  Ai.setZero();
	  Ai = hebAmat(N0,N1);

	  B = B + ( (Ai.transpose())*Ai);
  }
  SVD<MatrixXd> svd(B); //!< perform SVD decomposition of A
  Vector4d S = svd.singularValues();
  Matrix4d V = svd.matrixV();  // only V matrix is needed
  Vector4d quat = V.col(3); //!< last column of V (corresponding to smallest S) is singular vector
 
  //http://eigen.tuxfamily.org/dox/classEigen_1_1Quaternion.html#073d5be0431b9af3750e52c92b3fd754
  // Note the order of the arguments: the real w coefficient first, while internally the coefficients are stored in the following order: [x, y, z, w] 
  Quaterniond qrot = Quaterniond(quat(3), quat(0), quat(1), quat(2)); //!< construct quaternion from coeffs w, x, y, z
  Matrix3d mrot = qrot.toRotationMatrix();
  // std::cout << "mrot" << std::endl << mrot << std::endl;

  // Declare matrix transformation for storage
  Matrix4d resEig = trf1to0.matrix();

  ///** Total transform : \n
  // * - translate Pts1 to origin, \n
  // * - rotate Pts1 to be parallel to Pts0, \n 
  // * - translate Pts1 to Pts0.
  // */
  ////! compute the crossings to find the translations (rotations should be done around origin)
  //Vector3d xing0 = this->crossing( &n00, &n01, &n02);
  //Vector3d xing1 = this->crossing( &n10, &n11, &n12);
  //Vector3d zero3; zero3.setZero();
  //xing0 =  zero3 - xing0; // xing1 =  zero3 - xing1; // apparently, ist xing0 that must be inverted
		//											 // I don't understand :'(
  //Translation3d tran0 = Translation3d(xing0);
  //Translation3d tran1 = Translation3d(xing1);
  ////// std::cout << "matId\n" << resEig << std::endl;
  ////trf1to0.pretranslate(xing1);
  ////// resEig = trf1to0.matrix(); std::cout << "matTrl1\n" << resEig << std::endl;
  ////trf1to0.prerotate(qrot);
  ////// resEig = trf1to0.matrix(); std::cout << "matTrl1Rot\n" << resEig << std::endl;
  ////trf1to0.pretranslate(xing0);
  ////// resEig = trf1to0.matrix(); std::cout << "matTrl0\n" << resEig << std::endl;

  // following Hebert recipe for translation
  Vector3d tHeb; tHeb.setZero();
	  tHeb = this->tranHebert(np, plans0, plans1);
  trf1to0.rotate(qrot);
  trf1to0.pretranslate(tHeb);
  resEig = trf1to0.matrix();

  return trf1to0;
}

Matrix4d CamSRalign::hebAmat(Vector3d &n0, Vector3d &n1)
{ /*  [ (n0+n1)° (n1-n0) ]
   *  [ (n0-n1)t      0  ]
   */
	Matrix4d res; res.setZero();
	Vector3d nSum = n0+n1;
	res.corner<3,3>(TopLeft)  = heb0mat(nSum);
	res.corner<3,1>(TopRight) = (n1-n0);
	res.corner<1,3>(BottomLeft) = (n0.transpose()-n1.transpose()); 
	return res;
}

Matrix3d CamSRalign::heb0mat(Vector3d &v)
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

Vector3d CamSRalign::tranHebert(Vector4d *n00, Vector4d *n01, Vector4d *n02,
		               Vector4d *n10, Vector4d *n11, Vector4d *n12)
{ // translation vector according to Hebert recipe
	Vector3d res; res.setZero();
	Vector3d D; D.setZero();
	D(0) = ( n10->w() - n00->w() );
	D(1) = ( n11->w() - n01->w() );
	D(2) = ( n12->w() - n02->w() );
	Matrix3d C; C.setZero();
	C.row(0) = n00->start<3>();
	C.row(1) = n01->start<3>();
	C.row(2) = n02->start<3>();
	// DEBUG trick to find transl even if points are in xy plan only
	Vector3d tr = C.col(2); 
	if (tr.norm()< 2e-8 )
	{
		C(0,2) = 300;
		C(1,2) = 300;
		C(2,2) = 300;
	}
	// END OF DEBUG trick to find transl even if points are in xy plan only
	res = (C.transpose() * C ).inverse() * C.transpose() * D;
	return res;
}

Vector3d CamSRalign::tranHebert(int np, JMUPLAN3D* plans0, JMUPLAN3D* plans1)
{ // translation vector according to Hebert recipe
	Vector3d res; res.setZero();
	if((np <3) || (plans0 ==NULL) || (plans1==NULL)){ return res;};
	VectorXd D; D = VectorXd::Zero(np);
	MatrixXd C; C = MatrixXd::Zero(np,3);
	for(int pl = 0; pl < np; pl++)
	{
	  D(pl) = plans1[pl].n[3] - plans0[pl].n[3];
	  C.row(pl) = Map<RowVector3d>(plans0[pl].n);
	}

	// DEBUG trick to find transl even if points are in xy plan only
	VectorXd tr = C.col(2); 
	if (tr.norm()< 2e-6 )
	{
		Vector2d tHeb2D; tHeb2D.setZero();
		tHeb2D = this->tranHebXY(np, plans0, plans1);
		res.start<2>() = tHeb2D;
		res.z()=0.0;
		return res;
	}
	// END OF DEBUG trick to find transl even if points are in xy plan only
	res = (C.transpose() * C ).inverse() * C.transpose() * D;
	return res;
}
Vector2d CamSRalign::tranHebXY(int np, JMUPLAN3D* plans0, JMUPLAN3D* plans1)
{ // translation vector according to Hebert recipe
	Vector2d res; res.setZero();
	if((np <2) || (plans0 ==NULL) || (plans1==NULL)){ return res;};
	VectorXd D; D = VectorXd::Zero(np);
	MatrixXd C; C = MatrixXd::Zero(np,2);
	for(int pl = 0; pl < np; pl++)
	{
	  D(pl) = plans1[pl].n[3] - plans0[pl].n[3];
	  C.row(pl) = Map<RowVector2d>(plans0[pl].n);
	}

	// END OF DEBUG trick to find transl even if points are in xy plan only
	res = (C.transpose() * C ).inverse() * C.transpose() * D;
	return res;
}

//! ALIGN CALL.
int CamSRalign::align2dNpoints(double mat[16], int npts, double* x0, double* y0, double* x1, double* y1)
{
  int res = 0;
  if((npts <3) ||(x0 ==NULL)||(x1==NULL)||(y0 ==NULL)||(y1==NULL)){ return -1;};
  if(npts>12) {return -1;};
  
  int np= factorial(npts) / ( factorial(2) * factorial((npts-2)) ) ;
  JMUPLAN3D *plans0, *plans1;
  plans0 = (JMUPLAN3D*) malloc(np*sizeof(JMUPLAN3D));
  plans1 = (JMUPLAN3D*) malloc(np*sizeof(JMUPLAN3D));
  memset(plans0, 0x0, np*sizeof(JMUPLAN3D));
  memset(plans1, 0x0, np*sizeof(JMUPLAN3D));

  int pl = 0; int pt1=0;
  Vector2d nVec;
  for(int pt = 0; pt < npts; pt++)
  {
	  pt1 = pt+1;
	  for(int ptC = pt1; ptC < npts; ptC++)
	  {
		  // add normal vector for target list
		  nVec.setZero();
		  nVec(0) = -y0[ptC] + y0[pt];
		  nVec(1) = +x0[ptC] - x0[pt];
		  nVec.normalize();
		  plans0[pl].n[0] = nVec(0);
		  plans0[pl].n[1] = nVec(1);
		  plans0[pl].n[2] = 0;
		  plans0[pl].n[3] = (nVec(0)*x0[pt]) + (nVec(1)*y0[pt]); // dist with origin is scalar prod of nVec with one point belonging to line

		  // add normal vector for source lisr
		  nVec.setZero();
		  nVec(0) = -y1[ptC] + y1[pt];
		  nVec(1) = +x1[ptC] - x1[pt];
		  nVec.normalize();
		  plans1[pl].n[0] = nVec(0);
		  plans1[pl].n[1] = nVec(1);
		  plans1[pl].n[2] = 0;
		  plans1[pl].n[3] = (nVec(0)*x1[pt]) + (nVec(1)*y1[pt]); // dist with origin is scalar prod of nVec with one point belonging to line
		  // increment plane list count
		  pl++;
	  }
  }
  res += alignNplans(mat, np, plans0, plans1);
  return res;
}
Transform3d CamSRalign::GetTrfminZalign(JMUPLAN3D* plan)
{
	// see libSRPLransac CamSRransac::SetProjZRotMat
	// http://eigen.tuxfamily.org/dox/classEigen_1_1Quaternion.html#2e6246f7bf5ec16f738889a4f3e9c3b9
	Eigen::Vector3d nVec3;
	Eigen::Vector3d nVecZ; nVecZ.setZero(); nVecZ(2) = -1.0; //-1 sign for z coordinate so that z increases as we move away from camera
	for(int k=0; k<3; k++)
	{
		  nVec3(k) = plan->n[k];
	}
	Eigen::Quaterniond qz;
	qz.setFromTwoVectors(nVec3, nVecZ);
	Eigen::Transform3d trfEig; trfEig.setIdentity();
	trfEig.rotate(qz);
	/*Eigen::Matrix4d matEig = trfEig.matrix();
	int k=0; // cMatrix col major
	for(int col = 0; col < 4; col++)
	{
	  for(int row = 0; row < 4; row++)
	  {
		  plan->matZ[k] = matEig(row,col); //
		  k++;
	  }
	}*/

	return trfEig;
}
int CamSRalign::align1plan2dNpoints(double mat[16], JMUPLAN3D* plan0, JMUPLAN3D* plan1, int npts, double* xyz0, double* xyz1)
{
	int res=0;
	if((plan0 ==NULL)||(plan1==NULL)){ return -1;}; //avoid bad params
	if((npts <3) ||(xyz0 ==NULL)||(xyz1==NULL)){ return -1;}; //avoid bad params
	if(npts>12) {return -1;}; // avoid too many points

	// Get rotation matrices
	Transform3d rotZ0 = GetTrfminZalign(plan0);
	Transform3d rotZ1 = GetTrfminZalign(plan1);
	Matrix4d rMatZ0 = rotZ0.matrix();
	Matrix4d rMatZ1 = rotZ1.matrix();

	// Transform xyz coords into Vectors
	std::vector<Vector4d> pts0 ; std::vector<Vector4d> pts1 ;
	std::vector<Vector4d> ptsR0; std::vector<Vector4d> ptsR1;
	pts0.clear() ; pts1.clear();
	ptsR0.clear(); ptsR1.clear();
	Vector4d curPt4, curPtRot4; Vector3d curPt3;

	// store all pts in Eigen vectors, and rotate them according to input plans
	for(int pt = 0; pt < npts; pt++)
	{   // FIRST, points in the target list
		curPt4.setZero(); curPt3.setZero();
		curPt3 = Map<Vector3d>(&(xyz0[pt*3]));
		curPt4.start<3>()  = curPt3;
		//curPt4.end<1>() = 1.0; // causes C2679 error, no conv. operator found -> probably my 1.0 should be encapsulated in Eigen matrix somehow
		curPt4.w() = 1.0; // quick trick to set last component of 4vec to a value
		pts0.push_back(curPt4); // add this point to the list
		curPtRot4.setZero();
		curPtRot4 = rMatZ0 * curPt4;
		ptsR0.push_back(curPtRot4);

		// NOW, points in the source list
		curPt4.setZero(); curPt3.setZero();
		curPt3 = Map<Vector3d>(&(xyz1[pt*3]));
		curPt4.start<3>()  = curPt3;
		curPt4.w() = 1.0; // quick trick to set last component of 4vec to a value
		pts1.push_back(curPt4); // add this point to the list
		curPtRot4.setZero();
		curPtRot4 = rMatZ1 * curPt4;
		ptsR1.push_back(curPtRot4);
	}

	//NOW, transform POINT PAIRS (inside each set) into 2D lines
	int np= factorial(npts) / ( factorial(2) * factorial((npts-2)) ) ;
	JMUPLAN3D *linesXY0, *linesXY1;
	linesXY0 = (JMUPLAN3D*) malloc(np*sizeof(JMUPLAN3D));
	linesXY1 = (JMUPLAN3D*) malloc(np*sizeof(JMUPLAN3D));
	memset(linesXY0, 0x0, np*sizeof(JMUPLAN3D));
	memset(linesXY1, 0x0, np*sizeof(JMUPLAN3D));

	int li = 0; int pt1=0;
	Vector2d nVec;
	for(int pt = 0; pt < npts; pt++)
	{
	  pt1 = pt+1;
		for(int ptC = pt1; ptC < npts; ptC++)
		{
		  // add normal vector for target list
		  nVec.setZero();
		  /*nVec(0) = -y0[ptC] + y0[pt];
		  nVec(1) = +x0[ptC] - x0[pt];*/
		  nVec(0) = - ptsR0[ptC].y() + ptsR0[pt].y();
		  nVec(1) = + ptsR0[ptC].x() - ptsR0[pt].x();
		  nVec.normalize();
		  linesXY0[li].n[0] = nVec(0);
		  linesXY0[li].n[1] = nVec(1);
		  linesXY0[li].n[2] = 0;
		  linesXY0[li].n[3] = (nVec(0)*ptsR0[pt].x()) + (nVec(1)*ptsR0[pt].y()); // dist with origin is scalar prod of nVec with one point belonging to line

		  // add normal vector for source lisr
		  nVec.setZero();
		  nVec(0) = - ptsR1[ptC].y() + ptsR1[pt].y();
		  nVec(1) = + ptsR1[ptC].x() - ptsR1[pt].x();
		  nVec.normalize();
		  linesXY1[li].n[0] = nVec(0);
		  linesXY1[li].n[1] = nVec(1);
		  linesXY1[li].n[2] = 0;
		  linesXY1[li].n[3] = (nVec(0)*ptsR1[pt].x()) + (nVec(1)*ptsR1[pt].y()); // dist with origin is scalar prod of nVec with one point belonging to line
		  // increment plane list count
		  li++;
		}
	}
	res += alignNplans(mat, np, linesXY0, linesXY1);

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

SRPLALI_API int PLALI_alignNplans(SRPLALI srPLALI, double mat[16], int np, JMUPLAN3D* plans0, JMUPLAN3D* plans1)
{
  if(!srPLALI)return -1;
  return  srPLALI->alignNplans( mat, np, plans0, plans1);
}

SRPLALI_API int PLALI_align2dNpoints(SRPLALI srPLALI, double mat[16], int npts, double* x0, double* y0, double* x1, double* y1)
{
  if(!srPLALI)return -1;
  return  srPLALI->align2dNpoints( mat, npts, x0, y0, x1, y1);
}