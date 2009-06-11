/*
 * CamSRransac.cpp
 * Implementation of the RANSAC methods
 *
 * @author: James Mure-Dubois
 * @version: 2009.05.13
 */

#include "CamSRransac.h" //!< SR avg header file
#include "libSRPLransac.h" //!< library header file

/**
 *SR buffer ransac class constructor \n
 *  \n
 */
CamSRransac::CamSRransac(SRBUF srBuf)
{
	_srBuf = srBuf;
	_inBuf.fg = NULL; _inBuf.bg = NULL;
	_inBuf.nCols = srBuf.nCols;
	_inBuf.nRows = srBuf.nRows;
	int num = srBuf.nCols*srBuf.nRows;
	_inBuf.bufferSizeInBytes = num*2*sizeof(unsigned char);
	_nIterMax = 2800;
	_nIter = 0;
	_inliersStop=num-100;
	_poDist = NULL;
	_sgDist = NULL;
	_dist2pla = 300; // HARDCODED for now
	RscBufAlloc(); // SHOULD ALWAYS BE LAST
	
}

/**
 *SR buffer ransac class destructor \n
 */
CamSRransac::~CamSRransac()
{
	RscBufFree();
}

/**
 *SR ransac buffer allocator \n
 * many mallocs, don't forget to free
 */
void CamSRransac::RscBufAlloc()
{
	if((_srBuf.nCols<1) || (_srBuf.nRows <1)){return;};
	//unsigned int sizeBufUSh = (unsigned int) _srBuf.nCols*_srBuf.nRows*sizeof(unsigned short); // size of ushort buffer
	//unsigned int sizeBufSho = (unsigned int) _srBuf.nCols*_srBuf.nRows*sizeof(short); // size of short buffer
	unsigned int sizeBufUCh = (unsigned int) _srBuf.nCols*_srBuf.nRows*sizeof(unsigned char); // size of uchar buffer
	unsigned int sizeBufDbl = (unsigned int) _srBuf.nCols*_srBuf.nRows*sizeof(double); // size of uchar buffer

	if(_inBuf.bg != NULL){free(_inBuf.bg);};
	_inBuf.bg = (unsigned char*) malloc(sizeBufUCh); memset( (void*) _inBuf.bg, 0x00, sizeBufUCh);
	if(_inBuf.fg != NULL){free(_inBuf.fg);};
	_inBuf.fg = (unsigned char*) malloc(sizeBufUCh); memset( (void*) _inBuf.fg, 0x00, sizeBufUCh);
	_inBuf.nCols = _srBuf.nCols;
	_inBuf.nRows = _srBuf.nRows;
	_inBuf.bufferSizeInBytes = _srBuf.nCols*_srBuf.nRows*2*sizeof(unsigned char);
	if(_poDist != NULL){free(_poDist);};
		_poDist = (double*) malloc(sizeBufDbl);memset(_poDist, 0x00, sizeBufDbl);
	if(_sgDist != NULL){free(_sgDist);};
		_sgDist = (double*) malloc(sizeBufDbl);memset(_sgDist, 0x00, sizeBufDbl);
	
	return;
}

/**
 *SR ransac buffer DEallocator \n
 * don't forget to free
 */
void CamSRransac::RscBufFree()
{
	if(_inBuf.bg != NULL){free(_inBuf.bg); _inBuf.bg = NULL;};
	if(_inBuf.fg != NULL){free(_inBuf.fg); _inBuf.fg = NULL;};
	if(_poDist != NULL){free(_poDist); _poDist = NULL;};
	if(_sgDist != NULL){free(_sgDist); _sgDist = NULL;};
    
	return;
}

