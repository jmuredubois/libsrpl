/*
 * CamFlagNaN.cpp
 * Implementation of the NaN flagging methods
 *
 * @author: James Mure-Dubois
 * @version: 2008.10.30
 */

#include "imgPadder.h"
#include "CamFlagNaN.h" //!< camera settings panel header file
#include "CamFlagNaNInt.h" //!< camera settings panel header file


/**
 * Camera frame class constructor \n
 * Each instance must have a parent frame (usually CMainWnd) \n
 */
CamFlagNaN::CamFlagNaN(SRBUF srBuf)
{
	_imgNaN.nCols = srBuf.nCols;
	_imgNaN.nRows = srBuf.nRows;
	_imgNaN.nanBool = NULL;
	_imgNaN.bufferSizeInBytes = _imgNaN.nCols*_imgNaN.nRows*sizeof(bool);
}

/**
 * Camera frame class destructor \n
 */
CamFlagNaN::~CamFlagNaN()
{
	if(_imgNaN.nanBool!=NULL){delete(_imgNaN.nanBool); _imgNaN.nanBool=NULL;};
}


//---------------------------------------------------
/*!
	FlagNaNs() flags invalid (NIL) points. \n
	- FlagNaNs() produces boolean masks _imgNaN and _imgNaNbg . \n
	\sa PL_srAcquire(), PL_srReplay()
*/
//! This function flags invalid (NIL) points.
bool* CamFlagNaN::FlagNaN(SRBUF nanBuf)
{
	//! This function intends to flag invalid points.
	//! Such points have either a null intensity value
	//! or a null depth.
	//! When segmentation methods using background are used
	//! we must also flag points where the background image
	//! is invalid. (fct CalcBackground() takes care of this).
#ifdef FLAGNTIMER
	_flagNTimer.StartTimer();
#endif
	int num= nanBuf.nRows * nanBuf.nCols;//(int)_numRow*(int)_numCol;			// compute number of pixels
	unsigned short* pha = nanBuf.pha;//(WORD*)SR_GetImage(_srCam,0);	// read current Swissranger phase image
	unsigned short* amp = nanBuf.amp;//(WORD*)SR_GetImage(_srCam,1);	// read current Swissranger amplitude image
	if( (nanBuf.nCols != _imgNaN.nCols) || (nanBuf.nCols != _imgNaN.nCols) || ( _imgNaN.nanBool==NULL) )
	{
	    if(( _imgNaN.nanBool != NULL) ){free(_imgNaN.nanBool); _imgNaN.nanBool=NULL;};
	    _imgNaN.nCols = nanBuf.nCols;
	    _imgNaN.nRows = nanBuf.nRows;
	    _imgNaN.nanBool = (bool*)malloc(sizeof(bool)*num);
		memset( (void*) _imgNaN.nanBool, 0x00, sizeof(bool)*num);
		_imgNaN.bufferSizeInBytes = sizeof(bool)*num;
	}
	int i;
	for(i=0;i<num;i++)		// for all pixels
	{
		if( (pha[i]==0) || (amp[i] == 0) )	// check if either phase or amplitude is NIL
		{
		  _imgNaN.nanBool[i] = 1;	// flag the pixel as NIL in boolean image
		  pha[i] = 65535;	// change the pixel phase value to max phase
		}
		else				// if not NIL
		{
		  _imgNaN.nanBool[i]=0;		// flag the pixel as non-NIL in boolean image
		}
	}
#ifdef FLAGNTIMER
	__int64 tic = _flagNTimer.GetTime();
	// float timeFlagN_s = (float)tic/1e6f;
	_timeFlagN_s = (float)tic/1e6f;
#endif
	return _imgNaN.nanBool;
}

SRPLSCAT_API int PLNN_Open(SRPLNAN* srplNaN, SRBUF srBuf )
{
  //if(!srBuf)return -1;
  *srplNaN= new CamFlagNaN(srBuf);
  return 0;
}
SRPLSCAT_API int PLNN_Close(SRPLNAN srplNaN)
{
  if(!srplNaN)return -1;
  delete(srplNaN);
  return 0;
}
SRPLSCAT_API bool* PLNN_FlagNaN(SRPLNAN srplNaN, SRBUF srBuf)
{
  if(!srplNaN)return NULL;
  return srplNaN->FlagNaN(srBuf);
}
//SRPLSCAT_API int PLNN_GetNaNs(SRPLNAN srplNaN, SRBUF srBuf); // NOT YET IMPLEMENTED
//{
//  if(!srplNaN)return NULL;
//  return srplNaN->GetNaNs(srBuf);
//}