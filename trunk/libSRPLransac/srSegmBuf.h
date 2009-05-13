/*
 * srSegmBuf.h
 * A convenience structure to define a srSegmentationBuffer (bg and fg),
 * along with number of rows and columns
 *
 * @author: James Mure-Dubois
 * @version: 2009.05.11
 */

#ifndef SRSEGMBUFHEADER
#define SRSEGMBUFHEADER
typedef struct srSegmBufVar{
    unsigned char* fg;
    unsigned char* bg;
    int nCols;
    int nRows;
    int bufferSizeInBytes;
} SRSEGMBUF;
#endif // 