//! RANSAC CALL.
int CamSRransac::ransac(SRBUF srBuf, unsigned short* z, short* y, short* x, bool* isNaN, unsigned char* segmMap, unsigned char segIdx)
{
	int res = 0;
    if( (srBuf.nCols<1) || (srBuf.nRows<1) ){return -1;};

	if( (srBuf.nCols != _inBuf.nCols) || (srBuf.nRows != _inBuf.nRows) )
	{
		_inBuf.nCols = srBuf.nCols;
		_inBuf.nRows = srBuf.nRows;
		_inBuf.bufferSizeInBytes = srBuf.nCols*srBuf.nRows*2*sizeof(unsigned char);
		RscBufFree();
		RscBufAlloc();
	};

  res+=ResetPlane(&_plaBst); // reset best plane
  int num = srBuf.nCols*srBuf.nRows; 
  if( (z!=NULL) && (y!=NULL) && (x!=NULL) && (isNaN!=NULL) &&  (segmMap!=NULL))
  {
	  for(int it=0; it < _nIterMax; it++)
	  {
		// TODO:  add for loop on number of iterations
		// TODO: check if mutex is necessary to prevent buffers from being cleared while ransac is running
		res+=RansacIter(srBuf, z, y, x, isNaN, segmMap, segIdx);
	  	if((int)_plaCur.inliers.size() > num /10) // if 10% of points are in consensus set
		{
		  float perc = ((float) _plaCur.inliers.size()) / (float)num;
		  float percB= ((float) _plaBst.inliers.size()) / (float)num;
		  if(perc>percB)
		  {
			  // victory :-) , now, save best vector
			  res+=ResetPlane(&_plaBst); // reset best plane
			  memcpy( (void*) &(_plaBst.nVec), (const void*) &(_plaCur.nVec), 4*sizeof(double));
			  _plaBst.inliers = _plaCur.inliers; // HOPEFULLY, STD::VECTOR DOES DEEP COPY
			  _plaBst.iter = it;
		  }
		}
	  }
  } // END OF if protecting from null buffers

	return res;
}
//! ONE RANSAC ITERATION.
int CamSRransac::RansacIter(SRBUF srBuf, unsigned short* z, short* y, short* x, bool* isNaN, unsigned char* segmMap, unsigned char segIdx)
{
  int res = 0;
  int num = srBuf.nCols*srBuf.nRows; 
  res+=ResetPlane(&_plaCur); // reset current plane

	  const int nSeeds=9; //9; // more than 3 would be nice but Eigen does not ssem to like that :-(
	  //res+=GenPerms(isNaN, segmMap);
	  int seeds[nSeeds];
	  int pix;
	  //srand ( this->time_seed() ); // avoid seedign at each iter
	  //
	  Eigen::Matrix<double, nSeeds, 4> A;
	  for(int k=0; k<nSeeds; k++)
	  {
		pix = k;// DEBUG DEBUG DEBUG rand() / ( RAND_MAX / num + 1 );
		pix = rand() / ( RAND_MAX / num + 1 );
		seeds[k]=pix;
		A(k,0) = (double)x[pix];
		A(k,1) = (double)y[pix];
		A(k,2) = (double)z[pix];
		A(k,3) = 1.0;
	  }
	  Eigen::SVD<Eigen::MatrixXd> svd(A);
	  //Eigen::MatrixXd U = svd.matrixU();
	  Eigen::MatrixXd V = svd.matrixV();
      //Eigen::VectorXd S = svd.singularValues();
	  Eigen::Vector3d nVec3;
	  for(int k=0; k<3; k++)
	  {
		  nVec3(k) = V(k,3); // last column of V (corresponding to smallest S) is singular vect
	  }
	  double d = 1/nVec3.norm();
	  nVec3.normalize();
	  Eigen::Vector4d nVec4; nVec4.setZero();
	  nVec4(3)=d; _plaCur.nVec[3] = d;
	  for(int k=0; k<3; k++)
	  {
		  nVec4(k) = nVec3(k);
		  _plaCur.nVec[k] = nVec3(k);
	  }
	  //std::cout << nVec4;
	  //nVec4.normalize();
	  // prepare for distance computation
	  double poDist = 0;
	  double den = 1/sqrt((_plaCur.nVec[0]*_plaCur.nVec[0]) + (_plaCur.nVec[1]*_plaCur.nVec[1]) + (_plaCur.nVec[2]*_plaCur.nVec[2]) );
	  // compute distance and compare to objective _dist2pla
	  for(int i=0; i<num; i++)
	  {
		  poDist = abs(  (_plaCur.nVec[0]*(double)x[i]) +
					     (_plaCur.nVec[1]*(double)y[i]) +
					     (_plaCur.nVec[2]*(double)z[i]) +
					     (_plaCur.nVec[3]             ) ) * den;
		_poDist[i] = poDist;
		if(poDist <0.0)
		{ // something is awfully wrong in hotfix for ML compatibility since it generates negative square distances
			std::cout << poDist;
		}
		if( (isNaN[i]==0) && (segmMap[i]==segIdx) && (poDist < _dist2pla) && (poDist >= 0.0))
		{
			_plaCur.inliers.push_back(i); // add pixel to inliers
		}
	  }
	  _nIter+=1;
	return res;
}
//! Compute distances.
int CamSRransac::SetDists(SRBUF srBuf, unsigned short* z, short* y, short* x, bool* isNaN, unsigned char* segmMap, unsigned char segIdx)
{
  int res = 0;
  int num = srBuf.nCols*srBuf.nRows;
  double sgDist;
  double den = 1/((_plaCur.nVec[0]*_plaCur.nVec[0]) + (_plaCur.nVec[1]*_plaCur.nVec[1]) + (_plaCur.nVec[2]*_plaCur.nVec[2]) );
  for(int i=0; i<num; i++)
  {
	  sgDist =     abs(  (_plaCur.nVec[0]*(double)x[i]) +
					     (_plaCur.nVec[1]*(double)y[i]) +
					     (_plaCur.nVec[2]*(double)z[i]) +
					     (_plaCur.nVec[3]             ) ) * den;
	 _poDist[i] = abs(sgDist);
	 _sgDist[i] = abs(sgDist);
  }

	return res;
}

