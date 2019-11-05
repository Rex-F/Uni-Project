#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

using namespace std;

 //void merge(float arr[numDatapoints][2], int low, int high, int mid, int numDatapoints);
 //void merge_sort(float arr[numDatapoints][2], int low, int high, int numDatapoints);

 void merge(float arr[][2], int low, int high, int mid, int numDatapoints)
	{
	    int i, j, k;
	    float c[numDatapoints][2];
	    i = low;
	    k = low;
	    j = mid + 1;
	    while (i <= mid && j <= high) {
	        if (arr[i][0] < arr[j][0]) {
	            c[k][0] = arr[i][0];
	            c[k][1] = arr[i][1];
	            k++;
	            i++;
	        }
	        else  {
	            c[k][0] = arr[j][0];
	            c[k][1] = arr[j][1];
	            k++;
	            j++;
	        }
	    }
	    while (i <= mid) {
	        c[k][0] = arr[i][0];
	        c[k][1] = arr[i][1];
	        k++;
	        i++;
	    }
	    while (j <= high) {
	        c[k][0] = arr[j][0];
	        c[k][1] = arr[j][1];
	        k++;
	        j++;
	    }
	    for (i = low; i < k; i++)  {
	        arr[i][0] = c[i][0];
	        arr[i][1] = c[i][1];
	    }
	}
void merge_sort(float arr[][2], int low, int high, int numDatapoints) {

    int mid;
    if (low < high){
        //divide the array at mid and sort independently using merge sort
        mid=(low+high)/2;
	        merge_sort(arr,low,mid, numDatapoints);
	        merge_sort(arr,mid+1,high, numDatapoints);
	        //merge or conquer sorted arrays
	        merge(arr,low,high,mid, numDatapoints);
	}
}


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




    int numFeatures = dataArray[0].size()-1;
    int numDatapoints = dataArray.size();

    cout << "numFeatures is " << numFeatures << " numDatapoints is " << numDatapoints << endl;



    float totalPos = 0.0;
    float totalNeg = 0.0;

    float leftPosThresError = 0.0;
    float rightPosThresError = 0.0;

    int bestDirection = 1;
    float minError = 10.0;
    float bestThresh = 0.0;
    int bestFeature;

    float initialWeight = 1.0/numDatapoints;
    float weightArray[numDatapoints];
    for (int h=0; h<numDatapoints; ++h) {
        weightArray[h] = initialWeight;
    }

    int posClassificationArray[numDatapoints];
    int negClassificationArray[numDatapoints];

    for (int g=0; g<numDatapoints; ++g) {
        if (dataArray[g][numFeatures] == 1) {
            posClassificationArray[g] = 1;
            negClassificationArray[g] = 0;
        }
        else {
            posClassificationArray[g] = 0;
            negClassificationArray[g] = 1;
        }

    }
    cout << "Classification Arrays Pos then Neg" << endl;
    for (int g=0; g<numDatapoints; ++g) {
        cout << posClassificationArray[g] << " ";
    }
    cout << endl;
    for (int g=0; g<numDatapoints; ++g) {
        cout << negClassificationArray[g] << " ";
    }



    float partialSumPos, partialSumNeg;


    //loop that finds the total weightings for each of positive and negative classes
    for (int q =0; q<numDatapoints; ++q) {
        partialSumPos = posClassificationArray[q]*weightArray[q];
        partialSumNeg = negClassificationArray[q]*weightArray[q];
        totalPos = totalPos + partialSumPos;
        totalNeg = totalNeg + partialSumNeg;
    }
    cout << "\nTotal Pos is " << totalPos << endl;
    cout << "Total Neg is " << totalNeg << endl;




    for(int ab=0; ab<numFeatures; ab++) {   //loop to apply following to all our features
        float sumPosOnTheLine = 0.0;
        float sumNegOnTheLine = 0.0;

         //create array of only the specific feature with its classification
        float tempArray[numDatapoints][2];

        for (int ac = 0; ac<numDatapoints; ac++) {
            tempArray[ac][0] = dataArray[ac][ab];
            tempArray[ac][1] = dataArray[ac][numFeatures];
        }

        merge_sort(tempArray, 0, numDatapoints-1, numDatapoints);    //sort this temporary array

         cout << "\nWeightings" << endl;

        for (size_t i=0; i<numDatapoints; ++i) {

                cout << weightArray[i] << " ";
             }



        cout << "\nSorted Array" << endl;

        for (size_t i=0; i<numDatapoints; ++i) {

                cout << tempArray[i][0] << " ";
             }
             cout << "\nClasses after sorting " << endl;
          for (size_t i=0; i<numDatapoints; ++i) {

                cout << tempArray[i][1] << " ";
             }
        cout << "\nUnsorted Array" << endl;
        for (size_t i=0; i<numDatapoints; ++i) {

                cout << dataArray[i][ab] << " ";
             }
        cout << endl;



        for(int ad=0; ad<numDatapoints; ad++) {

            rightPosThresError = sumPosOnTheLine + (totalNeg - sumNegOnTheLine);
            cout << "rightError is " << rightPosThresError << " = " << sumPosOnTheLine << " + " << totalNeg << " - " << sumNegOnTheLine << endl;

            if (tempArray[ad][1] == -1) {
                cout << "Previous Sum on the Neg is " << sumNegOnTheLine << endl;
                sumNegOnTheLine = sumNegOnTheLine + negClassificationArray[ad]*weightArray[ad];
                cout << "sum on the Neg = previous + negClassArray*Weight Array " << endl;
                cout << sumNegOnTheLine << "+ previous = " << negClassificationArray[ad] << weightArray[ad];
            }
            else {
                cout << "Previous Sum on the Pos is " << sumPosOnTheLine << endl;
                sumPosOnTheLine = sumPosOnTheLine + posClassificationArray[ad]*weightArray[ad];

                cout << "sum on the Neg = previous + negClassArray*Weight Array " << endl;
                cout << sumPosOnTheLine << "+ previous = " << posClassificationArray[ad] << weightArray[ad];
            }

            leftPosThresError = sumNegOnTheLine + (totalPos - sumPosOnTheLine);
            cout << "leftError is " << leftPosThresError << " = " << sumNegOnTheLine << " + " << totalPos << " - " << sumPosOnTheLine << endl;
            cout << "Sum Negative on the line is " << sumNegOnTheLine << " Sum Pos on the Line is " << sumPosOnTheLine << endl;
             cout << "right error " << rightPosThresError << " left error " << leftPosThresError << endl;


            if (minError > rightPosThresError) {
                minError = rightPosThresError;
                bestThresh = tempArray[ad][0];
                bestFeature = ab;
                bestDirection = 1;
            }
            if (minError > leftPosThresError) {
                minError = leftPosThresError;
                bestThresh = tempArray[ad][0];
                bestFeature = ab;
                bestDirection = -1;
            }

            cout << "best error is " << minError << endl;
        }









        cout << "\n";

        cout << "Minimum error " << minError << "\nBest Thresh " << bestThresh << "\nBest Direction " << bestDirection << "\nbestFeature " << bestFeature
        << endl;

    }

}















/*

    for(int i=0; i<numFeatures; ++i) {
        for (int j=0; j>numDatapoints; ++j) {

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
*/


