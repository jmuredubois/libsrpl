/*
 * CamSRavg.cpp
 * Implementation of the scattering compensation methods
 *
 * @author: James Mure-Dubois
 * @version: 2008.11.06
 */

#include "CamSRavg.h" //!< SR avg header file
#include "libSRPLavg.h" //!< library header file

/**
 *SR buffer avg class constructor \n
 *  \n
 */
CamSRavg::CamSRavg(SRBUF srBuf)
{
	_avgCnt = 0;
	_avgAlpha = 0;
	_avgBuf.amp = NULL; _avgBuf.pha = NULL;
	_avgBuf.nCols = srBuf.nCols;
	_avgBuf.nRows = srBuf.nRows;
	_avgBuf.bufferSizeInBytes = srBuf.bufferSizeInBytes;
	_avgImgPha = NULL; _avgImgAmp = NULL;						
	_avgVarPha = NULL; _avgVarAmp = NULL;							
	_avgSquPha = NULL; _avgSquAmp = NULL;						
	_avgPhiPha = NULL; _avgPhiAmp = NULL;
	AvgBufAlloc();
}

/**
 *SR buffer avg class destructor \n
 */
CamSRavg::~CamSRavg()
{
	AvgBufFree();
}

/**
 *SR avg buffer allocator \n
 * many mallocs, don't forget to free
 */
void CamSRavg::AvgBufAlloc()
{
	if((_avgBuf.nCols<1) || (_avgBuf.nRows <1)){return;};
	unsigned int sizeBufUSh = (unsigned int) _avgBuf.nCols*_avgBuf.nRows*sizeof(unsigned short); // size of ushort buffer
	unsigned int sizeBufDbl = (unsigned int) _avgBuf.nCols*_avgBuf.nRows*sizeof(double); // size of double buffer

	if(_avgBuf.pha != NULL){free(_avgBuf.pha);};
	   _avgBuf.pha = (unsigned short*) malloc(sizeBufUSh); memset( (void*) _avgBuf.pha, 0x00, sizeBufUSh);
	if(_avgBuf.amp != NULL){free(_avgBuf.amp);};
	   _avgBuf.amp = (unsigned short*) malloc(sizeBufUSh); memset( (void*) _avgBuf.amp, 0x00, sizeBufUSh);

	if(_avgImgPha != NULL){free(_avgImgPha);};
	   _avgImgPha = (double*) malloc(sizeBufDbl); memset( (void*) _avgImgPha, 0x00, sizeBufDbl);
    if(_avgImgAmp != NULL){free(_avgImgAmp);};
	   _avgImgAmp = (double*) malloc(sizeBufDbl); memset( (void*) _avgImgAmp, 0x00, sizeBufDbl);
	if(_avgVarPha != NULL){free(_avgVarPha);};
	   _avgVarPha = (double*) malloc(sizeBufDbl); memset( (void*) _avgVarPha, 0x00, sizeBufDbl);
    if(_avgVarAmp != NULL){free(_avgVarAmp);};
	   _avgVarAmp = (double*) malloc(sizeBufDbl); memset( (void*) _avgVarAmp, 0x00, sizeBufDbl);
    if(_avgSquPha != NULL){free(_avgSquPha);};
	   _avgSquPha = (double*) malloc(sizeBufDbl); memset( (void*) _avgSquPha, 0x00, sizeBufDbl);
    if(_avgSquAmp != NULL){free(_avgSquAmp);};
	   _avgSquAmp = (double*) malloc(sizeBufDbl); memset( (void*) _avgSquAmp, 0x00, sizeBufDbl);
    if(_avgPhiPha != NULL){free(_avgPhiPha);};
	   _avgPhiPha = (double*) malloc(sizeBufDbl); memset( (void*) _avgPhiPha, 0x00, sizeBufDbl);
    if(_avgPhiAmp != NULL){free(_avgPhiAmp);};
	   _avgPhiAmp = (double*) malloc(sizeBufDbl); memset( (void*) _avgPhiAmp, 0x00, sizeBufDbl);
	
	return;
}

/**
 *SR avg buffer DEallocator \n
 * don't forget to free
 */
