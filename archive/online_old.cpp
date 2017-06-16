
#include "online.h"
#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include <iostream>
#include <cmath>
#include <time.h>
#include <map>
#include <set>
#include <algorithm>

#define INF 1e20       //Pseudo Infitinte number for this code
#define dist(x,y) ((x-y)*(x-y))

using namespace std;


const int EUCLIDEAN=0;
const int TAXICAB=1;
const int CHEBYSHEV=2;
const int DTW_ED=3;
const int DTW_MD=4;
const int DTW_CD=5;
//constructor passing the query data length
//ST, Ndata number of time series in data file, L is length of time series
onlineOperation::onlineOperation(double STnew, int Ndata, int Ldata, const char * matrixFile)
{
   ST=STnew;        //new ST`
   N=Ndata;
   L=Ldata;
   warpFilename=matrixFile;
   globalDist=INF;

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

//type =true means to normalize, for ONEX, type =false mean run Spring, do not normalize in spring
int onlineOperation::readTimeSeries(const char* Filename, bool type)
{
    inputFile.open(Filename); //Opening the file
    char tempC;
    if(!inputFile)
    {
        cout << "there's something wrong with the file!\n"; //Throw exception
        return -1;
    }
    else
    {
        //read the input file into time series structure
        //Read numbers from file into the array
        for(int i = 0; i < N; i++)
        {
            for(int j=0; j<L;j++)
            {
                //normalize at the same time
                inputFile>>timeSeries[i][j];
                if(type==true)
                 {

                    timeSeries[i][j]=(timeSeries[i][j]-min)/(max-min);
                    inputN<<timeSeries[i][j];
                    inputN<<" ";
                }

            }
            inputN<<endl;
        }
        inputFile.close();
        inputN.close();

        return 1;
    }

}

//reads the query file
//type =true means normalize and false means running for Spring don't normalize
void onlineOperation::readQueryFile(int m,const char * Filename, bool type)
{
    tempQ.clear();
    // Array for keeping the query data
    Q = (double *)malloc(sizeof(double)*m);
    queryLength=m;
    queryFile.open(Filename);
    //read the query data and normalize at the same time
    for(int i=0;i<m;i++)
    {
        queryFile>>Q[i];
        if(type==true)
        {
            Q[i]=(Q[i]-min)/(max-min);
            queryN<<Q[i];
            queryN<<" ";
        }
        tempQ.push_back(Q[i]);
    }
        queryN.close();
    queryFile.close();
}

//finds k similar time series, query Flag=true query sequence is given
//timeFlag=true time interval Exact, false means Any

void onlineOperation::loadQueryTS(int TSid, int startTS, int endTS)
{
    //load subsequence if TS in query object
    tempQ.clear();
    for(int i=startTS;i<=endTS;i++)
    {
        tempQ.push_back(timeSeries[TSid][i]);
        cout<<timeSeries[TSid][i]<<" ";
    }
}

//taken from http://bytefish.de/blog/dynamic_time_warping/
double onlineOperation::simpleDTW(const vector<double>& t1, const vector<double>& t2, bool matrixFlag)
{
    double temp;
    int m = t1.size();
    int n = t2.size();
    int warpcount = 0; // Count warpings
    pathLength=0;     //length of warping path
    // create cost matrix
    double cost[m][n];
    double costValue;

    cost[0][0] = distFunc(t1[0], t2[0]);
    costValue=cost[0][0];
//     temp=cost[0][0];
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


    //if warpings needs to vbe done then do this
    if(matrixFlag)
    {

        //print the matrix in file
        costMatrix.open(warpFilename,std::ofstream::trunc);
        for(int i = 0; i < m; i++)
        {
            for(int j = 0; j < n; j++)
            {

                costMatrix<<cost[i][j]<<" ";
            }
            costMatrix<<endl;
        }
        costMatrix.close();
        //calculate the warps
        int i=m-1;
        int j=n-1;
        double diagonal;
        double left;
        double up;
        cout<<"Warp path ";
        while(i>0 && j>0)
        {
            diagonal=cost[i-1][j-1];
            left=cost[i][j-1];
            up=cost[i-1][j];
            //print warping path too
            cout<<" i"<<i+1<<" j"<<j+1<<"|";
            pathLength++;
            if(i!=j)    //different value of i maped to j
            {
                warpcount++;
            }
            if((diagonal<=left) && (diagonal<=up))        //diagonal has lowest value move diagonally
            {
                i=i-1;
                j=j-1;
            }
            else if((left<=diagonal) && (left<=up))
            {
                //warpcount++;
                j=j-1;
            }
            else if((up<=diagonal) && (up<=left))
            {
                //warpcount++;
                i=i-1;
            }
            else
                cout<<"Something wrong";


        }
        if((i!=0) || (j!=0))        //not reached first cell
        {
            while(i!=0)
            {
                if(i!=j)    //different value of i maped to j
                {
                    warpcount++;
                }
                cout<<" i"<<i+1<<" j"<<j+1<<"|";
                pathLength++;
                i=i-1;

            }
            while(j!=0)
            {
                if(i!=j)    //different value of i maped to j
                {
                    warpcount++;
                }
                cout<<" i"<<i+1<<" j"<<j+1<<"|";
                pathLength++;
                j=j-1;

            }
            cout<<" i "<<i+1<<" j "<<j+1<<"|";
            pathLength++;


        }
        else
        {
            cout<<" i "<<i+1<<" j "<<j+1<<"|";
            pathLength++;
        }

        warpC=warpcount;
    }
    costValue=cost[m-1][n-1];
    //delete cost;
    return costValue;


}
double onlineOperation::simpleDTWM(const vector<double>& t1, const vector<double>& t2, bool matrixFlag)
{
    double temp;
    int m = t1.size();
    int warpcount = 0; // Count warpings
    pathLength=0;     //length of warping path
        int n = t2.size();

        // create cost matrix
        double cost[m][n];

        cost[0][0] = distFuncM(t1[0], t2[0]);
   //     temp=cost[0][0];
        // calculate first row
        for(int i = 1; i < m; i++)
        {

            cost[i][0] = cost[i-1][0] + distFuncM(t1[i], t2[0]);
     //       temp=cost[i][0];
        }
        // calculate first column
        for(int j = 1; j < n; j++)
        {

            cost[0][j] = cost[0][j-1] + distFuncM(t1[0], t2[j]);
       //     temp=cost[0][j];
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

   //     temp=cost[m-1][n-1];
        //print the matrix in file
        if(matrixFlag)
        {
            costMatrix.open(warpFilename,std::ofstream::trunc);
            for(int i = 0; i < m; i++)
            {
                for(int j = 0; j < n; j++)
                {

                    costMatrix<<cost[i][j]<<" ";
                }
                costMatrix<<endl;
            }
            costMatrix.close();
            //calculate the warps
            int i=m-1;
            int j=n-1;
            double diagonal;
            double left;
            double up;
            cout<<"Warp path ";
            while(i>0 && j>0)
            {
                diagonal=cost[i-1][j-1];
                left=cost[i][j-1];
                up=cost[i-1][j];
                //print warping path too
                cout<<" i "<<i+1<<" j "<<j+1<<"|";
                pathLength++;
                if(i!=j)    //different value of i maped to j
                {
                    warpcount++;
                }
                if((diagonal<=left) && (diagonal<=up))        //diagonal has lowest value move diagonally
                {
                    i=i-1;
                    j=j-1;
                }
                else if((left<=diagonal) && (left<=up))
                {
                 //   warpcount++;
                    j=j-1;
                }
                else if((up<=diagonal) && (up<=left))
                {
                   // warpcount++;
                    i=i-1;
                }
                else
                    cout<<"Something wrong";

            }
            if((i!=0) || (j!=0))        //not reached first cell
            {
                while(i!=0)
                {
                    if(i!=j)    //different value of i maped to j
                    {
                        warpcount++;
                    }
                    cout<<" i "<<i+1<<" j "<<j+1<<"|";
                    pathLength++;
                    i=i-1;

                }
                while(j!=0)
                {
                    if(i!=j)    //different value of i maped to j
                    {
                        warpcount++;
                    }
                    cout<<" i "<<i+1<<" j "<<j+1<<"|";
                    pathLength++;
                    j=j-1;

                }
                cout<<" i "<<i+1<<" j "<<j+1<<"|";
                pathLength++;


            }
            else
            {
                cout<<" i "<<i+1<<" j "<<j+1<<"|";
                pathLength++;
            }
        }
  //      cout<<"Warps "<<warpcount<<endl;
        warpC=warpcount;
        return cost[m-1][n-1];
}
double onlineOperation::simpleDTWC(const vector<double>& t1, const vector<double>& t2, bool matrixFlag)
{
    int m = t1.size();
    int n = t2.size();

    double cost[m][n];
    int warpcount = 0; // Count warpings
    pathLength=0;

    cost[0][0] = distFuncC(t1[0], t2[0]);
  //  cout<<cost[0][0]<<endl;
    // calculate first row
    for(int i = 1; i < m; i++) {
        double temp=distFuncC(t1[i], t2[0]);
        cost[i][0] = std::max(cost[i-1][0], temp);
        //cout<<cost[i][0]<<" ";
    }

  //  cout<<endl;
    // calculate first column
    for(int j = 1; j < n; j++) {
        double temp=distFuncC(t1[0], t2[j]);
        cost[0][j] = std::max(cost[0][j-1] , temp);
        //cout<<cost[0][j]<<" ";
    }

 //   cout<<endl;
    // fill matrix. If using dropout, keep tabs on min cost per row.


    for(int i = 1; i < m; i++) {
        for(int j = 1; j < n; j++) {

            double mp1 = std::max(cost[i-1][j],distFuncC(t1[i],t2[j]));
            double mp2 = std::max(cost[i][j-1],distFuncC(t1[i],t2[j]));
            double mp3 = std::max(cost[i-1][j-1],distFuncC(t1[i],t2[j]));

            double mp=std::min(mp1,mp2);
            cost[i][j] = std::min(mp,mp3);
   //         cout<<cost[i][j]<<" ";
        //    if (cost[i][j] != cost[i-1][j-1])
          //      warpcount++;
        }
     //   cout<<endl;
    }
 //   cout<<"Warps "<<warpcount<<endl;
    if(matrixFlag)
    {
        //print the matrix in file
        costMatrix.open(warpFilename,std::ofstream::trunc);
        for(int i = 0; i < m; i++)
        {
            for(int j = 0; j < n; j++)
            {

                costMatrix<<cost[i][j]<<" ";
            }
            costMatrix<<endl;
        }
        costMatrix.close();
        //calculate the warps
        //calculate the warps
        int i=m-1;
        int j=n-1;
        double diagonal;
        double left;
        double up;
        cout<<"Warp path ";
        while(i>0 && j>0)
        {
            diagonal=cost[i-1][j-1];
            left=cost[i][j-1];
            up=cost[i-1][j];
            //print warping path too
            cout<<" i "<<i+1<<" j "<<j+1<<"|";
            pathLength++;
            if(i!=j)    //different value of i maped to j
            {
                warpcount++;
            }
            if((diagonal<=left) && (diagonal<=up))        //diagonal has lowest value move diagonally
            {
                i=i-1;
                j=j-1;
            }
            else if((left<=diagonal) && (left<=up))
            {
              //  warpcount++;
                j=j-1;
            }
            else if((up<=diagonal) && (up<=left))
            {
              //  warpcount++;
                i=i-1;
            }
            else
            {

                cout<<"Something wrong";
                cout<<"diagonal"<<diagonal<<" left "<<left<<" up "<<up<<endl;
                return 0;
            }

        }
        if((i!=0) || (j!=0))        //not reached first cell
        {
            while(i!=0)
            {
                if(i!=j)    //different value of i maped to j
                {
                    warpcount++;
                }
                cout<<" i "<<i+1<<" j "<<j+1<<"|";
                pathLength++;
                i=i-1;

            }
            while(j!=0)
            {
                if(i!=j)    //different value of i maped to j
                {
                    warpcount++;
                }
                cout<<" i "<<i+1<<" j "<<j+1<<"|";
                pathLength++;
                j=j-1;

            }
            cout<<" i "<<i+1<<" j "<<j+1<<"|";
            pathLength++;


        }
        else
        {
            cout<<" i "<<i+1<<" j "<<j+1<<"|";
            pathLength++;
        }
        //warpC=warpcount;
    }
    warpC=warpcount;
    return cost[m-1][n-1];
}


void onlineOperation::springCalculation(int method, int k, int startPoint=1)
{
    double bsf=INF;
    int bsfIndex;       //index of bsf time series
    double currentDist=INF;
    int bestIntervalS;
    int bestIntervalE;
    vector<double> temp;    //temporary subsequence
    int kbestCount=0;
    int warpCount=0;
    double chebyshevDist=0;


    double dist=0;

    //get each subsequence
    for(int j=startPoint;j<L;j++)
    {
        /*
        if(method==EUCLIDEAN || method==TAXICAB || method==CHEBYSHEV)
        {
            if(tempQ.size()!=j-1)
                continue;
        }
        */
        cout<<"Level "<<j<<endl;
        for (int l=0, m=l+j; m<L; l++, m++)
        {
             for (int i=0;i<N;i++)
             {
                 //copy  the subsequence in temp
                 for(int p=0;p<=m-l;p++)
                     temp.push_back(timeSeries[i][l+p]);
                 //check if method is any point wise
                 if(method==EUCLIDEAN || method==TAXICAB || method==CHEBYSHEV)
                 {
                     if(temp.size()==tempQ.size())
                     {
                         //compute point wise distance
                         for(int i=0;i<temp.size();i++)
                         {
                             if(method==EUCLIDEAN)
                                dist+=distFunc(temp[i],tempQ[i]);
                             else if(method==TAXICAB)
                                 dist+=distFuncM(temp[i],tempQ[i]);
                             else if(method==CHEBYSHEV)
                             {

                                 chebyshevDist=distFuncC(temp[i],tempQ[i]);
                                 if(dist<chebyshevDist)
                                     dist=chebyshevDist;
                               //  dist+=distFuncC(temp[i],tempQ[i]);
                             }
                         }
                         if(method==EUCLIDEAN)
                            currentDist=sqrt(dist);
                         else
                             currentDist=dist;
                         if(currentDist<bsf)
                         {
                             bsf=currentDist;
                             bsfIndex=i;        //best time series index
                             bestIntervalS=l;   //record the best interval
                             bestIntervalE=m;
                         }
                         //clear the temp array
                         temp.clear();
                         dist=0;
                     }
                     else
                         temp.clear();
                 }
                 else
                 {

                     if(method==DTW_ED)
                        currentDist=sqrt(simpleDTW(temp,tempQ, false));
                     else if(method==DTW_MD)
                        currentDist=simpleDTWM(temp,tempQ, false);
                     else if(method==DTW_CD)
                        currentDist=simpleDTWC(temp,tempQ, false);
                     if(currentDist<bsf)
                     {
                         bsf=currentDist;
                         bsfIndex=i;        //best time series index
                         bestIntervalS=l;   //record the best interval
                         bestIntervalE=m;
                      //   warpCount=warpC;   //update the warp count of this best sequence
                     }
                     //clear the temp array
                     temp.clear();

                 }
             }

         }
     }

    //print the closest sequence
    cout<<"Closest sequence <";
    for(int i=bestIntervalS;i<=bestIntervalE;i++)
        cout<<timeSeries[bsfIndex][i]<<",";
    cout<<">"<<endl<<endl;
    cout<<"TS "<<bsfIndex<<" Interval "<<bestIntervalS<<" "<<bestIntervalE<<" Dist "<<bsf<<endl;
    /*
    //print the right matrix
    for(int p=bestIntervalS;p<=bestIntervalE;p++)
        temp.push_back(timeSeries[bsfIndex][p]);
    if(method==DTW_ED)
    {
       currentDist=sqrt(simpleDTW(temp,tempQ, true));
       warpCount=warpC;
       cout<<"Warp count "<<warpCount<<endl;
    }
    else if(method==DTW_MD)
    {
       currentDist=simpleDTWM(temp,tempQ, true);
       warpCount=warpC;
       cout<<"Warp count "<<warpCount<<endl;
    }
    else if(method==DTW_CD)
    {
       currentDist=simpleDTWC(temp,tempQ, true);
       warpCount=warpC;
       cout<<"Warp count "<<warpCount<<endl;
    }
*/

    temp.clear();


}

double onlineOperation::distSequences(int seq1, int start1, int end1,int seq2, int start2, int end2, int method)
{
    //get the two slices
    vector<double> temp1;
    vector<double> temp2;
    int count1=end1-start1+1;
    int count2=end2-start2+1;
    double sum=0;
    int warpCount=0;
    double distanceE=0;

    for(int i=start1;i<=end1;i++)
    {
        temp1.push_back(timeSeries[seq1][i]);
    }
    for(int i=start2;i<=end2;i++)
    {
        temp2.push_back(timeSeries[seq2][i]);
    }
    //calculate distance
    if(method==EUCLIDEAN)
    {
        if(count1!=count2)
        {
            cout<<"Two sequences are not of same length"<<endl;
            return -1;
        }
        //calculate Euclidean
        for(int i=0;i<count1;i++)
        {
            sum+=distFunc(temp1[i],temp2[i]);
        }
        distanceE=sqrt(sum);
        cout<<"Euclidean distance "<<distanceE<<endl;
        return distanceE;
    }
    else if(method==TAXICAB)
    {
        if(count1!=count2)
        {
            cout<<"Two sequences are not of same length"<<endl;
            return -1;
        }
        for(int i=0;i<count1;i++)
        {

            sum+=distFuncM(temp1[i],temp2[i]);
        }
        distanceE=sum;
        cout<<"Manhattan distance "<<distanceE<<endl;
        return distanceE;
    }
    else if(method==CHEBYSHEV)
    {
        if(count1!=count2)
        {
            cout<<"Two sequences are not of same length"<<endl;
            return -1;
        }
        for(int i=0;i<count1;i++)
        {

            distanceE=distFuncC(temp1[i],temp2[i]);
            if(sum<distanceE)
                sum=distanceE;
        }
        distanceE=sum;
        cout<<"Cheby distance "<<distanceE<<endl;
        return distanceE;
    }
    else if(method==DTW_ED)
    {
        distanceE=sqrt(simpleDTW(temp1,temp2,true));
        cout<<endl;
        cout<<"DTW-ED distance: "<<distanceE<<endl;
        cout<<"Warp count "<<warpC<<endl;
        cout<<"Length of warping path "<<pathLength<<endl;
        return distanceE;
    }
    else if(method==DTW_MD)
    {
        distanceE=simpleDTWM(temp1,temp2,true);
        cout<<endl;
        cout<<"DTW-MD distance: "<<distanceE<<endl;
        cout<<"Warp count "<<warpC<<endl;
        cout<<"Length of warping path "<<pathLength<<endl;
        return distanceE;
    }
    else if(method==DTW_CD)
    {
        distanceE=simpleDTWC(temp1,temp2,true);
        cout<<endl;
        cout<<"DTW-CD distance: "<<distanceE<<endl;
        cout<<"Warp count "<<warpC<<endl;
        cout<<"Length of warping path "<<pathLength<<endl;
        return distanceE;
    }
    else
        cout<<"Not valid distance measure"<<endl;
}