//! Reset a plane
int CamSRransac::ResetPlane(RSCPLAN* plan)
{
	int res=0;
	memset( (void*) &(plan->nVec), 0x0, 4*sizeof(double) );
	plan->inliers.erase(plan->inliers.begin(), plan->inliers.end());
	plan->iter = -1;
	return res;
}

//! generate indexes permutations
int CamSRransac::GenPerms(bool* isNaN, unsigned char* segmMap)
{
	int res = 0;
    if( (_inBuf.nCols<1) || (_inBuf.nRows<1) ){return -1;};
	int num = _inBuf.nCols*_inBuf.nRows; 
	if( (isNaN!=NULL) &&  (segmMap!=NULL))
	{
	  srand ( this->time_seed() );
	  _perms.erase(_perms.begin(),_perms.end());
	  std::vector<int>::iterator it;
	  int k, N;
	  k=0;
	  _perms.push_back(0);
	  //_perms.insert(it+k,0); //inserting zero-th element is probably silly
	  for(int i = 1; i<num; i++)
	  {
		  N=(int)_perms.size();
		  it=_perms.begin();
		  k = rand() / ( RAND_MAX / N + 1 );
		  _perms.insert(it+k,i);
	  } // END OF loop on all points
	} // END OF if protecting from null buffers
	return res;
}

//! for better random numbers
// from: http://eternallyconfuzzled.com/arts/jsw_art_rand.aspx
unsigned int CamSRransac::time_seed()
{
	time_t now = std::time ( 0 );
  unsigned char *p = (unsigned char *)&now;
  unsigned int seed = 0;
  size_t i;
  for ( i = 0; i < sizeof now; i++ )
    seed = seed * ( UCHAR_MAX + 2U ) + p[i];
  return seed;
}

//! Sets a maximum number of iterations (HARDCODED max at 100000)
int CamSRransac::SetIterMax(int iterMax)
{
	int res=-1;
	if((iterMax > 0) && (iterMax < 100001))
	{
		_nIterMax = iterMax;
		res = _nIterMax;
	}
	return res;
}

//! Sets the maximum inliers distance (mm)
double CamSRransac::SetDistPla(double distPla)
{
	if(distPla > 0)
	{
		_dist2pla = distPla;
	}
	return _dist2pla;
}

