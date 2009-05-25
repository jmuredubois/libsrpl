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
	_sqDist = NULL;
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
	if(_sqDist != NULL){free(_sqDist);};
		_sqDist = (double*) malloc(sizeBufDbl);memset(_sqDist, 0x00, sizeBufDbl);
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
	if(_sqDist != NULL){free(_sqDist); _sqDist = NULL;};
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
	  	if(_plaCur.inliers.size() > num /10) // if 10% of points are in consensus set
		{
		  float perc = ((float) _plaCur.inliers.size()) / (float)num;
		  float percB= ((float) _plaBst.inliers.size()) / (float)num;
		  if(perc>percB)
		  {
			  // victory :-) , now, save best vector
			  res+=ResetPlane(&_plaBst); // reset best plane
			  memcpy( (void*) &(_plaBst.nVec), (const void*) &(_plaCur.nVec), 4*sizeof(double));
			  _plaBst.inliers = _plaCur.inliers; // HOPEFULLY, STD::VECTOR DOES DEEP COPY
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
	  srand ( this->time_seed() );
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
	  Eigen::Matrix<double, nSeeds, 1> B; B.setOnes();
	  Eigen::SVD<Eigen::MatrixXd> svd(A);
	  Eigen::MatrixXd U = svd.matrixU();
	  Eigen::MatrixXd V = svd.matrixV();
      Eigen::VectorXd S = svd.singularValues();
	  Eigen::Vector3d nVec3;
	  //A.svd().solve(B,&nVec3); //eigen says invalid product for SVD decomp for more than 3 pts
	  //if((A.lu().solve(B,&nVec3))==false){continue;}; // LU compiles but returns false for more than 3 pts
	  for(int k=0; k<3; k++)
	  {
		  nVec3(k) = V(k,3);
	  }
	  //double d = -1/nVec3.norm();
	  //nVec3.normalize();
	  //Eigen::Vector4d nVec4; nVec4.setZero();
	  //nVec4(3)=d; _plaCur.nVec[3] = d;
	  //nVec4(3)=-1.0; _plaCur.nVec[3] = -1.0;
	  for(int k=0; k<4; k++)
	  {
		  //nVec4(k) = nVec3(k);
		  _plaCur.nVec[k] = V(k,3)/V(3,3);
	  }
	  //std::cout << nVec4;
	  //nVec4.normalize();
	  //std::cout << nVec4;
	  // prepare for squared distance computation
	  double sqDist = 0; double dist2plaSQ = _dist2pla*_dist2pla;
	  double den = 1/((_plaCur.nVec[0]*_plaCur.nVec[0]) + (_plaCur.nVec[1]*_plaCur.nVec[1]) + (_plaCur.nVec[2]*_plaCur.nVec[2]) );
	  //double denE = nVec4.norm();
	  // compute squared distance and compare to objective _dist2pla
	  for(int i=0; i<num; i++)
	  {
		sqDist =  ( (_plaCur.nVec[0]*(double)x[i])*(_plaCur.nVec[0]*(double)x[i]) +
					(_plaCur.nVec[1]*(double)y[i])*(_plaCur.nVec[1]*(double)y[i]) +
					(_plaCur.nVec[2]*(double)z[i])*(_plaCur.nVec[2]*(double)z[i]) -
					(_plaCur.nVec[3]             )*(_plaCur.nVec[3]             ) ) * den;
		/*sqDist =  ( (nVec4(0)*(double)x[i])*(nVec4(0)*(double)x[i]) +
					(nVec4(1)*(double)y[i])*(nVec4(1)*(double)y[i]) +
					(nVec4(2)*(double)z[i])*(nVec4(2)*(double)z[i]) +
					(nVec4(3)             )*(nVec4(3)             ) ) * denE;*/
		_sqDist[i] = sqDist;
		if( (isNaN[i]==0) && (segmMap[i]==0) && (sqDist < dist2plaSQ) )
		{
			_plaCur.inliers.push_back(i); // add pixel to inliers
		}
	  }
	  _nIter+=1;
	return res;
}
//! Compute squared distances.
int CamSRransac::SquaredDist(SRBUF srBuf, unsigned short* z, short* y, short* x, bool* isNaN, unsigned char* segmMap, unsigned char segIdx)
{
  int res = 0;
  int num = srBuf.nCols*srBuf.nRows;
  double den = 1/((_plaCur.nVec[0]*_plaCur.nVec[0]) + (_plaCur.nVec[1]*_plaCur.nVec[1]) + (_plaCur.nVec[2]*_plaCur.nVec[2]) );
  for(int i=0; i<num; i++)
  {
	  _sqDist[i] =  ( (_plaCur.nVec[0]*(double)x[i])*(_plaCur.nVec[0]*(double)x[i]) +
					  (_plaCur.nVec[1]*(double)y[i])*(_plaCur.nVec[1]*(double)y[i]) +
					  (_plaCur.nVec[2]*(double)z[i])*(_plaCur.nVec[2]*(double)z[i]) +
					  ((_plaCur.nVec[3]            )*(_plaCur.nVec[3])            ) ) * den;
  }

	return res;
}

//! Reset a plane
int CamSRransac::ResetPlane(RSCPLAN* plan)
{
	int res=0;
	memset( (void*) &(plan->nVec), 0x0, 4*sizeof(double) );
	plan->inliers.erase(plan->inliers.begin(), plan->inliers.end());
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
		  N=_perms.size();
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
  if(!srPLRSC)return NULL;
  return srPLRSC->GetIter();
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