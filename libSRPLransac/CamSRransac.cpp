/*
 * CamSRcoordTrf.cpp
 * Implementation of the coordinate transformation methods
 *
 * @author: James Mure-Dubois
 * @version: 2008.11.06
 */

#include "CamSRcoordTrf.h" //!< SR avg header file
#include "libSRPLcoordTrf.h" //!< library header file

/**
 *SR buffer coordTrf class constructor \n
 *  \n
 */
CamSRcoordTrf::CamSRcoordTrf(SRBUF srBuf)
{
	_srBuf.amp = NULL; _srBuf.pha = NULL;
	_srBuf.nCols = srBuf.nCols;
	_srBuf.nRows = srBuf.nRows;
	_srBuf.bufferSizeInBytes = srBuf.bufferSizeInBytes;
	_z = NULL;
	_y = NULL; 
	_x = NULL; 
	CtrBufAlloc();
}

/**
 *SR buffer coordTrf class destructor \n
 */
CamSRcoordTrf::~CamSRcoordTrf()
{
	CtrBufFree();
}

/**
 *SR coordTrf buffer allocator \n
 * many mallocs, don't forget to free
 */
void CamSRcoordTrf::CtrBufAlloc()
{
	if((_srBuf.nCols<1) || (_srBuf.nRows <1)){return;};
	unsigned int sizeBufUSh = (unsigned int) _srBuf.nCols*_srBuf.nRows*sizeof(unsigned short); // size of ushort buffer
	unsigned int sizeBufSho = (unsigned int) _srBuf.nCols*_srBuf.nRows*sizeof(short); // size of double buffer

	if(_srBuf.pha != NULL){free(_srBuf.pha);};
	   _srBuf.pha = (unsigned short*) malloc(sizeBufUSh); memset( (void*) _srBuf.pha, 0x00, sizeBufUSh);
	if(_srBuf.amp != NULL){free(_srBuf.amp);};
	   _srBuf.amp = (unsigned short*) malloc(sizeBufUSh); memset( (void*) _srBuf.amp, 0x00, sizeBufUSh);

	if(_z != NULL){free(_z);};
	   _z = (unsigned short*) malloc(sizeBufUSh); memset( (void*) _z, 0x00, sizeBufUSh);
    if(_y != NULL){free(_y);};
	   _y = (short*) malloc(sizeBufSho); memset( (void*) _y, 0x00, sizeBufSho);
	if(_x != NULL){free(_x);};
	   _x = (short*) malloc(sizeBufSho); memset( (void*) _x, 0x00, sizeBufSho);
   
	
	return;
}

/**
 *SR coordTrf buffer DEallocator \n
 * don't forget to free
 */
void CamSRcoordTrf::CtrBufFree()
{
	if(_srBuf.pha != NULL){free(_srBuf.pha); _srBuf.pha = NULL;};
	if(_srBuf.amp != NULL){free(_srBuf.amp); _srBuf.amp = NULL;};

    if(_z != NULL){free(_z); _z = NULL;};
    if(_y != NULL){free(_y); _y = NULL;};
	if(_x != NULL){free(_x); _x = NULL;};
	return;
}

//! Background learning initialization.
int CamSRcoordTrf::CoordTrf(SRBUF srBuf, SRCTR pa)
{
	int res = 0;
    if( (srBuf.nCols<1) || (srBuf.nRows<1) || (srBuf.nCols*srBuf.nRows*2*sizeof(unsigned short) != srBuf.bufferSizeInBytes) ){return -1;};

	if( (srBuf.nCols != _srBuf.nCols) || (srBuf.nRows != _srBuf.nRows) )
	{
		_srBuf.nCols = srBuf.nCols;
		_srBuf.nRows = srBuf.nRows;
		_srBuf.bufferSizeInBytes = srBuf.bufferSizeInBytes;
		CtrBufFree();
		CtrBufAlloc();
	};

	unsigned int sizeBufUSh = (unsigned int) _srBuf.nCols*_srBuf.nRows*sizeof(unsigned short); // size of ushort buffer
    //memcpy(_srBuf.amp, srBuf.amp, sizeBufUSh);  //EXPENSIVE
    //memcpy(_srBuf.pha, srBuf.pha, sizeBufUSh);
  if((srBuf.pha != NULL) && (srBuf.amp != NULL) && (_z!=NULL) && (_y!=NULL) && (_x!=NULL) )
  {
	  float xc = 0; float yc= 0; float x, y, z;
	  for(int r = 0; r< srBuf.nRows; r++)
	  {
		  for(int c = 0; c< srBuf.nCols; c++)
		  {
			  xc = -(c -pa.cX) * pa.pixDX;
			  yc = -(r -pa.cY) * pa.pixDY;

			  z = (sqrt(
				   (
				     (
					   ((float) srBuf.pha[r*srBuf.nCols+c]) / 65535.0f * pa.maxMM
					 )
					*(
					   ((float) srBuf.pha[r*srBuf.nCols+c]) / 65535.0f * pa.maxMM
					 )
					 * ((pa.f * pa.f) / ((pa.f * pa.f) + xc*xc + yc*yc)))));

			  x =(( xc * z) / pa.f);
			  y =(( yc * z) / pa.f);

			  _z[r*srBuf.nCols+c] = (unsigned short) z ;
			  _y[r*srBuf.nCols+c] = (short) y ;
			  _x[r*srBuf.nCols+c] = (short) x ;
		  }
	  }
  }

	return res;
}



////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
/////////////////////// API FUNCTIONS //////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

SRPLCTR_API int PLCTR_Open(SRPLCTR* srplCtr, SRBUF srBuf)
{
  //if(!srBuf)return -1;
  *srplCtr= new CamSRcoordTrf(srBuf);
  return 0;
}
SRPLCTR_API int PLCTR_Close(SRPLCTR srplCtr)
{
  if(!srplCtr)return -1;
  delete(srplCtr);
  return 0;
}
SRPLCTR_API int PLCTR_CoordTrf(SRPLCTR srplCtr, SRBUF srBuf, SRCTR srCtr)
{
  if(!srplCtr)return -1;
  return  srplCtr->CoordTrf(srBuf, srCtr);
}

SRPLCTR_API unsigned short* PLCTR_GetZ(SRPLCTR srplCtr)
{
  if(!srplCtr)return NULL;
  return srplCtr->GetZ();
}

SRPLCTR_API short* PLCTR_GetY(SRPLCTR srplCtr)
{
  if(!srplCtr)return NULL;
  return srplCtr->GetY();
}

SRPLCTR_API short* PLCTR_GetX(SRPLCTR srplCtr)
{
  if(!srplCtr)return NULL;
  return  srplCtr->GetX();
}