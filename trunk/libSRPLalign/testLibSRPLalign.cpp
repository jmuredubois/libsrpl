/*
 * testLibSRPLalign.cpp
 * SR ALIGNMENT test
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

#include "libSRPLalign.h"
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

void wait_for_enter()
{
  std::cout << "press <enter> to continue...\n";
  // Reset failstate, just in case.
  std::cin.clear();
  std::string line;
  std::getline( std::cin, line);
}

void printMat4(double mat[16])
{
  // display results
	std::cout << "mat" << std::endl;
	for(int row = 0; row < 4; row++)
	{
	  std::cout << "  <Row" << row ;
	  for(int col = 0; col < 4; col++)
	  {
		  std::cout << " val" << col << "=\"";
		  std::cout << mat[col*4+row] << "\"";
	  }
	  std::cout << " />" << std::endl;
	}
}

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
	//n0[ 8] = +0.00000 ; n0[ 9] = +0.00000; n0[10] = +1.00000; n0[11] = +3000.00;
	////n1
	//n1[ 0] = +0.50000 ; n1[ 1] = +0.86603; n1[ 2] = +0.00000; n1[ 3] = +100.00;
	//n1[ 4] = -0.86603 ; n1[ 5] = +0.50000; n1[ 6] = +0.00000; n1[ 7] = +200.00;
	//n1[ 8] = +0.00000 ; n1[ 9] = +0.00000; n1[10] = +1.00000; n1[11] = +300.00;

	//! HARDCODED test values
	//n0[ 0] = +0.24650 ; n0[ 1] = -0.52230; n0[ 2] = -0.81640; n0[ 3] = +1579.00;
	//n0[ 4] = -0.03611 ; n0[ 5] = +0.7576; n0[ 6] = -0.6517; n0[ 7] = +1059;
	//n0[ 8] = -0.9052 ; n0[ 9] = -0.1375 ; n0[10] = -0.4022; n0[11] = +1205;
	////n1
	//n1[ 0] = +0.7704 ; n1[ 1] = -0.2315; n1[ 2] = -0.594; n1[ 3] = +781;
	//n1[ 4] = +0.008164 ; n1[ 5] = +0.8274; n1[ 6] = -0.5615; n1[ 7] = +818.2;
	//n1[ 8] = -0.4919 ; n1[ 9] = -0.3976 ; n1[10] = -0.7746; n1[11] = +1125;

	//! HARDCODED test values
	double p00[2], p01[2],p02[2], p10[2], p11[2],p12[2] ;
	p00[0] = 668.02; p00[1] = -387.51;
	p01[0] = 395.60; p01[1] = -422.02;
	p02[0] = 270.05; p02[1] = -602.94;
	p10[0] = 888.37; p10[1] = -79.374;
	p11[0] = 641.50; p11[1] = -143.07;
	p12[0] = 557.99; p12[1] = -319.42;
	double norm = 0; double nS = 0;
	n0[ 0] = p01[0] - p00[0]; n0[ 1] = p01[1] - p00[1]; norm = sqrt( n0[ 0]*n0[ 0] + n0[ 1]*n0[ 1]); nS = n0[ 0];
	n0[ 0] = -n0[ 1]/norm;    n0[ 1] = nS/norm; n0[ 2] = 0; n0[ 3] = n0[ 0]*p00[0] + n0[ 1]*p00[1];
	n0[ 4] = p02[0] - p01[0]; n0[ 5] = p02[1] - p01[1]; norm = sqrt( n0[ 4]*n0[ 4] + n0[ 5]*n0[ 5]); nS = n0[ 4];
	n0[ 4] = -n0[ 5]/norm;    n0[ 5] = nS/norm; n0[ 6] = 0; n0[ 7] = n0[ 4]*p01[0] + n0[ 5]*p01[1];
	n0[ 8] = p00[0] - p02[0]; n0[ 9] = p00[1] - p02[1]; norm = sqrt( n0[ 8]*n0[ 8] + n0[ 9]*n0[ 9]); nS = n0[ 8];
	n0[ 8] = -n0[ 9]/norm;    n0[ 9] = nS/norm; n0[10] = 0; n0[11] = n0[ 8]*p02[0] + n0[ 9]*p02[1];

	n1[ 0] = p11[0] - p10[0]; n1[ 1] = p11[1] - p10[1]; norm = sqrt( n1[ 0]*n1[ 0] + n1[ 1]*n1[ 1]); nS = n1[ 0];
	n1[ 0] = -n1[ 1]/norm;    n1[ 1] = nS/norm; n1[ 2] = 0; n1[ 3] = n1[ 0]*p10[0] + n1[ 1]*p10[1];
	n1[ 4] = p12[0] - p11[0]; n1[ 5] = p12[1] - p11[1]; norm = sqrt( n1[ 4]*n1[ 4] + n1[ 5]*n1[ 5]); nS = n1[ 4];
	n1[ 4] = -n1[ 5]/norm;    n1[ 5] = nS/norm; n1[ 6] = 0; n1[ 7] = n1[ 4]*p11[0] + n1[ 5]*p11[1];
	n1[ 8] = p10[0] - p12[0]; n1[ 9] = p10[1] - p12[1]; norm = sqrt( n1[ 8]*n1[ 8] + n1[ 9]*n1[ 9]); nS = n1[ 8];
	n1[ 8] = -n1[ 9]/norm;    n1[ 9] = nS/norm; n1[10] = 0; n1[11] = n1[ 8]*p12[0] + n1[ 9]*p12[1];
	
	/****************************/
	/* test alignment function  */
	/****************************/
	res += PLALI_align3plans(ali, mat, n0, n1);
	// display results
	printMat4(mat);

	/****************************/
	/* test new interface alignment function  */
	/****************************/
	int np=3;for(int k=0; k<16; k++) { mat[k] = 0.0;};
	JMUPLAN3D *plans0, *plans1;
	plans0 = (JMUPLAN3D*) malloc(np*sizeof(JMUPLAN3D));
	plans1 = (JMUPLAN3D*) malloc(np*sizeof(JMUPLAN3D));
	memset(plans0, 0x0, np*sizeof(JMUPLAN3D));
	memset(plans1, 0x0, np*sizeof(JMUPLAN3D));
	int k2=0;
	for(int pl = 0; pl<np; pl++)
	{
		for(int k = 0; k<4; k++)
		{
		
			plans0[pl].n[k] = n0[k2];
			plans1[pl].n[k] = n1[k2];
			k2++;
		}
	}
	res += PLALI_alignNplans(ali, mat, np, plans0, plans1);
	free(plans0);
	free(plans1);
	// display results
	printMat4(mat);

    /****************************/
	/* test alignment method based on 2d points  */
	/****************************/
	int npts = 6;//3;
	double *x0, *y0, *x1, *y1;
	x0 = (double*) malloc(npts*sizeof(double)); memset(x0, 0x0, npts*sizeof(double));
	y0 = (double*) malloc(npts*sizeof(double)); memset(y0, 0x0, npts*sizeof(double));
	x1 = (double*) malloc(npts*sizeof(double)); memset(x1, 0x0, npts*sizeof(double));
	y1 = (double*) malloc(npts*sizeof(double)); memset(y1, 0x0, npts*sizeof(double));
	x0[0] = 668.02; y0[0] = -387.51;
	x0[1] = 395.60; y0[1] = -422.02;
	x0[2] = 270.05; y0[2] = -602.94;
	x1[0] = 888.37; y1[0] = -79.374;
	x1[1] = 641.50; y1[1] = -143.07;
	x1[2] = 557.99; y1[2] = -319.42;
	x0[3] = 291.60; y0[3] = -741.42;
	x0[4] = 808.94; y0[4] = -362.81;
	x0[5] = 127.26; y0[5] = -475.59;
	x1[3] = 594.20; y1[3] = -456.40;
	x1[4] = 988.93; y1[4] = -46.257;
	x1[5] = 529.36; y1[5] = -177.41;
	PLALI_align2dNpoints(ali, mat, npts, y0, x0, y1, x1);
	free(x0); free(y0); free(x1); free(y1);
	// display results
	printMat4(mat);

	//! clean up
	res += PLALI_Close(ali);

	std::ofstream fout;
	fout.open("eigRes.xml");
	// display results
	fout << "<?xml version=\"1.0\" encoding=\"utf-8\" ?>" << std::endl;
	fout << "<TrfMat>" << std::endl;
	for(int row = 0; row < 4; row++)
	{
	  fout << "    <Row" << row ;
	  for(int col = 0; col < 4; col++)
	  {
		  fout << " val" << col << "=\"";
		  fout << mat[col*4+row] << "\"";
	  }
	  fout << " />" << std::endl;
	}
	fout << "</TrfMat>" << std::endl;
	fout.close();

	wait_for_enter();
}

