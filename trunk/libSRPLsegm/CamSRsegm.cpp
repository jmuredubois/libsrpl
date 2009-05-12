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
	try
	{
		ticpp::Document doc( fn );
		doc.LoadFile();

		_segParaList.erase(_segParaList.begin(),_segParaList.end());
	
		ticpp::Element* pThd = doc.FirstChildElement("PersPass")->FirstChildElement("Segmentation")->FirstChildElement("kStd");
		while(pThd != NULL)
		{
			float thresh=0;
			int val=0;
			unsigned char valChar;
			
			pThd->GetAttribute("thd", &thresh);
			pThd->GetAttribute("val", &val);
			valChar = (unsigned char) val;
			_segParaList.push_back(SrSegm(SrSegm::ST_KSTD, thresh, valChar));
			pThd = pThd->NextSiblingElement("kStd", false);
		}

	}
	catch( ticpp::Exception& ex )
	{
		std::cout << ex.what();
		return -1;
	}
	catch(...)
	{
		_segParaList.push_back(SrSegm(SrSegm::ST_KSTD, 03.0, 45)); // dummy values
		return -1;
	}
	//res += InitKernels();
	return res;
}

//---------------------------------------------------
/*!
	Segmentation method.. \n
	 - Compares phase difference to phase variance. \n
	 - Set as foreground iff above threshold. \n
*/
//! Segmentation method.
int CamSRsegm::Segment(SRBUF srBuf, NANBUF nanBuf, SRBUF srBG, NANBUF nanBG, SRVARBUF srVar)
{
	int res = 0;
	// MAKE SEGMENTATION
	//
	unsigned int sizeBufUCh = (unsigned int) _segmBuf.nCols*_segmBuf.nRows*sizeof(unsigned char); // size of double buffer
	unsigned char* segmBayB = _segmBuf.fg;
	memset( (void*) _segmBuf.bg, 0x00, sizeBufUCh);	// reset segmented image
    // list of segmentation setting from XML file
	std::list<SrSegm>::iterator begin=_segParaList.begin(),end=_segParaList.end(),itr;
    // treat the list of settings sequentially
    // -> ORDER MATTERS IN XML FILE !!!
  //for(itr=begin;itr!=end;itr++)
  //{
  //  SrSegm& thd=*itr; // read current segmentation setting
  //  switch(thd._type) // depending on segmentation setting type
  //  {
  //    case SrSegm::ST_ZCAM:		// zCam segmentation
  //    {
  //      unsigned short val=(unsigned short)(thd._thresh*1000.f);	// convert threshold value from meter to millimeter
  //      for(i=0;i<num;i++)				// for each pixel
  //      {
		//  if(_imgNaN[i])   continue;	// do not process NIL pixels
  //        if(_segmImgB[i]) continue;	// do not reprocess already segmented pixels
  //        if(_z[i]<val)					// when value is below threshold (distance FROM camera)
  //          _segmImgB[i]=thd._val;		// assign appropriate value to segmented pixel
  //      }
  //      break;							// break to the next segmentation setting
  //    }
  //    case SrSegm::TT_ZDIFF:	// zDiff segmentation
  //    {
  //      WORD val=(WORD)(thd._thresh*1000.f);	// convert threshold value from meter to millimeter
  //      for(i=0;i<num;i++)					// for each pixel
  //      {
		//  if(_imgNaN[i] || _imgNaNbg[i])   continue;	// do not process NIL pixels
  //        if(_segmImgB[i]) continue;		// do not reprocess already segmented pixels
  //        int diff=(int)_zBG[i]-(int)_z[i];	// calculate difference, should be positive (background is farther away from camera)
  //        if(diff<val)						// when value is below threshold
  //          _segmImgB[i]=thd._val;			// assign appropriate value to segmented pixel
  //      }
  //      break;							// break to the next segmentation setting
  //    }
	 // case SrSegm::TT_IDIFF:	// ampDiff segmentation
  //    {
  //      WORD val=(WORD)(thd._thresh);	// get threshold value
  //      for(i=0;i<num;i++)				// for each pixel
  //      {
		//  if(_imgNaN[i] || _imgNaNbg[i])   continue;	// do not process NIL pixels
  //        if(_segmImgB[i]) continue;					// do not reprocess already segmented pixels
  //        int diff=(int)intImg[i]-(int)_bgImgW[i+num];	// calculate difference, should be positive (background reflects less light)
  //        if(diff<val)					// when value is below threshold
  //          _segmImgB[i]=thd._val;		// assign appropriate value to segmented pixel
  //      }
  //      break;							// break to the next segmentation setting
  //    }
  //    case SrSegm::TT_SURFCAM:	// surfCam segmentation
  //    {
  //      WORD bin,val;
  //      float surf=0.f;
  //      for(bin=0;bin<_countof(_histS_c);bin++)		// go through surfCam histogram
  //      {
  //        surf+=_histS_c[bin];	// accumulate surface
  //        if(surf>=thd._thresh)	// until threshold (m^2) is reached
  //          break;
  //      }
		//// compute z value (val) corresponding to surface
  //        // bin=(0..255)*(256.f/3000.f)
  //        // convert bin to mm: bin*3000.f/256.f
  //      val=(WORD)(bin*3000.f/256.f);	// height in mm
  //      for(i=0;i<num;i++)				// for each pixel
  //      {
  //        if(_segmImgB[i]) continue;	// do not process NIL pixels
		//  if(_imgNaN[i])   continue;	// do not reprocess already segmented pixels
  //        if(_z[i]<val)					// if the pixel is closer to camera (i.e. higher) than threshold
  //          _segmImgB[i]=thd._val;		// assign appropriate value to segmented pixel
  //      }
		//break;							// break to the next segmentation setting
  //    }
	 // case SrSegm::TT_SURFDIFF:	// surfDiff segmentation
  //    {
  //      WORD bin,val;
  //      float surf=0.f;
  //      for(bin=0;bin<_countof(_histS_d);bin++)	// go through surfDiff histogram
  //      {
  //        surf+=_histS_d[bin];		// accumulate surface
  //        if(surf>=thd._thresh)		// until threshold (m^2) is reached
  //          break;
  //      }
		//// compute z value (val) corresponding to surface
  //        // bin=(0..255)*(256.f/3000.f)
  //        // convert bin to mm: bin*3000.f/256.f
  //      val=(WORD)(bin*3000.f/256.f);	// height in mm
  //      for(i=0;i<num;i++)				// for each pixel
  //      {
		//  if(_imgNaN[i] || _imgNaNbg[i])   continue;	// do not process NIL pixels
  //        if(_segmImgB[i]) continue;	// do not reprocess already segmented pixels
  //        if(_z[i]<val)					// if the pixel is closer to camera (i.e. higher) than threshold
  //          _segmImgB[i]=thd._val;		// assign appropriate value to segmented pixel
  //      }
		//break;							// break to the next segmentation setting
  //    }
	 // case SrSegm::TT_KSTD:		// segmentation based on ratio of difference to std
  //    {
  //    	/*
		//WORD* phaSR = (WORD*)SR_GetImage(_srCam,0); // Get SR current amplitude image
		//double kFloat;  // some variable used in loop
  //      float val=(float)(thd._thresh);	// convert threshold value from meter to millimeter
  //      for(i=0;i<num;i++)				// for each pixel
  //      {
		//  if(_imgNaN[i] || _imgNaNbg[i])   continue;	// do not process NIL pixels
  //        if(_segmImgB[i]) continue;	// do not reprocess already segmented pixels
		//  if(_bgVarD[i]<=0.0) continue; // do not segment pixels with 0 variance.
		//  kFloat =  sqrt( (( (double)phaSR[i] - _bgImgD[i]) * ( (double)phaSR[i] - _bgImgD[i]) ) / (_bgVarD[i]) );
  //        if(kFloat>val)					// when value is above threshold
  //          _segmImgB[i]=thd._val;		// assign appropriate value to segmented pixel
  //      }*/
  //      break;							// break to the next segmentation setting
  //    }
  //  }
  //}
  int num=srBuf.nCols*srBuf.nRows; // Get dimension of SR image
  memset(segmBayB,0x00,num*sizeof(unsigned char));	// reset segmented image
  unsigned short* phaSR = srBuf.pha; // Get SR current amplitude image
  unsigned short* ampSR = srBuf.amp; // Get SR current amplitude image
  bool* imgNaN = nanBuf.nanBool;
  bool* imgNaNbg = nanBG.nanBool;
  unsigned short* ampBG = srBG.amp; // Get SR BG amplitude image
  unsigned short* phaBG = srBG.pha; // Get SR BG amplitude image
  double* bgVarD = srVar.pha;
  double kFloat;  // some variable used in loop
  /*std::list<Thrhld>::iterator*/ begin=_segParaList.begin(),end=_segParaList.end(),itr;
    // treat the list of settings sequentially
    // -> ORDER MATTERS IN XML FILE !!!
  for(itr=begin;itr!=end;itr++)
  {
    SrSegm& thd=*itr; // read current segmentation setting
    switch(thd._type) // depending on segmentation setting type
    {
      case SrSegm::ST_KSTD:		// segmentation based on ratio of difference to std
      {
        float val=(float)(thd._thresh);	// read threshold value as float
        for(int i=0;i<num;i++)				// for each pixel
        {
		  if(imgNaN[i] || imgNaNbg[i])   continue;	// do not process NIL pixels
          if(segmBayB[i]) continue;	// do not reprocess already segmented pixels
		  if(bgVarD[i]<=0.0) continue; // do not segment pixels with 0 variance.
		  // kFloat =  sqrt( (( (double)phaSR[i] - _bgImgD[i]) * ( (double)phaSR[i] - _bgImgD[i]) ) / (_bgVarD[i]) );
		  kFloat =  sqrt( (( (double)phaSR[i] - (double)phaBG[i]) * ( (double)phaSR[i] - (double)phaBG[i]) ) / (bgVarD[i]) );
		  // // //kFloat =  sqrt( (( (double)ampSR[i] - _bgImgD[i+num]) * ( (double)ampSR[i] - _bgImgD[i+num]) ) / (_bgVarD[i+num]) ); // test amplitude segmentation NOT USED ANYMORE
          if(kFloat>val)					// when value is above threshold
            segmBayB[i]=thd._val;		// assign appropriate value to segmented pixel
        }
        break;							// break to the next segmentation setting
      }
	  case SrSegm::ST_ZCAM:		// zCam segmentation
      {
        break;							// break to the next segmentation setting
      }
      case SrSegm::ST_ZDIFF:	// zDiff segmentation
      {
        break;							// break to the next segmentation setting
      }
	  case SrSegm::ST_IDIFF:	// ampDiff segmentation
      {
        break;							// break to the next segmentation setting
      }
      case SrSegm::ST_SURFCAM:	// surfCam segmentation
      {
		break;							// break to the next segmentation setting
      }
	  case SrSegm::ST_SURFDIFF:	// surfDiff segmentation
      {
		break;							// break to the next segmentation setting
      }
	}
  }
	
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
SRPLSEGM_API int PLSEGM_Segment(SRPLSEGM srplSegm, SRBUF srBuf, NANBUF nanBuf, SRBUF srBG, NANBUF nanBG, SRVARBUF srVar)
{
  if(!srplSegm)return -1;
  return srplSegm->Segment(srBuf, nanBuf, srBG, nanBG, srVar);
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

