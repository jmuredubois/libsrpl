/*
 * CamSRsegm.cpp
 * Implementation of the scattering compensation methods
 *
 * @author: James Mure-Dubois
 * @version: 2008.11.06
 */

#include "CamSRsegm.h" //!< SR avg header file
#include "libSRPLsegm.h" //!< library header file
#define LIMIT(val,min,max) (((val)<(min))?(min):(((val)>(max))? (max):(val)))

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
	
		//ticpp::Element* pThd = doc.FirstChildElement("libSRPL")->FirstChildElement("Segmentation")->FirstChildElement("kStd");
		//if(pThd == NULL)
		//{
		//	pThd = doc.FirstChildElement("PersPass")->FirstChildElement("Segmentation")->FirstChildElement("kStd");
		//}
		//while(pThd != NULL)
		//{
		//	float thresh=0;
		//	int val=0;
		//	unsigned char valChar;
		//	
		//	pThd->GetAttribute("thd", &thresh);
		//	pThd->GetAttribute("val", &val);
		//	valChar = (unsigned char) val;
		//	_segParaList.push_back(SrSegm(SrSegm::ST_KSTD, thresh, valChar));
		//	pThd = pThd->NextSiblingElement("kStd", false);
		//}
		ticpp::Element* pThd = NULL;
		try
		{
			pThd = doc.FirstChildElement("libSRPL")->FirstChildElement("Segmentation");
		}
		catch( ticpp::Exception& ex )
		{
			std::cout << ex.what();
			pThd = doc.FirstChildElement("PersPass")->FirstChildElement("Segmentation");
		}
		pThd = pThd->FirstChildElement();
		char type[64];
		while(pThd != NULL)
		{
			float thresh=0;
			int val=0;
			unsigned char valChar;
			pThd->GetAttribute("thd", &thresh);
			pThd->GetAttribute("val", &val);
			valChar = (unsigned char) val;

			memset(&type, 0x0, 64*sizeof(char));
			pThd->GetValue(&type);
			if( strcmp(type, "kStd") ==0)
			  {_segParaList.push_back(SrSegm(SrSegm::ST_KSTD, thresh, valChar));}
			else if( strcmp(type, "iDiff") ==0)
			  {_segParaList.push_back(SrSegm(SrSegm::ST_IDIFF, thresh, valChar));}
			else if( strcmp(type, "surfCam") ==0)
			  {_segParaList.push_back(SrSegm(SrSegm::ST_SURFCAM, thresh, valChar));}
			else if( strcmp(type, "surfDiff") ==0)
			  {_segParaList.push_back(SrSegm(SrSegm::ST_SURFDIFF, thresh, valChar));}
			else if( strcmp(type, "zCam") ==0)
			  {_segParaList.push_back(SrSegm(SrSegm::ST_ZCAM, thresh, valChar));}
			else if( strcmp(type, "zDiff") ==0)
			  {_segParaList.push_back(SrSegm(SrSegm::ST_ZDIFF, thresh, valChar));}
			else
			{
				// do not add if string was not recognized
			}
			pThd = pThd->NextSiblingElement(false);
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
	//unsigned char* segmImgB = _segmBuf.bg;
	unsigned char* segmBayB = _segmBuf.fg;
	int num=srBuf.nCols*srBuf.nRows; // Get dimension of SR image
	unsigned short* phaSR = srBuf.pha; // Get SR current amplitude image
	unsigned short* ampSR = srBuf.amp; // Get SR current amplitude image
	bool* imgNaN = nanBuf.nanBool;  if(!imgNaN){return -2;};
	bool* imgNaNbg = nanBG.nanBool; if(!imgNaNbg){return -3;};
	unsigned short* ampBG = srBG.amp; // Get SR BG amplitude image
	unsigned short* phaBG = srBG.pha; // Get SR BG amplitude image
// BAYESIAN ONLY SEGMENTATION
  memset(segmBayB,0x00,num*sizeof(unsigned char));	// reset segmented image
  double* bgVarD = srVar.pha;
  double kFloat;  // some variable used in loop
    // list of segmentation setting from XML file
	std::list<SrSegm>::iterator begin=_segParaList.begin(),end=_segParaList.end(),itr;
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
// END OF BAYESIAN ONLY SEGMENTATION
	return res;
}
//---------------------------------------------------
/*!
	Do segmentation according to loaded settings. \n
	- Segment() produces a new segmented image _segmBuf.bg . \n
	- Segment() produces new histograms for zCam, zDiff, surfCam, surfDiff. \n
	- Segment() uses boolean masks from nanBuf and nanBG to exclude NIL points from segmentation. \n
	- (NIL points are assigned value 0 in segmented image). \n
	The function uses: \n
	- a z image. \n
	- an amplitude image. \n
	- segmentation settings read from XML file (see LoadSettings())
  \sa PL_Segment(), FlagNaNs(), LearnBackground()
*/
//! Segmentation method.
int CamSRsegm::SegmentXYZ(SRBUF srBuf, NANBUF nanBuf, SRBUF srBG, NANBUF nanBG, SRVARBUF srVar, unsigned short* z, unsigned short* zBG)
{
	int res = 0;
	unsigned int sizeBufUCh = (unsigned int) _segmBuf.nCols*_segmBuf.nRows*sizeof(unsigned char); // size of unsigned char buffer
	unsigned char* segmImgB = _segmBuf.bg;
	int num=srBuf.nCols*srBuf.nRows; // Get dimension of SR image
	unsigned short* phaSR = srBuf.pha; // Get SR current amplitude image
	unsigned short* ampSR = srBuf.amp; // Get SR current amplitude image
	bool* imgNaN = nanBuf.nanBool;
	bool* imgNaNbg = nanBG.nanBool;
	unsigned short* ampBG = srBG.amp; // Get SR BG amplitude image
	unsigned short* phaBG = srBG.pha; // Get SR BG amplitude image
	//WORD  _histZ_c[256];
	//float _histS_c[256];
	//WORD  _histZ_d[256];
	//float _histS_d[256];
    // reset all histograms
	memset(_histZ_c,0,sizeof(_histZ_c));
	memset(_histS_c,0,sizeof(_histS_c));
	memset(_histZ_d,0,sizeof(_histZ_d));
	memset(_histS_d,0,sizeof(_histS_d));
	memset(_histI_d,0,sizeof(_histI_d));

	float sq; // variable for area calculations
	// information about current SR CAMERA
    // TODO : check if this is still valid for SR-3000
	float focal=0.008f,pixSzX=5.48e-005f,pixSzY=3.92e-005f;
	float surfMag=(pixSzX*pixSzY)/(focal*focal)*1E-6f;//1E-6= convert mm^2 to m^2
	//realArea=pixArea/(focal^2)*(distance^2)
	int i=0; // used to span all pixels


    // MAKE HISTOGRAMS (surf histograms needed for surf segmentation)
	unsigned char bin;
	//_histZ_c : distance    0mm to 3000mm in 256 bins
	//_histS_c : surface     0mm to 3000mm in 256 bins
	//_histZ_d : distance -100mm to 2900mm in 256 bins
	//_histS_d : surface  -100mm to 2900mm in 256 bins
	for(i=0;i<num;i++)
	{
	  if(!imgNaN[i]) // exclude NIL points from histograms
	  {
		sq=(float) z[i];//mm // calculate surface associated with current pixel
		sq*=sq*surfMag;//m^2;
		  // split 3000 mm into 256 bins
		bin=(unsigned char)LIMIT((z[i])*(256.f/3000.f),0.f,255.f);
		_histZ_c[bin]++;	// zCam hist
		_histS_c[bin]+=sq;	// surfCam hist

		if(!imgNaNbg[i]) // exclude background NIL points from difference histograms
		{
			  // split 3000 mm into 256 bins
			  // add offset of 100mm to see negative values
			bin=(unsigned char)LIMIT(((int)zBG[i]-(int)z[i]+100)*(256.f/3000.f),0.f,255.f);
			_histZ_d[bin]++;	// zDiff hist
			_histS_d[bin]+=sq;	// surfDiff hist

			  // split 25000 amplitude units into 256 bins
			  // add offset of 1000 amplitude units to see negative values
			bin = (unsigned char) LIMIT( (ampSR[i]-ampBG[i]+1000)*(256.f/25000.f),0.f,255.f);
			_histI_d[bin]++;	// ampDiff hist
		}
	  }
  }
	// MAKE SEGMENTATION
	//
// MISC SEGMENTATION
	memset( (void*) segmImgB, 0x00, sizeBufUCh);	// reset segmented image
    // list of segmentation setting from XML file
	std::list<SrSegm>::iterator begin=_segParaList.begin(),end=_segParaList.end(),itr;
    // treat the list of settings sequentially
    // -> ORDER MATTERS IN XML FILE !!!
  for(itr=begin;itr!=end;itr++)
  {
    SrSegm& thd=*itr; // read current segmentation setting
    switch(thd._type) // depending on segmentation setting type
    {
      case SrSegm::ST_ZCAM:		// zCam segmentation
      {
        unsigned short val=(unsigned short)(thd._thresh*1000.f);	// convert threshold value from meter to millimeter
        for(i=0;i<num;i++)				// for each pixel
        {
		  if(imgNaN[i])   continue;	// do not process NIL pixels
          if(segmImgB[i]) continue;	// do not reprocess already segmented pixels
          if(z[i]<val)					// when value is below threshold (distance FROM camera)
            segmImgB[i]=thd._val;		// assign appropriate value to segmented pixel
        }
        break;							// break to the next segmentation setting
      }
      case SrSegm::ST_ZDIFF:	// zDiff segmentation
      {
		int diff = 500;
        unsigned short val=(unsigned short)(thd._thresh*1000.f);	// convert threshold value from meter to millimeter
        for(i=0;i<num;i++)					// for each pixel
        {
		  if(imgNaN[i] || imgNaNbg[i])   continue;	// do not process NIL pixels
          if(segmImgB[i]) continue;		// do not reprocess already segmented pixels
          diff=(int)zBG[i]-(int)z[i];	// calculate difference, should be positive (background is farther away from camera)
          if(diff<val)						// when value is below threshold
            segmImgB[i]=thd._val;			// assign appropriate value to segmented pixel
        }
        break;							// break to the next segmentation setting
      }
	  case SrSegm::ST_IDIFF:	// ampDiff segmentation
      {
		int diff = 500;
        unsigned short val=(unsigned short)(thd._thresh);	// get threshold value
        for(i=0;i<num;i++)				// for each pixel
        {
		  if(imgNaN[i] || imgNaNbg[i])   continue;	// do not process NIL pixels
          if(segmImgB[i]) continue;					// do not reprocess already segmented pixels
          diff=(int)ampSR[i]-(int)ampBG[i];	// calculate difference, should be positive (background reflects less light)
          if(diff<val)					// when value is below threshold
            segmImgB[i]=thd._val;		// assign appropriate value to segmented pixel
        }
        break;							// break to the next segmentation setting
      }
      case SrSegm::ST_SURFCAM:	// surfCam segmentation
      {
        unsigned short bin,val;
        float surf=0.f;
        for(bin=0;bin<_countof(_histS_c);bin++)		// go through surfCam histogram
        {
          surf+=_histS_c[bin];	// accumulate surface
          if(surf>=thd._thresh)	// until threshold (m^2) is reached
            break;
        }
		// compute z value (val) corresponding to surface
          // bin=(0..255)*(256.f/3000.f)
          // convert bin to mm: bin*3000.f/256.f
        val=(unsigned short)(bin*3000.f/256.f);	// height in mm
        for(i=0;i<num;i++)				// for each pixel
        {
          if(segmImgB[i]) continue;	// do not process NIL pixels
		  if(imgNaN[i])   continue;	// do not reprocess already segmented pixels
          if(z[i]<val)					// if the pixel is closer to camera (i.e. higher) than threshold
            segmImgB[i]=thd._val;		// assign appropriate value to segmented pixel
        }
		break;							// break to the next segmentation setting
      }
	  case SrSegm::ST_SURFDIFF:	// surfDiff segmentation
      {
        unsigned short bin,val;
        float surf=0.f;
        for(bin=0;bin<_countof(_histS_d);bin++)	// go through surfDiff histogram
        {
          surf+=_histS_d[bin];		// accumulate surface
          if(surf>=thd._thresh)		// until threshold (m^2) is reached
            break;
        }
		// compute z value (val) corresponding to surface
          // bin=(0..255)*(256.f/3000.f)
          // convert bin to mm: bin*3000.f/256.f
        val=(unsigned short)(bin*3000.f/256.f);	// height in mm
        for(i=0;i<num;i++)				// for each pixel
        {
		  if(imgNaN[i] || imgNaNbg[i])   continue;	// do not process NIL pixels
          if(segmImgB[i]) continue;	// do not reprocess already segmented pixels
          if(z[i]<val)					// if the pixel is closer to camera (i.e. higher) than threshold
            segmImgB[i]=thd._val;		// assign appropriate value to segmented pixel
        }
		break;							// break to the next segmentation setting
      }
	  case SrSegm::ST_KSTD:		// segmentation based on ratio of difference to std
      {
      	/*
		WORD* phaSR = (WORD*)SR_GetImage(_srCam,0); // Get SR current amplitude image
		double kFloat;  // some variable used in loop
        float val=(float)(thd._thresh);	// convert threshold value from meter to millimeter
        for(i=0;i<num;i++)				// for each pixel
        {
		  if(_imgNaN[i] || _imgNaNbg[i])   continue;	// do not process NIL pixels
          if(_segmImgB[i]) continue;	// do not reprocess already segmented pixels
		  if(_bgVarD[i]<=0.0) continue; // do not segment pixels with 0 variance.
		  kFloat =  sqrt( (( (double)phaSR[i] - _bgImgD[i]) * ( (double)phaSR[i] - _bgImgD[i]) ) / (_bgVarD[i]) );
          if(kFloat>val)					// when value is above threshold
            _segmImgB[i]=thd._val;		// assign appropriate value to segmented pixel
        }*/
        break;							// break to the next segmentation setting
      }
    }
  }
//END OF MISC SEGMENTATION
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
SRPLSEGM_API int PLSEGM_SegmentXYZ(SRPLSEGM srplSegm, SRBUF srBuf, NANBUF nanBuf, SRBUF srBG, NANBUF nanBG, SRVARBUF srVar, unsigned short* z, unsigned short* zBG)
{
  if(!srplSegm)return -1;
  return srplSegm->SegmentXYZ(srBuf, nanBuf, srBG, nanBG, srVar, z, zBG);
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

