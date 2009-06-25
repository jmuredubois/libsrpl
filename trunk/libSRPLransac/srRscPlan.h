/*
 * srRscPlan.h
 * A convenience structure to define a SR ransac plan 
 * which has a normal and a list of inliers
 *
 * @author: James Mure-Dubois
 * @version: 2009.05.25
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

