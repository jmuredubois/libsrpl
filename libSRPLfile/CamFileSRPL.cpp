/*
 * CamFileSRPL.cpp
 * Implementation of the file handling methods
 *
 *    Copyright 2009 James Mure-Dubois, Heinz Hügli and Institute of Microtechnology of EPFL.
 *      http://parlab.epfl.ch/
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $Rev$:
 * $Author$:
 * $Date$:
 * THIS LIBRARY IS NOT COMPLETE YET, AND CURRENTLY NOT NEEDED
 */

#include "libSRPLfile.h" //!< export header file
#include "CamFileSRPL.h" //!< internal header file

/**
 * Camera frame class constructor \n
 * Each instance must have a parent frame (usually CMainWnd) \n
 */
CamFileSRPL::CamFileSRPL(SRBUF srBuf)
{
	_curBuf.amp = srBuf.pha;
	_curBuf.pha = srBuf.pha;
	_curBuf.nCols = srBuf.nCols;
	_curBuf.nRows = srBuf.nRows;
	_curBuf.bufferSizeInBytes = srBuf.bufferSizeInBytes;
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
	return res;
}

SRPLFILE_API int PLFL_Open(SRPLFILE* srplFile, SRBUF srBuf, bool readOnly)
{
  //if(!srBuf)return -1;
  *srplFile= new CamFileSRPL(srBuf, readOnly);
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