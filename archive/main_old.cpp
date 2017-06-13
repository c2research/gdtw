/***********************************************************************/
/************************* DISCLAIMER **********************************/
/***********************************************************************/
/**
/**                                                                   **/
/** Unless stated otherwise, all software is provided free of charge. **/
/** As well, all software is provided on an "as is" basis without     **/
/** warranty of any kind, express or implied. Under no circumstances  **/
/** and under no legal theory, whether in tort, contract,or otherwise,**/
/** shall be liable to you or to any other **/
/** person for any indirect, special, incidental, or consequential    **/
/** damages of any character including, without limitation, damages   **/
/** for loss of goodwill, work stoppage, computer failure or          **/
/** malfunction, or for any and all other damages or losses.          **/
/**                                                                   **/
/** If you do not agree with these terms, then you you are advised to **/
/** not use this software.                                            **/
/***********************************************************************/
/***********************************************************************/
//VERSION OF SYSTEM TO IMPLEMENT GDTW DISTANCES


#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include <iostream>
#include "online.h"
#include <sys/time.h>
#include <math.h>
#include <time.h>


using namespace std;


int main()
{
    int arg1,arg2,arg3,arg5,arg6,arg7,arg8;
    double arg4;
    string filename;
    string filenameMatrix;
    string queryFilename;
    string filePath;
    cout<<"Load data file, Enter filename followed by N L"<<endl;
    cin>>filename;
    cin>>arg2;//N
    cin>>arg3;//L
    cout<<"Enter filename with full path for cost matrix"<<endl;
    cin>>filenameMatrix;//L
    int queryLength;

    //read data
    onlineOperation *onlineObj=new onlineOperation(0.2,arg2,arg3,filenameMatrix.c_str());// ST, number of time series in original file, length of time series
    onlineObj->readTimeSeries(filename.c_str(),false);
    cout<<"Time series read"<<endl;



    cout<<"Commands to run, Enter 1 to find GDTW between 2 pair of sequences, 2 to find 1NN using GDTW distances, 4 to exit "<<endl;

    //cout<<"preprocess filename"<<endl;
    clock_t start1;
    cin>>arg1;
    while(arg1!=4)
    {
        if(arg1==1) //calculate GDTW between two sequences
        {


            cout<<"seq1 start end seq2 start end method"<<endl;
            cout<<"method, EUCLIDEAN=0, TAXICAB=1,CHEBYSHEV=2, DTW_ED=3, DTW_MD=4,DTW_CD=5"<<endl;
            cin>>arg2;//seq1
            cin>>arg3;//start
            cin>>arg4;//end
            cin>>arg5;//seq2
            cin>>arg6;//start
            cin>>arg7;//end
            cin>>arg8;//method
            
            onlineObj->distSequences(arg2,arg3,arg4,arg5,arg6,arg7,arg8);

        }
        else if(arg1==2)
        {

            cout<<"Enter 1 to load query from time series, 2 from query file"<<endl;
            cin>>arg2;
            if(arg2==1) //load from time series
            {
                cout<<"<queryTS, start, end, method (EUCLIDEAN=0, TAXICAB=1,CHEBYSHEV=2, DTW_ED=3, DTW_MD=4,DTW_CD=5, ED-ED=6, MD-MD=7, CD-CD=8)"<<endl;
                cin>>arg2;
                cin>>arg3;
                cin>>arg4;
               onlineObj->loadQueryTS(arg2,arg3,arg4);
               cin>>arg8;//method


            }
            else
            {
                cout<<"<queryFile, query length, method (EUCLIDEAN=0, TAXICAB=1,CHEBYSHEV=2, DTW_ED=3, DTW_MD=4,DTW_CD=5)"<<endl;
                cin>>queryFilename;
                cin>>queryLength;
                onlineObj->readQueryFile(queryLength,queryFilename.c_str(),false);
                cin>>arg8;//method
            }



            start1 = clock();
          // cout<<"Spring Calculation"<<endl;
            onlineObj->springCalculation(arg8,1,1);//onlineObj->tempQ.size()-1);
           cout << "Spring " << (clock()-start1)/double(CLOCKS_PER_SEC) << "s]" << endl;
        }
        else
            cout<<"Invalid choice"<<endl;
        cout<<"Commands to run, Enter 1 to find GDTW between 2 pair of sequences, 2 to find 1NN using GDTW distances, 4 to exit "<<endl;
        cin>>arg1;
    }

}