//! Returns the matrix to rotate best plan axis on Z axis
double CamSRransac::GetProjZRotMat(double mat[9])
{
	// in ML, see trunk/srMatlab/utils/rotateSRPtsArbitrary.m
/* nVec = nVec / norm(nVec);
a = nVec(1); b = nVec(2); c = nVec(3) ;
Rx = [ 1 0 0 0; 0 (c / sqrt(b*b + c*c)) -(b / sqrt(b*b + c*c)) 0; 0 (b / sqrt(b*b + c*c)) (c / sqrt(b*b + c*c)) 0; 0 0 0 1];
v2 = Rx*[nVec;1];
Ry = [sqrt(b*b +c*c) 0 -a 0 ; 0 1 0 0; a 0 sqrt(b*b +c*c) 0; 0 0 0 1];
v3 = Ry*[v2];

clear a ans b c nVec v2 v3
% clear a ans b c nVec ptsZ v2 v3 Rx Ry
rotMat = Ry*Rx;

if exist('selXYZ')
    rotMat = Rlast*rotMat;
end
*/
	// http://eigen.tuxfamily.org/dox/classEigen_1_1Quaternion.html#2e6246f7bf5ec16f738889a4f3e9c3b9
	Eigen::Vector3d nVec3;
	Eigen::Vector3d nVecZ; nVecZ.setZero(); nVecZ(2) = 1.0;
	for(int k=0; k<3; k++)
	{
		  nVec3(k) = _plaBst.nVec[k];
	}
	Eigen::Quaterniond qz;
	qz.setFromTwoVectors(nVec3, nVecZ);
	Eigen::Matrix3d matE = qz.toRotationMatrix();
	for(int k=0; k<9; k++)
	{
		mat[k] = matE(k);
	}

	return 0;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
/////////////////////// API FUNCTIONS //////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

SRPLRSC_API int PLRSC_Open(SRPLRSC* srPLRSC, SRBUF srBuf)
{
  //if(!srBuf)return -1;
  *srPLRSC= new CamSRransac(srBuf);
  return 0;
}
SRPLRSC_API int PLRSC_Close(SRPLRSC srPLRSC)
{
  if(!srPLRSC)return -1;
  delete(srPLRSC);
  return 0;
}
SRPLRSC_API int PLRSC_ransac(SRPLRSC srPLRSC, SRBUF srBuf, unsigned short* z, short* y, short* x, bool* isNaN, unsigned char* segmMap, unsigned char segIdx)
{
  if(!srPLRSC)return -1;
  return  srPLRSC->ransac(srBuf, z, y, x, isNaN, segmMap, segIdx);
}
SRPLRSC_API int PLRSC_GetIter(SRPLRSC srPLRSC)
{
  if(!srPLRSC)return -1;
  return srPLRSC->GetIter();
}
SRPLRSC_API int PLRSC_GetIterMax(SRPLRSC srPLRSC)
{
  if(!srPLRSC)return -1;
  return srPLRSC->GetIterMax();
}
SRPLRSC_API int PLRSC_SetIterMax(SRPLRSC srPLRSC, int iterMax)
{
  if(!srPLRSC)return -1;
  return srPLRSC->SetIterMax(iterMax);
}
SRPLRSC_API RSCPLAN PLRSC_GetPlaBest(SRPLRSC srPLRSC)
{
  if(!srPLRSC)
  {
	RSCPLAN plan0;
	for(int i=0; i<4; i++) {plan0.nVec[i]=0.0;} ;
	return plan0;
  }
  return srPLRSC->GetPlaBest();
}
SRPLRSC_API RSCPLAN PLRSC_GetPlaCurr(SRPLRSC srPLRSC)
{
  if(!srPLRSC)
  {
	RSCPLAN plan0;
	for(int i=0; i<4; i++) {plan0.nVec[i]=0.0;} ;
	return plan0;
  }
  return srPLRSC->GetPlaCurr();
}
SRPLRSC_API double PLRSC_GetDispPla(SRPLRSC srPLRSC)
{
  if(!srPLRSC)return -1;
  return srPLRSC->GetDistPla();
}
SRPLRSC_API double PLRSC_SetDistPla(SRPLRSC srPLRSC, double distPla)
{
  if(!srPLRSC)return -1;
  return srPLRSC->SetDistPla(distPla);
}

SRPLRSC_API double PLRSC_GetProjZRotMat(SRPLRSC srPLRSC, double mat[9])
{
	if(!srPLRSC)return -1.0;
	return srPLRSC->GetProjZRotMat(mat);
}

