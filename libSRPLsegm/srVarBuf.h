/*
 * srVarBuf.h
 * A convenience structure to define a srVarianceBuffer (amp and phase),
 * along with number of rows and columns
 *
 * @author: James Mure-Dubois
 * @version: 2008.05.11
 */

#ifndef SRVARBUFHEADER
#define SRVARBUFHEADER
typedef struct srVariBufVar{
    double* amp;
    double* pha;
    int nCols;
    int nRows;
    int bufferSizeInBytes;
} SRVARBUF;
#endif // 