void CamSRavg::AvgBufFree()
{
	if(_avgBuf.pha != NULL){free(_avgBuf.pha); _avgBuf.pha = NULL;};
	if(_avgBuf.amp != NULL){free(_avgBuf.amp); _avgBuf.amp = NULL;};

    if(_avgImgPha != NULL){free(_avgImgPha); _avgImgPha = NULL;};
    if(_avgImgAmp != NULL){free(_avgImgAmp); _avgImgAmp = NULL;};
	if(_avgVarPha != NULL){free(_avgVarPha); _avgVarPha = NULL;};
    if(_avgVarAmp != NULL){free(_avgVarAmp); _avgVarAmp = NULL;};
    if(_avgSquPha != NULL){free(_avgSquPha); _avgSquPha = NULL;};
    if(_avgSquAmp != NULL){free(_avgSquAmp); _avgSquAmp = NULL;};
    if(_avgPhiPha != NULL){free(_avgPhiPha); _avgPhiPha = NULL;};
    if(_avgPhiAmp != NULL){free(_avgPhiAmp); _avgPhiAmp = NULL;};
	return;
}

/*!
	Background learning initialization \n
	(MUST BE CALLED at least once before LearnBackground() !). \n
	 - resets background buffer, \n
	 - resets background variance buffer, \n
	 - resets background image count, \n
  \sa PLAVG_LearnBackground()
*/
//! Background learning initialization.
int CamSRavg::LearnBackgroundInitReset(SRBUF srBuf)
{
	int res = 0;
    if( (srBuf.nCols<1) || (srBuf.nRows<1) || (srBuf.nCols*srBuf.nRows*2*sizeof(unsigned short) != srBuf.bufferSizeInBytes) ){return -1;};

	if( (srBuf.nCols != _avgBuf.nCols) || (srBuf.nRows != _avgBuf.nRows) )
	{
		_avgBuf.nCols = srBuf.nCols;
		_avgBuf.nRows = srBuf.nRows;
		_avgBuf.bufferSizeInBytes = srBuf.bufferSizeInBytes;
		AvgBufFree();
		AvgBufAlloc();
	};

	_avgCnt = 0;
	unsigned int sizeBufUSh = (unsigned int) _avgBuf.nCols*_avgBuf.nRows*sizeof(unsigned short); // size of ushort buffer
    memcpy(_avgBuf.amp, srBuf.amp, sizeBufUSh); 
    memcpy(_avgBuf.pha, srBuf.pha, sizeBufUSh);

	return res;
}

