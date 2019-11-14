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

    vector< vector<float> > dataArray;  // the 2D array
                   // array of values for one line only

    vector< vector<float> > boostingArray;

    getline (in, line);
    stringstream ss(line);

    while (getline(ss, field, ',')) {
        variableNames.push_back(field);
    }





    while (getline (in,line) )    // get next line in file
    {

        string val;
        vector<float> row;
        stringstream ss(line);

        while (getline(ss, val,','))  // break line into comma delimitted fields
        {
            row.push_back(stof(val));  // add each field to the 1D array
        }

        dataArray.push_back(row);  // add the 1D array to the 2D array
    }


    int direction = 1;

    int numFeatures = dataArray[0].size()-1;
    int numDatapoints = dataArray.size();

    float initialWeight = 1.0/numDatapoints;
    float weightArray[numDatapoints];



    for (int h=0; h<numDatapoints; ++h) {
        weightArray[h] = initialWeight;

    }
    for (int h=0; h<numDatapoints; ++h) {
    cout << weightArray[h] << " ";
    }



    float sumPosThresGreater, sumNegThresGreater, sumPosThresLesser, sumNegThresLesser;

    float giniOne, giniTwo, weightedGini;



for (int boostNum=0; boostNum<numBoostingRounds; boostNum++) {

    float bestThreshold = 0.0;
    int bestDirection = 1;
    float bestError = 10.0;
    int bestFeature = 9999;
    float minimumGini = 10.0;

    cout << "boostNum is " << boostNum << endl;
    //cout << initialWeight << endl;



    for(int i=0; i<numFeatures; ++i) {

            //first loop, cycle through features
            cout << "feature num is " << i << endl;
      //  for(int j=0; j<2; ++j) {  //second loop, need our cycle to test both directions

            for (int k=0; k<numDatapoints; ++k) { //third loop, cycle through datapoint values for each feature

                int tempArray[numDatapoints];


                float pathOneDenom = 0;
                float pathTwoDenom = 0;

                sumPosThresGreater = 0.0;
                sumNegThresGreater = 0.0;
                sumPosThresLesser = 0.0;
                sumNegThresLesser = 0.0;

                cout << "threshold index value is " << k << " actual value is " << dataArray[k][i] << endl;

               // if (direction == 1) {
                       // for(int m =0; m<numDatapoints; ++m) {
                //tempArray[m] = (dataArray[m][i] >= dataArray[k][i]);    //creates a new array of datapoint size, with 1s if satisfies given

                for(int m =0; m<numDatapoints; ++m) {

                    if (dataArray[m][i] >= dataArray[k][i]) {

                        if (dataArray[m][numFeatures] == 1) {                    //threshold requirement and 0 if doesn't.
                            sumPosThresGreater = sumPosThresGreater + weightArray[m];
                        }
                        else {
                            sumNegThresGreater = sumNegThresGreater + weightArray[m];
                        }
                        pathOneDenom = pathOneDenom + weightArray[m];

                     }
                     else {
                        if (dataArray[m][numFeatures] == 1) {                    //threshold requirement and 0 if doesn't.
                            sumPosThresLesser = sumPosThresLesser + weightArray[m];
                        }
                        else {
                            sumNegThresLesser = sumNegThresLesser + weightArray[m];
                        }
                        pathTwoDenom = pathTwoDenom + weightArray[m];

                     }
                }
                cout << "path 1 denom is " << pathOneDenom << " path 2 denom is " << pathTwoDenom << endl;
                cout << "path 1 numerator is " << sumPosThresGreater << " and " << sumNegThresGreater << endl;
                cout << "path 2 numerator is " << sumPosThresLesser << " and " << sumNegThresLesser << endl;

                if (pathOneDenom == 0 || pathTwoDenom == 0) {
                   cout << "we used continue" << endl;
                   continue;
               }

                giniOne = 1.0 -(pow((sumPosThresGreater/pathOneDenom), 2.0) + (pow((sumNegThresGreater/pathOneDenom), 2.0)));

                cout << "giniOne is " << giniOne << endl;



                  giniTwo = 1.0 -(pow((sumPosThresLesser/pathTwoDenom), 2.0) + (pow((sumNegThresLesser/pathTwoDenom), 2.0)));

                cout << "giniTwo is " << giniTwo << endl;
                  weightedGini = (pathOneDenom)*giniOne + (pathTwoDenom)*giniTwo;


                   cout << "weightedGini is " << weightedGini << endl;

                  if (sumPosThresGreater >= sumNegThresGreater) {
                    direction = 1;
                  }
                  else {
                    direction = -1;
                  }
                  cout << "direction is " << direction << endl;

                for (int az=0; az<numDatapoints; ++az) {
                  if (direction == 1) {
                    tempArray[az] = (dataArray[az][i] >= dataArray[k][i]);
                  }
                  else {
                    tempArray[az] = (dataArray[az][i] < dataArray[k][i]);
                  }
                }
                cout << endl;
                cout << "our temp array is ";           //temp array is just assigning class 1 to higher(or lower depedning on direction) than our threshold.
                for (int ba=0; ba<numDatapoints; ++ba) {
                    cout << tempArray[ba] << " ";
                }
                cout << endl;




                for (int n=0; n<numDatapoints; ++n) {
                    if (tempArray[n] == 0) {
                        tempArray[n] = -1;          //convert all our 0s (from falses) to -1s our class.
                    }
                }
                cout << "our new temp array is ";           //temp array is just assigning class 1 to higher(or lower depedning on direction) than our threshold.
                for (int ba=0; ba<numDatapoints; ++ba) {
                    cout << tempArray[ba] << " ";
                }


                int comparisonArray[numDatapoints];

                for (int p =0; p<numDatapoints; ++p) {
                    if(tempArray[p] == dataArray[p][numFeatures]){
                        comparisonArray[p] = 0;
                    }
                    else {
                        comparisonArray[p] = 1;         //after comparing our predictions from this threshold to actual values
                    }                                   //we will have a comparison array where 1s are our errors, 0s are our matches.
                }
                cout << "our new comparison array is (1s are missclassified) ";           //temp array is just assigning class 1 to higher(or lower depedning on direction) than our threshold.
                for (int ba=0; ba<numDatapoints; ++ba) {
                    cout << comparisonArray[ba] << " ";
                }
                cout << endl;


                float partialSum;
                float errorProportion = 0.0;
                for (int q =0; q<numDatapoints; ++q) {
                    partialSum = comparisonArray[q]*weightArray[q];     //summing all our weighted errors
                    errorProportion = errorProportion + partialSum;
                    //cout << "ComparisonArray number " << comparisonArray[q] << endl; //0s are good, 1s are misclassified
                   //cout << "WeightArray is " << weightArray[q] << endl;
                    //cout << "PartialSum is " << partialSum << endl;
                    //cout << "ErrorProportion is " << errorProportion << endl;
                }




                if (weightedGini < minimumGini) {       //if our weighted errors sum is our actual minimum for now
                    minimumGini = weightedGini;
                    bestThreshold = dataArray[k][i];
                    bestError = errorProportion;
                    bestDirection = direction;
                    bestFeature = i;

                }
                cout << "minimum Gini is " << minimumGini << endl;
                cout << "best error is " << bestError << endl;
                cout << "best threshold is " << bestThreshold << endl;
                cout << "best direction is " << bestDirection << endl;
                cout << "best feature is " << bestFeature << endl;
                cout << "boost num is " << boostNum << endl;
                cout << endl << endl;



            }//end of threshold loop

        }//end of feature loop

       // }//end of direction loop

    //end of feature loop

    /*
    */
    //next need to calculate alpha of classifier found

    //while in a boosting round that resets the row every boosting loop
    if (bestError == 0.0) {bestError = 0.0000001;}

    float rowHelper[] = {bestError, bestThreshold, bestDirection, bestFeature};
    vector<float> boostingRow(rowHelper, rowHelper+4);


    boostingArray.push_back(boostingRow);



    alpha = 0.5*log((1.0 - bestError)/(bestError));



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
    cout << "our new weight array non-normalized" << endl;
    for (int i=0; i<numDatapoints; ++i) {
        cout << weightArray[i] << " ";
    }
    cout << endl;

    for (int i=0; i< numDatapoints; i++) {   //normalise the weights
        weightArray[i] = weightArray[i]/sumWeights;

    }
    cout << endl << endl;
    cout << "our new weight array non-normalized" << endl;
    for (int i=0; i<numDatapoints; ++i) {
        cout << weightArray[i] << " ";
    }
    cout << endl;



 }//end boosting loop

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

    //boosting array 0 bestError, 1 bestThreshold, 2 bestDirection, 3 bestFeature, 4 alpha
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








