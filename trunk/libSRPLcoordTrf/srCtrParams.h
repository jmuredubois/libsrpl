/*
 * srBuf.h
 * A convenience structure to define a srBuffer (amp and phase),
 * along with number of rows and columns
 *
 *    Copyright 2009 James Mure-Dubois, Heinz H�gli and Institute of Microtechnology of EPFL.
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
 */

#ifndef SRCTRPARAMS
#define SRCTRPARAMS
typedef struct srCtr{
	int nCols;
    int nRows;
    float cX; // optical center X position
    float cY; // optical center Y position
	float f; // focal [mm]
	float pixDX; // pixel pitch X direction
    float pixDY; // pixel pitch X direction
	float maxMM; // milimeters corresponding to max ushort phase value
} SRCTR;
#endif // 