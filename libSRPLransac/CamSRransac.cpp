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
	_inBuf.fg = NULL; _inBuf.bg = NULL;
	_inBuf.nCols = srBuf.nCols;
	_inBuf.nRows = srBuf.nRows;
	_inBuf.bufferSizeInBytes = srBuf.nRows*srBuf.nRows*2*sizeof(unsigned char);
	RscBufAlloc();
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
	unsigned int sizeBufUSh = (unsigned int) _srBuf.nCols*_srBuf.nRows*sizeof(unsigned short); // size of ushort buffer
	unsigned int sizeBufSho = (unsigned int) _srBuf.nCols*_srBuf.nRows*sizeof(short); // size of double buffer
	unsigned int sizeBufUCh = (unsigned int) _srBuf.nCols*_srBuf.nRows*sizeof(unsigned char); // size of uchar buffer

	if(_inBuf.bg != NULL){free(_inBuf.bg);};
	_inBuf.bg = (unsigned char*) malloc(sizeBufUCh); memset( (void*) _inBuf.bg, 0x00, sizeBufUCh);
	if(_inBuf.fg != NULL){free(_inBuf.fg);};
	_inBuf.fg = (unsigned char*) malloc(sizeBufUCh); memset( (void*) _inBuf.fg, 0x00, sizeBufUCh);
	_inBuf.nCols = _srBuf.nCols;
	_inBuf.nRows = _srBuf.nRows;
	_inBuf.bufferSizeInBytes = _srBuf.nCols*_srBuf.nRows*2*sizeof(unsigned char);
	
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
  if( (z!=NULL) && (y!=NULL) && (x!=NULL) && (isNaN!=NULL) &&  (segmMap!=NULL))
  {
	  res+=GenPerms(isNaN, segmMap);
  } // END OF if protecting from null buffers

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
