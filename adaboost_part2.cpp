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

void thresCompare (int currentLevel, int maxLevel, int pos, float& bestThreshold, int& bestDirection, float& minimumError, int& bestFeature,
	vector<vector<vector<float> > > &dataArrays, vector<vector<float> > singleBoostArray) {

    cout << "thresCompare called" << endl;

    float alpha;


	int directionArray[2] = {1, -1};
	cout << "thresCompare called" << endl;
    int numFeatures = dataArrays[pos][0].size()-2;
    int numDatapoints = dataArrays[pos].size();
    float sumWeights = 0.0;
    cout << "thresCompare called" << endl;
    cout << "numFeatures is " << numFeatures << " numDatapoints is " << numDatapoints << endl;


    for (int i=0; i< numDatapoints; i++) {
        sumWeights = sumWeights + dataArrays[pos][i][numFeatures+1];
    }
    cout << "thresCompare called" << endl;

    for (int i=0; i< numDatapoints; i++) {   //normalise the weights
        dataArrays[pos][i][numFeatures+1] = dataArrays[pos][i][numFeatures+1]/sumWeights;

    }
    cout << "after assigning weights" << endl;

    for(int i=0; i<numFeatures; ++i) {   //first loop, cycle through features

        for(int j=0; j<2; ++j) {  //second loop, need our cycle to test both directions

            for (int k=0; k<numDatapoints; ++k) { //third loop, cycle through datapoint values for each feature

                int tempArray[numDatapoints];

                if (directionArray[j] == 1) {
                        for(int m =0; m<numDatapoints; ++m) {
                            tempArray[m] = (dataArrays[pos][m][i] >= dataArrays[pos][k][i]);
                            //cout << "compare " << dataArrays[pos][m][i] << " to threshold " << dataArrays[pos][k][i] << endl;
                            //cout << "1s for trues, 0s for falses "<< tempArray[m] << endl;
                        }
                }


                if (directionArray[j] == -1) {
                        for(int m =0; m<numDatapoints; ++m) {
                            tempArray[m] = (dataArrays[pos][m][i] < dataArrays[pos][k][i]);
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
                    if(tempArray[p] == dataArrays[pos][p][numFeatures]){
                        comparisonArray[p] = 0;
                    }
                    else {
                        comparisonArray[p] = 1;
                    }
                }

                float partialSum;
                float errorProportion = 0.0;
                for (int q =0; q<numDatapoints; ++q) {
                    partialSum = comparisonArray[q]*dataArrays[pos][q][numFeatures+1];
                    errorProportion = errorProportion + partialSum;
                   // cout << "ComparisonArray number " << comparisonArray[q] << endl; //0s are good, 1s are misclassified
                    //cout << "WeightArray is " << dataArrays[pos][q][numFeatures+1] << endl;
                   // cout << "PartialSum is " << partialSum << endl;
                   // cout << "ErrorProportion is " << errorProportion << endl;
                }

                if (errorProportion < minimumError) {
                    minimumError = errorProportion;
                    bestThreshold = dataArrays[pos][k][i];
                    bestDirection = directionArray[j];
                    bestFeature = i;
                    cout << "minimum error is " << minimumError << endl;
                    cout << "best threshold is " << bestThreshold << endl;
                    cout << "best direction is " << bestDirection << endl;
                    cout << "best feature is " << bestFeature << endl;
                }


            }//end datapoints loop
        } //end direction loop

    }//end feature loop

    cout << "after feature loop ends" << endl;

    if (minimumError == 0.0) {minimumError = 0.0000001;}
    alpha = 0.5*log((1.0 - minimumError)/(minimumError));

    float rowHelper[] = {minimumError, bestThreshold, bestDirection, bestFeature, alpha};
    vector<float> boostingRow(rowHelper, rowHelper+5);


    singleBoostArray.push_back(boostingRow);




    if (currentLevel < maxLevel) {
        dataArrays.resize(pos+3);
        vector<float> row1;
        vector<float> row2;
        vector<vector<float> > singleArray1;
        vector<vector<float> > singleArray2;
        cout << "inside our splitter" << endl;
        cout << "threshold is " << bestThreshold << endl;


        for (int i=0; i<numDatapoints; ++i) {


             if (dataArrays[pos][i][bestFeature] < bestThreshold) {
                cout << "have a hit less than" << endl;
                for (int a=0; a<numFeatures+2; ++a) {
                    row1.push_back(dataArrays[pos][i][a]);
                    cout << "push back " << dataArrays[pos][i][a];
                }
                singleArray1.push_back(row1);
                row1.clear();
                dataArrays[pos+1] = singleArray1;

                cout << "have a hit less than" << endl;
             }
        }
        for (int i=0; i<numDatapoints; ++i) {


             if (dataArrays[pos][i][bestFeature] >= bestThreshold) {
                  cout << "have a hit greater than" << endl;
                for (int a=0; a<numFeatures+2; ++a) {
                    row2.push_back(dataArrays[pos][i][a]);
                    cout << "push back " << dataArrays[pos][i][a];
                }
                singleArray2.push_back(row2);
                row2.clear();
                 cout << "have a hit greater than2" << endl;
                dataArrays[pos+2] = singleArray2;

                    cout << "have a hit greater than3" << endl;
             }
        }

   currentLevel++;

   thresCompare(currentLevel, maxLevel, pos, bestThreshold,
                 bestDirection, minimumError, bestFeature, dataArrays, singleBoostArray);


   thresCompare(currentLevel, maxLevel, pos, bestThreshold,
                 bestDirection, minimumError, bestFeature, dataArrays, singleBoostArray);


    }//end of if we need call more loop





 }//end function


int main()
{
    string filename;

    int maxLevel;
    int numBoostingRounds;
    int currentLevel = 1;

    int pos = 0;  //the position the 2d array we are curretnly accessing is on the 3d array

    cout << "Enter the number of boosting rounds: " << endl;
    cin >> numBoostingRounds;
    //ifstream in(filename.c_str());
    cout << "Enter the number of tree levels (where a level of one is a stump: " << endl;
    cin >> maxLevel;


    ifstream in("dataset.csv");

    string line, field;

    vector<string> variableNames;

    vector<vector<vector<float> > > dataArrays;  // create 3D array


    vector<vector<vector<float> > > boostingArrays;
    vector<vector<float > > singleBoostArray;


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

    int numDatapoints = dataArrays[0].size();
    int numFeatures = dataArrays[0][0].size()-1;


    float initialWeight = 1.0/numDatapoints;


    for (int h=0; h<numDatapoints; ++h) {
        dataArrays[pos][h].push_back(initialWeight);   //creating a weighting column for our current array
    }

for (int boostNum=0; boostNum<numBoostingRounds; boostNum++) {

    float bestThreshold = 0.0;
    int bestDirection = 1;
    float minimumError = 10.0;
    int bestFeature = 9999;




    thresCompare(currentLevel, maxLevel, pos, bestThreshold,
                 bestDirection, minimumError, bestFeature, dataArrays, singleBoostArray);

    boostingArrays.push_back(singleBoostArray);

    cout << "our first split array" << endl;



    for (int i=0; i<dataArrays[1].size(); ++i){
            for (int j=0; j<numFeatures+2; ++j) {
                cout << dataArrays[1][i][j] << " ";
            }
            cout << endl;
    }
    cout << endl;
    cout << "our second split array" << endl;
     for (int i=0; i<dataArrays[2].size(); ++i){
            for (int j=0; j<numFeatures+2; ++j) {
                cout << dataArrays[2][i][j] << " ";
            }
            cout << endl;
    }


    /*for (int i = 0; i<5; i++) {
        cout << boostingArray[0][i] << endl; //replace 0 with appropriate boosting round counter variable
    }
    */
    //using the best threshold we've found to classify our dataset
    int predictArray[numDatapoints];

    for (int i=0; i<numDatapoints; i++) {
        if (bestDirection == -1) {
            if (dataArrays[pos][i][bestFeature] < bestThreshold) {
                predictArray[i] = 1;
            }

            else {
                predictArray[i] = -1;
            }
        }
        else {
           if (bestDirection == 1) {
                if (dataArrays[pos][i][bestFeature] < bestThreshold) {
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
        if (predictArray[i] == dataArrays[pos][i][numFeatures]) {
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
        dataArrays[pos][i][numFeatures+1] = dataArrays[pos][i][numFeatures+1] * exp((-boostingArrays[boostNum][0][4])*dataArrays[pos][i][numFeatures]*predictArray[i]);
        sumWeights = sumWeights + dataArrays[pos][i][numFeatures+1];
    }
    for (int i=0; i< numDatapoints; i++) {   //normalise the weights
        dataArrays[pos][i][numFeatures+1] = dataArrays[pos][i][numFeatures+1]/sumWeights;

    }



 }//end of boosting rounds


 //   cout << "Minimum Error | Best Threshold | Best Direction | Best Feature | Alpha" << endl;
  //for (int h=0; )
  //for (int i=0; i<numBoostingRounds; i++) {
  //      for (int j=0; j<5; j++) {
  //          cout << boostingArrays[i][j] << "\t\t";
  //      }
  //    cout << endl;
  //}


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

   /*

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

    */


} //end main








