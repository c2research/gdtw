/*
 * dtw_mink_mex.cpp - call MATLAB External Interfaces
 *
 * Computes the DTW_mink distance between two time series
 * Output is a 1x1 matrix
 * The calling syntax is:
 *
 *		outMatrix = dtw_mink(inMatrix_1, inMatrix_2)
 *
 * This is a MEX file for MATLAB.
 */

#include <mex.h>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <cmath>
#include <time.h>
#include <map>
#include <set>
#include <algorithm>
#include <vector>

#define INF 1e20       //Pseudo Infitinte number for this code

void arrayProduct(double *x, double *y, double *z, mwSize n);
double distFunc(double x, double y);
double distFuncM(double x, double y);
double distFuncC(double x, double y);
void DTW_mink(double *t1, double *t2, double *result, int ncols);
void DTW_manhattan(double *t1, double *t2, double *result, int ncols);
void DTW_euclidean(double *t1, double *t2, double *result, int ncols);

/* The gateway function */
void mexFunction(int nlhs, mxArray *plhs[],
        int nrhs, const mxArray *prhs[])
{
    
    if(nrhs != 2) {
        mexErrMsgIdAndTxt("MyToolbox:arrayProduct:nrhs",
                "Two inputs required.");
    }
    
    if(nlhs != 1) {
        mexErrMsgIdAndTxt("MyToolbox:arrayProduct:nlhs",
                "One output required.");
    }
    
    if( !mxIsDouble(prhs[0]) ||
            mxIsComplex(prhs[1])) {
        mexErrMsgIdAndTxt("MyToolbox:arrayProduct:notDouble",
                "Input matrix must be type double.");
    }
    
    if( !mxIsDouble(prhs[1]) ||
            mxIsComplex(prhs[1])) {
        mexErrMsgIdAndTxt("MyToolbox:arrayProduct:notDouble",
                "Input matrix must be type double.");
    }
    
    /* check that number of rows in input argument is 1 */
    if(mxGetM(prhs[0]) != 1 || mxGetM(prhs[1]) != 1) {
        mexErrMsgIdAndTxt("MyToolbox:arrayProduct:notRowVector",
                "Input must be a row vector.");
    }
    
    /* variable declarations here */
    
    double multiplier;      /* input scalar */
    double *inMatrix_1;       /* 1xN input matrix */
    double *inMatrix_2;       /* 1xN input matrix */
    
    mwSize ncols;           /* size of matrix */
    
    double *outMatrix;      /* output matrix */
    
    /* code here */
    
    /* create a pointer to the real data in the first input matrix  */
    inMatrix_1 = mxGetPr(prhs[0]);
    
    /* create a pointer to the real data in the second input matrix  */
    inMatrix_2 = mxGetPr(prhs[1]);
    
    /* get dimensions of the input matrix */
    ncols = mxGetN(prhs[1]);
    
    /* create the output matrix */
    plhs[0] = mxCreateDoubleMatrix(1,1,mxREAL);
    
    /* get a pointer to the real data in the output matrix */
    outMatrix = mxGetPr(plhs[0]);
    
    /* call the computational routine */
    DTW_mink(inMatrix_1,inMatrix_2,outMatrix,ncols);
//     DTW_manhattan(inMatrix_1,inMatrix_2,outMatrix,ncols);
//     DTW_euclidean(inMatrix_1,inMatrix_2,outMatrix,ncols);
}

double distFunc(double x, double y)
{
    
    return (pow((x - y), 2));
    
}
double distFuncM(double x, double y)
{
    
    return fabs(x - y);
    
}
double distFuncC(double x, double y)
{
    
    return fabs(x - y);
    
}

//GDTW Minkowski
void DTW_mink(double *t1, double *t2, double *result, mwSize ncols)
{ 
    int m = ncols;
    int n = ncols;
    
    double cost[m][n];
    
    cost[0][0] = distFuncC(t1[0], t2[0]);
    
    // calculate first row
    for(int i = 1; i < m; i++) {
        double temp=distFuncC(t1[i], t2[0]);
        cost[i][0] = std::max(cost[i-1][0], temp);
        
    }
    
    
    // calculate first column
    for(int j = 1; j < n; j++) {
        double temp=distFuncC(t1[0], t2[j]);
        cost[0][j] = std::max(cost[0][j-1] , temp);
        
    }
      
    // fill matrix.   
    for(int i = 1; i < m; i++) {
        for(int j = 1; j < n; j++) {
            
            double mp1 = std::max(cost[i-1][j],distFuncC(t1[i],t2[j]));
            double mp2 = std::max(cost[i][j-1],distFuncC(t1[i],t2[j]));
            double mp3 = std::max(cost[i-1][j-1],distFuncC(t1[i],t2[j]));
            
            double mp=std::min(mp1,mp2);
            cost[i][j] = std::min(mp,mp3);
        }
        
    }
    
    result[0] = cost[m-1][n-1];
}

//GDTW Manhattan
void DTW_manhattan(double *t1, double *t2, double *result, mwSize ncols)
{
    int m = ncols;
    int n = ncols;
    
    // create cost matrix
    double cost[m][n];
    
    cost[0][0] = distFuncM(t1[0], t2[0]);
    // calculate first row
    for(int i = 1; i < m; i++)
    {
        
        cost[i][0] = cost[i-1][0] + distFuncM(t1[i], t2[0]);
        
    }
    // calculate first column
    for(int j = 1; j < n; j++)
    {
        
        cost[0][j] = cost[0][j-1] + distFuncM(t1[0], t2[j]);
        
    }
    // fill matrix
    for(int i = 1; i < m; i++)
    {
        for(int j = 1; j < n; j++)
        {
            double mp = std::min(cost[i-1][j],
                    std::min(cost[i][j-1],
                    cost[i-1][j-1]));
            
            
            cost[i][j] = distFuncM(t1[i],t2[j])+ std::min(cost[i-1][j],std::min(cost[i][j-1], cost[i-1][j-1]));
        }
    }
    
    result[0] = cost[m-1][n-1];
}

//taken from http://bytefish.de/blog/dynamic_time_warping/
void DTW_euclidean(double *t1, double *t2, double *result, mwSize ncols)
{
    int m = ncols;
    int n = ncols;
    
    // create cost matrix
    double cost[m][n];
    
    
    cost[0][0] = distFunc(t1[0], t2[0]);
    
    // calculate first row
    for(int i = 1; i < m; i++)
    {
        
        cost[i][0] = cost[i-1][0] + distFunc(t1[i], t2[0]);
          
    }
    // calculate first column
    for(int j = 1; j < n; j++)
    {
        cost[0][j] = cost[0][j-1] + distFunc(t1[0], t2[j]);
    }
    // fill matrix
    for(int i = 1; i < m; i++)
    {
        for(int j = 1; j < n; j++)
        {
            double mp = std::min(cost[i-1][j],
                    std::min(cost[i][j-1],
                    cost[i-1][j-1]));
            
            cost[i][j] = distFunc(t1[i],t2[j])+ std::min(cost[i-1][j],std::min(cost[i][j-1], cost[i-1][j-1]));
            
        }
    } 
    result[0] = cost[m-1][n-1];
}

