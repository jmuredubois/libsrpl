/*
 * CamSRsegm.cpp
 * Implementation of the scattering compensation methods
 *
 * @author: James Mure-Dubois
 * @version: 2008.11.06
 */

#include "CamSRsegm.h" //!< SR avg header file
#include "libSRPLsegm.h" //!< library header file

/**
 *SR buffer avg class constructor \n
 *  \n
 */
CamSRsegm::CamSRsegm(SRBUF srBuf)
{
	_segmBuf.fg = NULL; _segmBuf.bg = NULL;
	_segmBuf.nCols = srBuf.nCols;
	_segmBuf.nRows = srBuf.nRows;
	_segmBuf.bufferSizeInBytes = srBuf.nRows * srBuf.nRows * 2 * sizeof(unsigned char);
	SegmBufAlloc();
}

/**
 *SR buffer avg class destructor \n
 */
CamSRsegm::~CamSRsegm()
{
	SegmBufFree();
}

/**
 *SR avg buffer allocator \n
 * many mallocs, don't forget to free
 */
void CamSRsegm::SegmBufAlloc()
{
	if((_segmBuf.nCols<1) || (_segmBuf.nRows <1)){return;};
	//unsigned int sizeBufUSh = (unsigned int) _segmBuf.nCols*_segmBuf.nRows*sizeof(unsigned short); // size of ushort buffer
	//unsigned int sizeBufDbl = (unsigned int) _segmBuf.nCols*_segmBuf.nRows*sizeof(double); // size of double buffer
	unsigned int sizeBufUCh = (unsigned int) _segmBuf.nCols*_segmBuf.nRows*sizeof(unsigned char); // size of double buffer

	if(_segmBuf.bg != NULL){free(_segmBuf.bg);};
	   _segmBuf.bg = (unsigned char*) malloc(sizeBufUCh); memset( (void*) _segmBuf.bg, 0x00, sizeBufUCh);
	if(_segmBuf.fg != NULL){free(_segmBuf.fg);};
	   _segmBuf.fg = (unsigned char*) malloc(sizeBufUCh); memset( (void*) _segmBuf.fg, 0x00, sizeBufUCh);
	
	return;
}

/**
 *SR avg buffer DEallocator \n
 * don't forget to free
 */
void CamSRsegm::SegmBufFree()
{
	if(_segmBuf.bg != NULL){free(_segmBuf.bg); _segmBuf.bg = NULL;};
	if(_segmBuf.fg != NULL){free(_segmBuf.fg); _segmBuf.fg = NULL;};

	return;
}

/*!
	Load segmentation setting method
*/
//! Loading setting mehod
int CamSRsegm::LoadSegmSettings(const char* fn)
{
	int res = 0;

	return res;
}

//---------------------------------------------------
/*!
	Segmentation method.. \n
	 - Compares phase difference to phase variance. \n
	 - Set as foreground iff above threshold. \n
*/
//! Segmentation method.
int CamSRsegm::Segment(SRBUF srBuf, SRBUF srBG, SRVARBUF srVar)
{
	int res = 0;
	
  return res;
}

SRSEGMBUF   CamSRsegm::GetSegmBuf() //!< Returns the current segmentation buffer
{
	return _segmBuf;
}


////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
/////////////////////// API FUNCTIONS //////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

SRPLSEGM_API int PLSEGM_Open(SRPLSEGM* srplSegm, SRBUF srBuf)
{
  //if(!srBuf)return -1;
  *srplSegm= new CamSRsegm(srBuf);
  return 0;
}
SRPLSEGM_API int PLSEGM_Close(SRPLSEGM srplSegm)
{
  if(!srplSegm)return -1;
  delete(srplSegm);
  return 0;
}
SRPLSEGM_API int PLSEGM_LoadSegmSettings(SRPLSEGM srplSegm, const char* fn)
{
  if(!srplSegm)return -1;
  return  srplSegm->LoadSegmSettings(fn);
}
SRPLSEGM_API int PLSEGM_Segment(SRPLSEGM srplSegm, SRBUF srBuf, SRBUF srBG, SRVARBUF srVar)
{
  if(!srplSegm)return -1;
  return srplSegm->Segment(srBuf,srBG,srVar);
}

SRPLSEGM_API SRSEGMBUF PLSEGM_GetSegmBuf(SRPLSEGM srplSegm)
{
  if(!srplSegm)
  { 
	  SRSEGMBUF buf; buf.bg=NULL; buf.fg = NULL; 
	  buf.nCols = 0; buf.nRows=0; buf.bufferSizeInBytes=0;
	  return buf;
  }
  return srplSegm->GetSegmBuf();
}

