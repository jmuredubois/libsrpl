/*
 * CamFileSRPL.cpp
 * Implementation of the file handling methods
 *
 * @author: James Mure-Dubois
 * @version: 2008.12.11
 */

#include "libSRPLfile.h" //!< export header file
#include "CamFileSRPL.h" //!< internal header file

/**
 * Camera frame class constructor \n
 * Each instance must have a parent frame (usually CMainWnd) \n
 */
CamFileSRPL::CamFileSRPL(SRBUF srBuf)
{
	_imgSz.cx = srBuf.nCols;
	_imgSz.cy = srBuf.nRows;

}

/**
 * Camera frame class destructor \n
 */
CamFileSRPL::~CamFileSRPL()
{

}

//! read  buffer 
int CamFileSRPL::Read(SRBUF rdBuf)
{
	int res = 0;
    if( (rdBuf.amp==NULL) || (rdBuf.pha==NULL) || (rdBuf.nCols<1) || (rdBuf.nRows<1) || (rdBuf.nCols*rdBuf.nCols*sizeof(unsigned short) != rdBuf.bufferSizeInBytes) ){return -1;};
    rdBuf.amp = _curBuf.amp;
    rdBuf.pha = _curBuf.pha;
    rdBuf.nCols = _curBuf.nCols;
    rdBuf.nRows = _curBuf.nRows;
    rdBuf.bufferSizeInBytes = _curBuf.bufferSizeInBytes;
	return res;
}

//! read  buffer 
int CamFileSRPL::Write(SRBUF wrBuf)
{
	int res = 0;
    if( (wrBuf.amp==NULL) || (wrBuf.pha==NULL) || (wrBuf.nCols<1) || (wrBuf.nRows<1) || (wrBuf.nCols*wrBuf.nCols*sizeof(unsigned short) != wrBuf.bufferSizeInBytes) ){return -1;};
    _curBuf.amp = wrBuf.amp;
    _curBuf.pha = wrBuf.pha;
    _curBuf.nCols = wrBuf.nCols;
    _curBuf.nRows = wrBuf.nRows;
    _curBuf.bufferSizeInBytes = wrBuf.bufferSizeInBytes;
	return res;
}



//---------------------------------------------------
/*!
	This function sets a file name for amplitude
*/
int CamFileSRPL::SetFileNameAmp(const char* fn)
{
	int res=0;
	return res;
}

//---------------------------------------------------
/*!
	This function sets a file name for amplitude
*/
int CamFileSRPL::SetFileNameAmp(const char* fn)
{
	int res = 0;
	// define local variables for image dimensions
	int wdt = _imgSz.cx;	// image width
	int hgt = _imgSz.cy;	// image height
	int num=wdt*hgt;	// pixel count

	// file open operation
	FILE* pModel = fopen(fn, "rb");
	if(!pModel) return -1;
	if(!_cxBuf) return -2;
	double val;
	double max=0.0; int indMax = 0; // DEBUG, for parity filter
	for(int k=0; k<num ; k++) // loop on all coefficients
	{
		fread(&val, sizeof(double), 1, pModel);
		_cxBuf[k] = std::complex<double>(val, 0.0);
		_psf[k] = val;
		//_cxBuf[k] = std::complex<double>(0.0, 0.0);// DEBUG DEBUG DEBUG
		if(k==0)// DEBUG, for parity filter
		{
			max = val;
		}
		if(val>max)// DEBUG, for parity filter
		{
			max = val;
			indMax = k;
		}
		_cxPar[k] = std::complex<double>(0.0, 0.0);// DEBUG, for parity filter
	}
	//_cxBuf[(int)floor(num/2.0)-0] = std::complex<double>(1.0, 0.0);// DEBUG DEBUG DEBUG
	_cxPar[indMax] = std::complex<double>(1.0,0.0); //DEBUG, for parity unity filter

	res+=fclose(pModel); //close file

	res+= UpdateScatPsf4PadDft(); // _cxBuf, _cxFilt and _cxPar will NOT be changed
	res+= UpdateScatPsf4Dft();	  // _cxBuf, _cxFilt and _cxPar will be changed

	return res;
}

SRPLFILE_API int PLFL_Open(SRPLFILE* srplFile, SRBUF srBuf, bool readOnly)
{
  //if(!srBuf)return -1;
  *srplFile= new CamFileSRPL(srBuf);
  return 0;
}
SRPLFILE_API int PLFL_Close(SRPLFILE srplFile)
{
  if(!srplFile)return -1;
  delete(srplFile);
  return 0;
}
SRPLFILE_API int PLFL_Read(SRPLFILE srplFile, SRBUF srBuf)
{
  if(!srplFile)return -1;
  int res = srplFile->Read(srBuf, nanBuf);
  return res;
}
SRPLFILE_API int PLFL_Write(SRPLFILE srplFile, SRBUF srBuf)
{
  if(!srplFile)return -1;
  int res = srplFile->Write(srBuf, nanBuf);
  return res;
}

SRPLFILE_API int PLFL_SetFileNameAmp(SRPLFILE srplFile, const char* fn)
{
  if(!srplFile)return -1;
  int res = srplFile->SetFileNameAmp(fn);
  return res;
}

SRPLFILE_API int PLFL_SetFileNamePha(SRPLFILE srplFile, const char* fn)
{
  if(!srplFile)return -1;
  int res = srplFile->SetFileNamePha(fn);
  return res;
}