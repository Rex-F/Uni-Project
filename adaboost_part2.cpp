#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <cmath>
#include <math.h>

using namespace std;


int main()
{
    string filename;
    int numBoostingRounds;
    float alpha;

    cout << "Enter the number of boosting rounds: " << endl;
    cin >> numBoostingRounds;
    //ifstream in(filename.c_str());

    ifstream in("dataset.csv");

    string line, field;

    vector<string> variableNames;

    vector<vector<vector<float> > > dataArrays;  // create 3D array


    vector< vector<float> > boostingArray;

    getline (in, line);
    stringstream ss(line);

    while (getline(ss, field, ',')) {
        variableNames.push_back(field);
    }



    vector<vector<float> > singleArray;

    while (getline (in,line) )    // get next line in file
    {

        string val;
        vector<float> row;
        stringstream ss(line);


        while (getline(ss, val,','))  // break line into comma delimitted fields
        {
            row.push_back(stof(val));  // add each field to the 1D array
        }

        singleArray.push_back(row);  // add the 1D array to the 2D array
    }
    dataArrays.push_back(singleArray);

    int directionArray[2] = {1, -1};

    int numFeatures = dataArray[0].size()-1;
    int numDatapoints = dataArray.size();

    float initialWeight = 1.0/numDatapoints;
    float weightArray[numDatapoints];
    for (int h=0; h<numDatapoints; ++h) {
        weightArray[h] = initialWeight;
    }

for (int boostNum=0; boostNum<numBoostingRounds; boostNum++) {

    float bestThreshold = 0.0;
    int bestDirection = 1;
    float minimumError = 10.0;
    int bestFeature = 9999;


    //cout << numDatapoints << endl;
    //cout << initialWeight << endl;



    for(int i=0; i<numFeatures; ++i) {   //first loop, cycle through features

        for(int j=0; j<2; ++j) {  //second loop, need our cycle to test both directions

            for (int k=0; k<numDatapoints; ++k) { //third loop, cycle through datapoint values for each feature

                int tempArray[numDatapoints];

                if (directionArray[j] == 1) {
                        for(int m =0; m<numDatapoints; ++m) {
                            tempArray[m] = (dataArray[m][i] >= dataArray[k][i]);
                            //cout << "compare " << dataArray[m][i] << " to threshold " << dataArray[k][i] << endl;
                            //cout << "1s for trues, 0s for falses "<< tempArray[m] << endl;
                        }
                }


                if (directionArray[j] == -1) {
                        for(int m =0; m<numDatapoints; ++m) {
                            tempArray[m] = (dataArray[m][i] < dataArray[k][i]);
                        }
                }

                for (int n=0; n<numDatapoints; ++n) {
                    if (tempArray[n] == 0) {
                        tempArray[n] = -1;
                    }
                }
                //cout << endl;

                int comparisonArray[numDatapoints];

                for (int p =0; p<numDatapoints; ++p) {
                    if(tempArray[p] == dataArray[p][numFeatures]){
                        comparisonArray[p] = 0;
                    }
                    else {
                        comparisonArray[p] = 1;
                    }
                }

                float partialSum;
                float errorProportion = 0.0;
                for (int q =0; q<numDatapoints; ++q) {
                    partialSum = comparisonArray[q]*weightArray[q];
                    errorProportion = errorProportion + partialSum;
                   // cout << "ComparisonArray number " << comparisonArray[q] << endl; //0s are good, 1s are misclassified
                    //cout << "WeightArray is " << weightArray[q] << endl;
                   // cout << "PartialSum is " << partialSum << endl;
                   // cout << "ErrorProportion is " << errorProportion << endl;
                }

                if (errorProportion < minimumError) {
                    minimumError = errorProportion;
                    bestThreshold = dataArray[k][i];
                    bestDirection = directionArray[j];
                    bestFeature = i;

                }


            }//end datapoints loop
        } //end direction loop

    } //end feature loop

    /*cout << "minimum error is " << minimumError << endl;
    cout << "best threshold is " << bestThreshold << endl;
    cout << "best direction is " << bestDirection << endl;
    cout << "best feature is " << bestFeature << endl;
    */
    //next need to calculate alpha of classifier found

    //while in a boosting round that resets the row every boosting loop
    if (minimumError == 0.0) {minimumError = 0.0000001;}

    float rowHelper[] = {minimumError, bestThreshold, bestDirection, bestFeature};
    vector<float> boostingRow(rowHelper, rowHelper+4);


    boostingArray.push_back(boostingRow);



    alpha = 0.5*log((1.0 - minimumError)/(minimumError));



    boostingArray[boostNum].push_back(alpha);

    /*for (int i = 0; i<5; i++) {
        cout << boostingArray[0][i] << endl; //replace 0 with appropriate boosting round counter variable
    }
    */
    //using the best threshold we've found to classify our dataset
    int predictArray[numDatapoints];

    for (int i=0; i<numDatapoints; i++) {
        if (bestDirection == -1) {
            if (dataArray[i][bestFeature] < bestThreshold) {
                predictArray[i] = 1;
            }

            else {
                predictArray[i] = -1;
            }
        }
        else {
           if (bestDirection == 1) {
                if (dataArray[i][bestFeature] < bestThreshold) {
                    predictArray[i] = -1;
                }
                else {
                    predictArray[i] = 1;
                }

             }

         }
     }
    /*cout << "Prediction Array" << endl;
    for (int i=0; i< numDatapoints; i++) {
        cout << predictArray[i] << " ";

    }*/
    /*int lossArray[numDatapoints];

    for (int i=0; i< numDatapoints; i++) {
        if (predictArray[i] == dataArray[i][numFeatures]) {
            lossArray[i] = 0;
        }
        else {
            lossArray[i] = 1;
        }
    }
     cout << "Loss Array" << endl;
    for (int i=0; i< numDatapoints; i++) {
        cout << lossArray[i] << " ";

    }
    */
    float sumWeights = 0;
    for (int i=0; i< numDatapoints; i++) {  //updating the weights, all this takes is actual vs predicted and alpha.
        weightArray[i] = weightArray[i] * exp((-alpha)*dataArray[i][numFeatures]*predictArray[i]);
        sumWeights = sumWeights + weightArray[i];
    }
    for (int i=0; i< numDatapoints; i++) {   //normalise the weights
        weightArray[i] = weightArray[i]/sumWeights;

    }



 }
    cout << "Minimum Error | Best Threshold | Best Direction | Best Feature | Alpha" << endl;
  for (int i=0; i<numBoostingRounds; i++) {
        for (int j=0; j<5; j++) {
            cout << boostingArray[i][j] << "\t\t";
        }
      cout << endl;
  }


  ifstream in2("dataset.csv");
  string line2, field2;

    vector<string> variableNames2;

    vector< vector<float> > data2Array;
    getline (in2, line2);
    stringstream ss2(line2);

    while (getline(ss2, field2, ',')) {
        variableNames2.push_back(field2);
    }





    while (getline (in2,line2) )    // get next line in file
    {

        string val;
        vector<float> row;
        stringstream ss2(line2);

        while (getline(ss2, val,','))  // break line into comma delimitted fields
        {
            row.push_back(stof(val));  // add each field to the 1D array
        }

        data2Array.push_back(row);  // add the 1D array to the 2D array
    }

    numFeatures = data2Array[0].size()-1;
    numDatapoints = data2Array.size();

    for (int i=0; i<numDatapoints; i++) {
        for (int j=0; j<numFeatures; j++) {
            cout << data2Array[i][j] << " ";
        }
        cout << endl;
    }

    int predict2Array[numDatapoints];
    float classifierSumArray[numDatapoints];

    for (int i=0; i<numDatapoints; i++) {
        classifierSumArray[i] = 0.0;
    }
    int finalClassification[numDatapoints];

    //boosting array 0 minimumError, 1 bestThreshold, 2 bestDirection, 3 bestFeature, 4 alpha
    for (int i=0; i<numBoostingRounds; i++) {
        for (int j=0; j<numDatapoints; j++) {
            if (boostingArray[i][2] == -1) {
                if (data2Array[j][boostingArray[i][3]] < boostingArray[i][1]) {
                    predict2Array[j] = 1;
                }

                else {
                    predict2Array[j] = -1;
                }
            }
            else {
                if (boostingArray[i][2] == 1) {
                     if (data2Array[j][boostingArray[i][3]] < boostingArray[i][1]) {
                        predict2Array[j] = -1;
                    }
                    else {
                        predict2Array[j] = 1;
                    }

                }

            }

        }
        for (int k=0; k<numDatapoints; k++)
        classifierSumArray[k] =  classifierSumArray[k] + boostingArray[i][4] * predict2Array[k];
    }
    for (int m=0; m<numDatapoints; m++) {
        if (classifierSumArray[m] >= 0) {
            finalClassification[m] = 1;
        }
        else {
             finalClassification[m] = -1;

        }
        cout << finalClassification[m] << " ";
    }



} //end main








