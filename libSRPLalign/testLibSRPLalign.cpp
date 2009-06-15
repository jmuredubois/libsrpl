/*
 * testLibSRPLalign.cpp
 * SR ALIGNMENT test
 *
 * @author: James Mure-Dubois
 * @version: 2009.06.15
 */

#include "libSRPLalign.h"
#include <iostream>

int main(int argc, char *argv[])
{
	//! initialise
	SRPLALI ali = 0;
	int res = PLALI_Open(&ali);

	//! perform the tests
	//double n00[4], n01[4], n02[4], n10[4], n11[4], n12[4];
	//n00[0] = ;
	double mat[16];  double n0[12]; double n1[12];
	for(int k=0; k<16; k++) { mat[k] = 0.0;};
	for(int k=0; k<12; k++) { n0[k] = 0.0; n1[k] = 0.0;};
	
	//! HARDCODED test values
	//n0[ 0] = +1.00000 ; n0[ 1] = +0.00000; n0[ 2] = +0.00000; n0[ 3] = +1000.00;
	//n0[ 4] = +0.00000 ; n0[ 5] = +1.00000; n0[ 6] = +0.00000; n0[ 7] = +2000.00;
	//n0[ 8] = +0.00000 ; n0[ 8] = +0.00000; n0[10] = +1.00000; n0[11] = +3000.00;
	////n1
	//n1[ 0] = +0.00000 ; n1[ 1] = +1.00000; n1[ 2] = +0.00000; n1[ 3] = +100.00;
	//n1[ 4] = -1.00000 ; n1[ 5] = +0.00000; n1[ 6] = +0.00000; n1[ 7] = +200.00;
	//n1[ 8] = +0.00000 ; n1[ 8] = +0.00000; n1[10] = +1.00000; n1[11] = +300.00;

	//! HARDCODED test values
	n0[ 0] = +0.24650 ; n0[ 1] = -0.52230; n0[ 2] = -0.81640; n0[ 3] = +1579.00;
	n0[ 4] = -0.03611 ; n0[ 5] = +0.7576; n0[ 6] = -0.6517; n0[ 7] = +01059;
	n0[ 8] = -0.9052 ; n0[ 8] = -0.1375 ; n0[10] = -0.4022; n0[11] = +01205;
	//n1
	n1[ 0] = +0.7704 ; n1[ 1] = -0.2315; n1[ 2] = -0.594; n1[ 3] = +00781;
	n1[ 4] = +0.008164 ; n1[ 5] = +0.8274; n1[ 6] = -0.5615; n1[ 7] = +818.2;
	n1[ 8] = -0.4919 ; n1[ 8] = -0.3976 ; n1[10] = -0.7746; n1[11] = +01125;

	res += PLALI_align3plans(ali, mat, n0, n1);

	//! clean up
	res += PLALI_Close(ali);
}
