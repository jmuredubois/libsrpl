/*
 * srRscPlan.h
 * A convenience structure to define a SR ransac plan 
 * which has a normal and a list of inliers
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
 */

#ifndef SRRSCPLANHEADER
#define SRRSCPLANHEADER
typedef struct rscPlanVar{
	double nVec[4];
	double matZ[16];
	int iter;
    std::vector<int> inliers;
	std::vector<int> outliers;
} RSCPLAN;
#endif // 