//---------------------------------------------------
/*!
	Background learning. \n
	 - Sums phase,amplitude data in buffers. \n
	 - Calls background calculation procedure. \n
  \sa PLAVG_SetBGalpha()
*/
//! Background learning.
int CamSRavg::LearnBackground(SRBUF srBuf)
{
	int res = 0;
	int i = 0;
	double pi = 3.1415926535;
    if( (srBuf.nCols<1) || (srBuf.nRows<1) || (srBuf.nCols*srBuf.nRows*2*sizeof(unsigned short) != srBuf.bufferSizeInBytes) ){return -1;};

	if( (srBuf.nCols != _avgBuf.nCols) || (srBuf.nRows != _avgBuf.nRows) )
	{
		LearnBackgroundInitReset(srBuf);
	};

	int num=srBuf.nCols*srBuf.nRows;			// size of required data buffer
	unsigned short* pha= srBuf.pha;	// get SR phase
	unsigned short* amp= srBuf.amp;	// get SR amplitude
    double re = 0; double im = 0;		// variables to store current pixel information
    double reBG = 0; double imBG = 0; // variables to store background pixel information
    double rad = 0;	// variable to store phase information
    if(_avgCnt == 0)
    {
	  for(i=0;i<num;i++)	// for all pixels
	  {
        _avgImgPha[i] = ((double)(pha[i]));	// first PHASE image is taken as mean value
	    _avgImgAmp[i] = ((double)(amp[i]));	// first AMPLITUDE is taken as mean value
		_avgPhiPha[i] = ((double)(pha[i]));	// first PHASE image is taken as mean value
		_avgPhiAmp[i] = ((double)(amp[i]));	// first AMPLITUDE is taken as mean value
      }
    }

    if(_avgAlpha !=0.0)
    {
      for(i=0;i<num;i++)	// for all pixels in all images (amp, pha)
      {
	    /// If \f$ ( \alpha \not =  0 )\f$, the temporal IIR is enabled:
	    /// \f[ {\mathbf{S}}_{Bt} = (1 - \alpha) \cdot {\mathbf{S}}_{B(t-1)} + \alpha \cdot {\mathbf{S}}_{t} \f]
	    //COMPLEX averaging: compute real and imaginary parts for current and Background image
		    // Euler :  amp*e^(j*pha) = amp*( cos(pha) + j*sin(pha))
		re = ((double)amp[i] ) * cos( (( (double)pha[i] ) / 65535 * 2 * pi)) ;
		im = ((double)amp[i] ) * sin( (( (double)pha[i] ) / 65535 * 2 * pi)) ;
		reBG = (_avgImgAmp[i]) * cos( (( _avgImgPha[i] ) / 65535 * 2 * pi)) ;
		imBG = (_avgImgAmp[i]) * sin( (( _avgImgPha[i] ) / 65535 * 2 * pi)) ;

		  //_avgVarD[i]= (1.0 - _avgAlpha)* _avgVarD[i] + _avgAlpha*(
		  //	             ( ((re - reBG)*(re - reBG)) + ((im - imBG)*(im - imBG)))* 65535 / 2 /pi);
		      // vector variance is too sensitive to amplitude variations
		//! - phase variance \f[ {\sigma}^2_{{\varphi},B,t} = (1 - \alpha) \cdot {\sigma}^2_{{\varphi},B,(t-1)} + \alpha \cdot ({{\varphi}_t} - {\mu}_{{\varphi},B,(t-1)} )^2 \f]
		_avgVarPha[i]= (1.0 - _avgAlpha)* _avgVarPha[i] + _avgAlpha*(((double)(pha[i]) - _avgImgPha[i]) * ((double)(pha[i]) - _avgImgPha[i]) );
		    // new PHASE variance value
		//! - amplitude variance \f[ {\sigma}^2_{{A},B,t} = (1 - \alpha) \cdot {\sigma}^2_{{A},B,(t-1)} + \alpha \cdot ({A_t} - {\mu}_{{A},B,(t-1)} )^2 \f]
		_avgVarAmp[i]= (1.0 - _avgAlpha)* _avgVarAmp[i] + _avgAlpha*(((double)(amp[i]) - _avgImgAmp[i]) * ((double)(amp[i]) - _avgImgAmp[i]) );
		    // new amplitude variance value
		reBG = (1.0 - _avgAlpha)* reBG + _avgAlpha*re; // new average REAL value
	    imBG = (1.0 - _avgAlpha)* imBG + _avgAlpha*im; // new average IMAG value

		rad = atan2( imBG, reBG) ;	// compute phase (through atan2)
		if ((rad<0))rad += 2*pi;	// unwrap phase (only positive values)
		_avgImgPha[i] = (65535* rad / (2*pi)); // scale phase value to SR format
		_avgImgAmp[i] = sqrt((reBG*reBG) + (imBG*imBG)); // compute amplitude

		// cast into SR buffer
		_avgBuf.pha[i] = (unsigned short) _avgImgPha[i];
		_avgBuf.amp[i] = (unsigned short) _avgImgAmp[i];
    }
  }// if(_avgAlpha !=0.0)
  else
  {
	double avgCntD = (double)(_avgCnt);
	double phaPI; phaPI = 0.0;
	for(i=0;i<num;i++)	// for all pixels in all images (amp, pha)
    {
		/*if(pha[i]>32767){ phaPI = (double)(pha[i]-65535); }
		else {phaPI = (double)(pha[i]);}*/
		phaPI = (double)(pha[i]);
		_avgSquPha[i] = ( (avgCntD/(avgCntD+1.0)) * _avgSquPha[i] )+ ( (1.0/(avgCntD+1.0)) * phaPI * phaPI );  // sum of pha^2 needed for variance
		_avgSquAmp[i] = ( (avgCntD/(avgCntD+1.0)) * _avgSquAmp[i] )+ ( (1.0/(avgCntD+1.0)) *(double)(amp[i]) * (double)(amp[i]) ); // sum of amp^2 needed for variance
	  /// If \f$ ( \alpha =  0 )\f$, the temporal IIR is disabled, and the standard average is used
	  /// \f[ {\mathbf{S}}_{B,N} =  \frac{1}{N} \sum_{k=1}^{N} {\mathbf{S}}_{k} \f]
	  //COMPLEX averaging: compute real and imaginary parts for current and Background image
		    // Euler :  amp*e^(j*pha) = amp*( cos(pha) + j*sin(pha))
		re = ((double)amp[i] ) * cos( (( (double)pha[i] ) / 65535 * 2 * pi)) ;
		im = ((double)amp[i] ) * sin( (( (double)pha[i] ) / 65535 * 2 * pi)) ;
		reBG = (_avgImgAmp[i]) * cos( (( _avgImgPha[i] ) / 65535 * 2 * pi)) ;
		imBG = (_avgImgAmp[i]) * sin( (( _avgImgPha[i] ) / 65535 * 2 * pi)) ;

		reBG = ( (avgCntD/(avgCntD+1.0)) * reBG )+ ( (1.0/(avgCntD+1.0)) *re ); // new average REAL value
	    imBG = ( (avgCntD/(avgCntD+1.0)) * imBG )+ ( (1.0/(avgCntD+1.0)) *im ); // new average IMAG value

		rad = atan2( imBG, reBG) ;	// compute phase (through atan2)
		if ((rad<0))rad += 2*pi;	// unwrap phase (only positive values)
		_avgImgPha[i] = (65535.0* (rad / (2*pi))); // scale phase value to SR format
		_avgImgAmp[i] = sqrt((reBG*reBG) + (imBG*imBG)); // compute amplitude

		_avgPhiPha[i] = ( (avgCntD/(avgCntD+1.0)) * _avgPhiPha[i] )+ ( (1.0/(avgCntD+1.0)) * phaPI  );  // avg of pha needed for variance
		_avgPhiAmp[i] = ( (avgCntD/(avgCntD+1.0)) * _avgPhiAmp[i] )+ ( (1.0/(avgCntD+1.0)) *(double)(amp[i]) ); // avg of amp needed for variance
		//! - phase variance: \f$ {\sigma}^2_{{\varphi},B,N} =  \frac{1}{N} \sum_{k=1}^{N} {{\varphi}_{k}}^2  - \left( \frac{1}{N} \sum_{k=1}^{N} {{\varphi}_{k}} \right)^2 \f$
		_avgVarPha[i] = (_avgSquPha[i] - _avgPhiPha[i]*_avgPhiPha[i]) ;
		//! - amplitude variance: \f$ {\sigma}^2_{{A},B,N} =  \frac{1}{N} \sum_{k=1}^{N} {{A}_{k}}^2  - \left( \frac{1}{N} \sum_{k=1}^{N} {{A}_{k}} \right)^2 \f$
		_avgVarAmp[i] = (_avgSquAmp[i] - _avgPhiAmp[i]*_avgPhiAmp[i] );

		// cast into SR buffer
		_avgBuf.pha[i] = (unsigned short) _avgImgPha[i];
		_avgBuf.amp[i] = (unsigned short) _avgImgAmp[i];
    }
  };
  _avgCnt++;			// increment image count
  return res;
}

