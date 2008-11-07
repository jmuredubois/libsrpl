/*
 * srBuf.h
 * A convenience structure to define a srBuffer (amp and phase),
 * along with number of rows and columns
 *
 * @author: James Mure-Dubois
 * @version: 2008.10.30
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