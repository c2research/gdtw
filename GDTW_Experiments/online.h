#ifndef ONLINE_H
#define ONLINE_H

#endif // ONLINE_H
#include <vector>
#include <set>
#include <string>
#include <map>
#include <fstream>

using namespace std;

const int otimeSeriesN=2;      //dataset
const int otimeSeriesL=16;

struct osubsequence
{
    int id;     //time series id
    int startT; //start interval of this subsequence
    int endT;   //end interval of this subsequence

};


class onlineOperation
{
public:
    onlineOperation(double STnew, int Ndata, int Ldata, const char *matrixFile);
    ~onlineOperation();

    void kSimilar(bool queryFlag, int k,bool timeFlag,int startTime, int endTime);
    void readQueryFile(int m, const char *Filename, bool type);
    int readTimeSeries(const char *Filename, bool type);
    double dtw(double* A, double* B, double *cb, int m, int r, double bsf);
    double simpleDTW(const vector<double>& t1, const vector<double>& t2, bool matrixFlag);
    void springCalculation(int method, int k, int startPoint);

    double kSimilarnew(int length, bool threshold, int method);
    double distSequences(int seq1, int start1, int end1,int seq2, int start2, int end2, int method);
    double simpleDTWM(const vector<double>& t1, const vector<double>& t2, bool matrixFlag);
    double simpleDTWC(const vector<double>& t1, const vector<double>& t2, bool matrixFlag);

    void loadQueryTS(int TSid, int startTS, int endTS);

    //variables
    ifstream inputTimeFile;         //input Time data file
    ifstream queryFile;         //input query file
    ifstream groupFile;         //input Group file
    ifstream inputFile;         //input time series data file

    ofstream inputN;            //normalized inputfile
    ofstream queryN;            //normalized query file
    ofstream costMatrix;            //warped cost matrix


    int N;  //number of time series
    int L;  //length of time series
    double ST;            //Similarity threshold
    double min;         //min value in data
    double max;         //max value in data for normalization
    int centroidDist[otimeSeriesN][otimeSeriesN];     //Dc distance

    double timeSeries[otimeSeriesN][otimeSeriesL];
    double *Q;      //query data
    int queryLength;          //size of query data
    vector<double> tempQ;       //vector to hold query
    int warpC;                  //warp count
    const char* warpFilename;
    int pathLength;//length of warping path
    string GroupFilePath;       //path for groups file
    string timeFilePath;        //path for Time file

    double globalDist;



};