SRBUF   CamSRavg::GetAvgBuf() //!< Returns the current average buffer
{
	return _avgBuf;
}

//---------------------------------------------------
/*!
   Sets alpha factor for adaptive buffer learning.
 \f[ {\mathbf{S}}_{B,t} = (1 - \alpha) \cdot {\mathbf{S}}_{B,(t-1)} + \alpha \cdot {\mathbf{S}}_{t} \f]
 where: \f$ {\mathbf{S}}_{t} = A_t \cdot e^{\jmath {\varphi}_t} \f$. \n
 Alpha must be between 0.0 and 1.0 \n
 - average phase:  \f$ {\mu}_{{\varphi},B,t} =  angle( S_{B,t} ) \f$
 - average amplitude: \f$ {\mu}_{{A},B,t} =  abs( S_{B,t} ) \f$
 - phase variance: \f$ {\sigma}^2_{{\varphi},B,t} = (1 - \alpha) \cdot {\sigma}^2_{{\varphi},B,(t-1)} + \alpha \cdot ({{\varphi}_t} - {\mu}_{{\varphi},B,(t-1)} )^2 \f$
 - amplitude variance: \f$ {\sigma}^2_{{A},B,t} = (1 - \alpha) \cdot {\sigma}^2_{{A},B,(t-1)} + \alpha \cdot ({A_t} - {\mu}_{{A},B,(t-1)} )^2 \f$ \n
 \n
 NEW NEW NEW \n
 If Alpha is set equal to 0.0 ( \f$ {\alpha} = 0 \f$ ), the IIR is disabled.
 The background model is then computed as a standard average :\n
 \f[ {\mathbf{S}}_{B,N} =  \frac{1}{N} \sum_{k=1}^{N} {\mathbf{S}}_{k} \f]
 The background phase and amplitude are still computed from the complex average.
 - average phase:  \f$ {\mu}_{{\varphi},B,N} =  angle( S_{B,N} ) \f$
 - average amplitude: \f$ {\mu}_{{A},B,N} =  abs( S_{B,N} ) \f$ \n
 However, in this case, the deviation is computed from the phase algebraic average :\n
 - phase variance: \f$ {\sigma}^2_{{\varphi},B,N} =  \frac{1}{N} \sum_{k=1}^{N} {{\varphi}_{k}}^2  - \left( \frac{1}{N} \sum_{k=1}^{N} {{\varphi}_{k}} \right)^2 \f$
 - amplitude variance: \f$ {\sigma}^2_{{A},B,N} =  \frac{1}{N} \sum_{k=1}^{N} {{A}_{k}}^2  - \left( \frac{1}{N} \sum_{k=1}^{N} {{A}_{k}} \right)^2 \f$
\n \n \n
WARNING: Setting \f$ \alpha \f$ to 0.0 will reset the background model, since temporal IIR model
and standard average model are not compatible.
 \sa LearnBackground()
*/
int CamSRavg::SetAvgAlpha(double alpha)
{
	if(alpha < 0.0)  return -1;
	if(alpha > 1.0)  return -1;
	_avgAlpha = alpha;
	if(alpha == 0.0) return LearnBackgroundInitReset(_avgBuf); // should reset the average when disabling IIR
	return 0;
}

