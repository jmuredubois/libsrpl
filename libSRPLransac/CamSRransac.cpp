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
	_nIterMax = 3000;
	_nIter = 0;
	_inliersStop=num-100;
	_poDist = NULL;
	_sgDist = NULL;
	_dist2pla = 100; // HARDCODED for now
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

	int num = srBuf.nCols*srBuf.nRows; 
    std::vector<int> outliers; outliers.erase(outliers.begin(),outliers.end());
    for(int k=0; k< num; k++)
    {
	  if(segmMap[k]==segIdx)  // if pixel is not segmented yet
	  {
		outliers.push_back(k); // add pixel to outliers
	  }
    }
    

  res+=ResetPlane(&_plaBst); // reset best plane
  if( (z!=NULL) && (y!=NULL) && (x!=NULL) && (isNaN!=NULL) &&  (segmMap!=NULL))
  {
	  for(int it=0; it < _nIterMax; it++)
	  {
		// TODO:  add for loop on number of iterations
		// TODO: check if mutex is necessary to prevent buffers from being cleared while ransac is running
		// res+=RansacIter(srBuf, z, y, x, isNaN, segmMap, segIdx);
		res += RansacIter(srBuf, outliers, z, y, x, isNaN, segmMap, segIdx);
	  	if((int)_plaCur.inliers.size() > num /10) // if 10% of points are in consensus set
		{
		  float perc = ((float) _plaCur.inliers.size()) / (float)num;
		  float percB= ((float) _plaBst.inliers.size()) / (float)num;
		  if(perc>percB)
		  {
			  // victory :-) , now, save best vector
			  res+=ResetPlane(&_plaBst); // reset best plane
			  memcpy( (void*) &(_plaBst.nVec), (const void*) &(_plaCur.nVec), 4*sizeof(double));
			  _plaBst.inliers = _plaCur.inliers;   //  STD::VECTOR DOES DEEP COPY
			  //_plaBst.outliers = _plaCur.outliers; //  STD::VECTOR DOES DEEP COPY
			  _plaBst.iter = it;
		  }
		}
	  }
  } // END OF if protecting from null buffers
  if(_plaBst.iter <0){return res;};
  res += SetDists(srBuf,_plaBst.nVec, z, y, x, isNaN, segmMap, segIdx);
  for(int k=0; k< num; k++)
  {
	  if(!( (isNaN[k]==0) && (segmMap[k]==segIdx) && (_poDist[k] < _dist2pla) && (_poDist[k] >= 0.0))) // CAREFUL, there is a not
	  {
		_plaBst.outliers.push_back(k); // add pixel to outliers
	  }
  }

  return res;
}
//! ONE RANSAC ITERATION.
int CamSRransac::RansacIter(SRBUF srBuf, std::vector<int> outliers, unsigned short* z, short* y, short* x, bool* isNaN, unsigned char* segmMap, unsigned char segIdx)
{
  int res = 0;
  const int nSeeds=9; // HARDCODED NUMBER OF SEEDS;
  int num = (int) outliers.size(); 
  if(num< nSeeds){ return -1;}; // if too few points, do nothing
  res+=ResetPlane(&_plaCur); // reset current plane
  int seeds[nSeeds];
  int idx,pix;
  //srand ( this->time_seed() ); // avoid seeding at each iter
  Eigen::Matrix<double, nSeeds, 4> A;  // trying to solve the overdetermined system   A*x = 0
  for(int k=0; k<nSeeds; k++)
  {
	idx = k;// DEBUG DEBUG DEBUG rand() / ( RAND_MAX / num + 1 );
	idx = rand() / ( RAND_MAX / num + 1 );
	pix = outliers[idx];
	seeds[k]=pix;
	A(k,0) = (double)x[pix];
	A(k,1) = (double)y[pix];
	A(k,2) = (double)z[pix];
	A(k,3) = 1.0;
  }
  res += this->SetPlaCurNvec(A);
  // prepare for distance computation
  double poDist = 0;
  double den = 1/sqrt((_plaCur.nVec[0]*_plaCur.nVec[0]) + (_plaCur.nVec[1]*_plaCur.nVec[1]) + (_plaCur.nVec[2]*_plaCur.nVec[2]) );
  int pos=outliers[0];
  // compute distance and compare to objective _dist2pla
  for(int i=0; i<num; i++)
	  {
		  pix = outliers[i];
	  poDist = abs(  (_plaCur.nVec[0]*(double)x[pix]) +
				     (_plaCur.nVec[1]*(double)y[pix]) +
				     (_plaCur.nVec[2]*(double)z[pix]) +
				     (_plaCur.nVec[3]             ) ) * den;
	_poDist[pix] = poDist;
	if( (isNaN[pix]==0) && (segmMap[pix]==segIdx) && (poDist < _dist2pla) && (poDist >= 0.0))
	{
		_plaCur.inliers.push_back(pix); // add pixel to inliers
	}
	/*else // TOO LONG FOR EACH ITERATION, DONE ONLY AFTER THE LOOP
	{
		_plaCur.outliers.push_back(outliers[i]); 
	}*/
  }
  _nIter+=1;
  return res;
}
//! ONE RANSAC ITERATION.
int CamSRransac::SetPlaCurNvec(Eigen::MatrixXd A)
{
  int res = 0;
  Eigen::SVD<Eigen::MatrixXd> svd(A); // perform SVD decomposition of A
  //Eigen::MatrixXd U = svd.matrixU(); //Eigen::VectorXd S = svd.singularValues();
  Eigen::MatrixXd V = svd.matrixV();  // only V matrix is needed
  Eigen::Vector3d nVec3;
  for(int k=0; k<3; k++)
  {
	  nVec3(k) = V(k,3); // last column of V (corresponding to smallest S) is singular vector
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
  return res;
}
//! Compute distances.
int CamSRransac::SetDists(SRBUF srBuf, double nVec[4], unsigned short* z, short* y, short* x, bool* isNaN, unsigned char* segmMap, unsigned char segIdx)
{
  int res = 0;
  int num = srBuf.nCols*srBuf.nRows;
  double sgDist;
  double den = 1/((nVec[0]*nVec[0]) + (nVec[1]*nVec[1]) + (nVec[2]*nVec[2]) );
  for(int i=0; i<num; i++)
  {
	  sgDist =     abs(  (nVec[0]*(double)x[i]) +
					     (nVec[1]*(double)y[i]) +
					     (nVec[2]*(double)z[i]) +
					     (nVec[3]             ) ) * den;
	 _poDist[i] = abs(sgDist);
	 _sgDist[i] = sgDist;
  }

	return res;
}

//! Reset a plane
int CamSRransac::ResetPlane(RSCPLAN* plan)
{
	int res=0;
	memset( (void*) &(plan->nVec), 0x0, 4*sizeof(double) );
	plan->inliers.erase(plan->inliers.begin(), plan->inliers.end());
	plan->outliers.erase(plan->outliers.begin(), plan->outliers.end());
	plan->iter = -1;
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
	// http://eigen.tuxfamily.org/dox/classEigen_1_1Quaternion.html#2e6246f7bf5ec16f738889a4f3e9c3b9
	Eigen::Vector3d nVec3;
	Eigen::Vector3d nVecZ; nVecZ.setZero(); nVecZ(2) = -1.0;
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
SRPLRSC_API double PLRSC_GetDistPla(SRPLRSC srPLRSC)
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

