#include <mex.h>

#include "TimeSeries.hpp"
#include "distance/Distance.hpp"

/**
*  @brief function to call gdtw c++ code from matlab
*
*  this function allows you to access c++ gdtw functions
*  via matlab for scripting and automation. Example usage:
*
*  | dist = gdtw(inMatrix_1, inMatrix_2, distanceName)
*
*  @param first timeseries to compute the distance between
*  @param sencond timeseries to compute the distance between
*  @param the name for the base distance to use.
*      if not defined, defaults to 'euclidean'.
*
*  @return sets the output to the resulting distance
*
*  @throw GenexException
*/
void mexFunction(int nlhs, mxArray *plhs[],
        int nrhs, const mxArray *prhs[])
{

    if(nrhs < 2) {
        mexErrMsgIdAndTxt("MyToolbox:arrayProduct:nrhs",
                "At least two inputs required.");
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

    if( nrhs > 2 && !mxIsChar(prhs[2]) ) {
        mexErrMsgIdAndTxt("MyToolbox:arrayProduct:notChar",
                "Distance type must be a string.");
    }

    /* check that number of rows in input argument is 1 */
    if(mxGetM(prhs[0]) != 1 || mxGetM(prhs[1]) != 1) {
        mexErrMsgIdAndTxt("MyToolbox:arrayProduct:notRowVector",
                "Input must be a row vector.");
    }

    /* variable declarations here */
    data_t *inMatrix_1;     /* 1xN input matrix */
    data_t *inMatrix_2;     /* 1xN input matrix */
    data_t *outMatrix;      /* output matrix    */

    /* create a pointer to the real data in the first input matrix  */
    inMatrix_1 = mxGetPr(prhs[0]);
    const TimeSeries x_1(mxGetPr(prhs[0]), mxGetN(prhs[0]));

    /* create a pointer to the real data in the second input matrix  */
    inMatrix_2 = mxGetPr(prhs[1]);
    const TimeSeries x_2(mxGetPr(prhs[1]), mxGetN(prhs[1]));

    /* create string for distance name */
    std::string in_name;
    if (nrhs > 2) {
        in_name = mxArrayToString(prhs[2]);
    } else {
        in_name = "euclidean";
    }

    /* create the output matrix */
    plhs[0] = mxCreateDoubleMatrix(1,1,mxREAL);

    /* get a pointer to the real data in the output matrix */
    outMatrix = mxGetPr(plhs[0]);

    /* get distance */
    const dist_t distance = getDistance(in_name + "_dtw");

    /* perform gdtw */
    outMatrix[0] = distance(x_1, x_2, INF);
}