//! Retrieves alpha factor for adaptive learning.
//! \sa PLAVG_GetAvgAlpha(), SetAvgAlpha()
double CamSRavg::GetAvgAlpha()
{
	double alpha = _avgAlpha;
	return alpha;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
/////////////////////// API FUNCTIONS //////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

SRPLAVG_API int PLAVG_Open(SRPLAVG* srplAvg, SRBUF srBuf)
{
  //if(!srBuf)return -1;
  *srplAvg= new CamSRavg(srBuf);
  return 0;
}
SRPLAVG_API int PLAVG_Close(SRPLAVG srplAvg)
{
  if(!srplAvg)return -1;
  delete(srplAvg);
  return 0;
}
SRPLAVG_API int PLAVG_LearnBackgroundInitReset(SRPLAVG srplAvg, SRBUF srBuf)
{
  if(!srplAvg)return -1;
  return  srplAvg->LearnBackgroundInitReset(srBuf);
}
SRPLAVG_API int PLAVG_LearnBackground(SRPLAVG srplAvg, SRBUF srBuf)
{
  if(!srplAvg)return -1;
  return srplAvg->LearnBackground(srBuf);
}

//SRPLAVG_API int PLAVG_LearnBackgroundEnd(SRPLAVG srplAvg)
//{
//  if(!srplAvg)return -1;
//  int res = srplAvg->LearnBackgroundEnd();
//  return res;
//}

SRPLAVG_API SRBUF PLAVG_GetAvgBuf(SRPLAVG srplAvg)
{
  if(!srplAvg)
  { 
	  SRBUF buf; buf.amp=NULL; buf.pha = NULL; 
	  buf.nCols = 0; buf.nRows=0; buf.bufferSizeInBytes=0;
	  return buf;
  }
  return srplAvg->GetAvgBuf();
}

SRPLAVG_API int PLAVG_GetAvgCnt(SRPLAVG srplAvg)
{
  if(!srplAvg)return -1;
  return srplAvg->GetAvgCnt();
}

SRPLAVG_API int PLAVG_SetAvgAlpha(SRPLAVG srplAvg, double alpha)
{
  if(!srplAvg)return -1;
  return srplAvg->SetAvgAlpha(alpha);
}

SRPLAVG_API double PLAVG_GetAvgAplha(SRPLAVG srplAvg)
{
  if(!srplAvg)return -1.0;
  return  srplAvg->GetAvgAlpha();
}