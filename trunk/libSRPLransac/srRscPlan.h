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
    std::vector<int> inliers;
    double nVec[4];
	int iter;
} RSCPLAN;
#endif // 

