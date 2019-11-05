#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

using namespace std;


int main()
{
    string filename;

    //cout << "Enter a filename please: " << endl;
    //cin >> filename;
    //ifstream in(filename.c_str());

    ifstream in("dataset.csv");

    string line, field;

    vector<string> variableNames;

    vector< vector<float> > dataArray;  // the 2D array
                   // array of values for one line only

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


    int directionArray[2] = {1, -1};

    int numFeatures = dataArray[0].size()-1;
    int numDatapoints = dataArray.size();

    float bestThreshold = 0.0;
    int bestDirection = 1;
    float minimumError = 10.0;
    int bestFeature = 9999;

    float initialWeight = 1.0/numDatapoints;
    cout << numDatapoints << endl;
    cout << initialWeight << endl;

    float weightArray[numDatapoints];
    for (int h=0; h<numDatapoints; ++h) {
        weightArray[h] = initialWeight;
    }

    for(int i=0; i<numFeatures; ++i) {   //first loop, cycle through features

        for(int j=0; j<2; ++j) {  //second loop, need our cycle to test both directions

            for (int k=0; k<numDatapoints; ++k) { //third loop, cycle through datapoint values for each feature

                int tempArray[numDatapoints];

                if (directionArray[j] == 1) {
                        for(int m =0; m<numDatapoints; ++m) {
                            tempArray[m] = (dataArray[m][i] >= dataArray[k][i]);
                            cout << "compare " << dataArray[m][i] << " to threshold " << dataArray[k][i] << endl;
                            cout << "1s for trues, 0s for falses "<< tempArray[m] << endl;
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
                cout << endl;

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
                    cout << "ComparisonArray number " << comparisonArray[q] << endl; //0s are good, 1s are misclassified
                    cout << "WeightArray is " << weightArray[q] << endl;
                    cout << "PartialSum is " << partialSum << endl;
                    cout << "ErrorProportion is " << errorProportion << endl;
                }

                if (errorProportion < minimumError) {
                    minimumError = errorProportion;
                    bestThreshold = dataArray[k][i];
                    bestDirection = directionArray[j];
                    bestFeature = i;

                }


            }
        }
    }
    cout << "minimum error is " << minimumError << endl;
    cout << "best threshold is " << bestThreshold << endl;
    cout << "best direction is " << bestDirection << endl;
    cout << "best feature is " << bestFeature << endl;











    // print out what was read in

    for (size_t i=0; i<dataArray.size(); ++i)
    {
        for (size_t j=0; j<dataArray[i].size(); ++j)
        {
            cout << dataArray[i][j] << " ";
        }
        cout << "\n";
    }

    cout << dataArray[0][1]+dataArray[1][1] << endl;

    cout << variableNames[0] << endl;



    return 0;
    //changed!
}


