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
#include <chrono>

using namespace std;

 ofstream fout("output.csv");

struct bestThresReturns {
    double minimumError;
    double bestThreshold;
    int bestDirection;
    int bestFeature;
};

struct node {
  //vector<vector<double> > *pointerToArray;
   //usage   pointerToArray = &dataArray;
  bestThresReturns key_value;
  node *left;
  node *right;
};

void merge(double arr[][3], int low, int high, int mid, int numDatapoints) {

	    int i, j, k;
	    double c[numDatapoints][3];
	    i = low;
	    k = low;
	    j = mid + 1;
	    while (i <= mid && j <= high) {
	        if (arr[i][0] < arr[j][0]) {
	            c[k][0] = arr[i][0];
	            c[k][1] = arr[i][1];
	            c[k][2] = arr[i][2];
	            k++;
	            i++;
	        }
	        else  {
	            c[k][0] = arr[j][0];
	            c[k][1] = arr[j][1];
	            c[k][2] = arr[j][2];
	            k++;
	            j++;
	        }
	    }
	    while (i <= mid) {
	        c[k][0] = arr[i][0];
	        c[k][1] = arr[i][1];
	        c[k][2] = arr[i][2];
	        k++;
	        i++;
	    }
	    while (j <= high) {
	        c[k][0] = arr[j][0];
	        c[k][1] = arr[j][1];
	        c[k][2] = arr[j][2];
	        k++;
	        j++;
	    }
	    for (i = low; i < k; i++)  {
	        arr[i][0] = c[i][0];
	        arr[i][1] = c[i][1];
	        arr[i][2] = c[i][2];
	    }
}


void merge_sort(double arr[][3], int low, int high, int numDatapoints) {

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

bestThresReturns discoverBestThresGini(vector<vector<double> > &dataArray) {

    bestThresReturns a;

    float minimumGini = 10.0;

    int numFeatures = dataArray[0].size()-2;
    int numDatapoints = dataArray.size();

    float sumPosThresGreater, sumNegThresGreater, sumPosThresLesser, sumNegThresLesser;

    float giniOne, giniTwo, weightedGini;

    a.bestThreshold = 0.0;
    a.bestDirection = 1;
    a.minimumError = 10.0;
    a.bestFeature = 9999;



    for(int i=0; i<numFeatures; ++i) {

            //first loop, cycle through features
           // cout << "feature num is " << i << endl;
      //  for(int j=0; j<2; ++j) {  //second loop, need our cycle to test both directions

            for (int k=0; k<numDatapoints; ++k) { //third loop, cycle through datapoint values for each feature as the chosen threshold

                int tempArray[numDatapoints];


                float pathOneDenom = 0;
                float pathTwoDenom = 0;

                sumPosThresGreater = 0.0;
                sumNegThresGreater = 0.0;
                sumPosThresLesser = 0.0;
                sumNegThresLesser = 0.0;

                int direction;

          //      cout << "threshold index value is " << k << " actual value is " << dataArray[k][i] << endl;

               // if (direction == 1) {
                       // for(int m =0; m<numDatapoints; ++m) {
                //tempArray[m] = (dataArray[m][i] >= dataArray[k][i]);    //creates a new array of datapoint size, with 1s if satisfies given

                for(int m =0; m<numDatapoints; ++m) {

                    if (dataArray[m][i] >= dataArray[k][i]) {

                        if (dataArray[m][numFeatures] == 1) {                    //threshold requirement and 0 if doesn't.
                            sumPosThresGreater = sumPosThresGreater + dataArray[m][numFeatures+1];
                        }
                        else {
                            sumNegThresGreater = sumNegThresGreater + dataArray[m][numFeatures+1];
                        }
                        pathOneDenom = pathOneDenom + dataArray[m][numFeatures+1];

                     }
                     else {
                        if (dataArray[m][numFeatures] == 1) {                    //threshold requirement and 0 if doesn't.
                            sumPosThresLesser = sumPosThresLesser + dataArray[m][numFeatures+1];
                        }
                        else {
                            sumNegThresLesser = sumNegThresLesser + dataArray[m][numFeatures+1];
                        }
                        pathTwoDenom = pathTwoDenom + dataArray[m][numFeatures+1];

                     }
                }
         //       cout << "path 1 denom is " << pathOneDenom << " path 2 denom is " << pathTwoDenom << endl;
          //      cout << "path 1 numerator is " << sumPosThresGreater << " and " << sumNegThresGreater << endl;
         //       cout << "path 2 numerator is " << sumPosThresLesser << " and " << sumNegThresLesser << endl;

                if (pathOneDenom == 0 || pathTwoDenom == 0) {
          //         cout << "we used continue" << endl;
                   continue;
               }

                giniOne = 1.0 -(pow((sumPosThresGreater/pathOneDenom), 2.0) + (pow((sumNegThresGreater/pathOneDenom), 2.0)));

           //     cout << "giniOne is " << giniOne << endl;



                  giniTwo = 1.0 -(pow((sumPosThresLesser/pathTwoDenom), 2.0) + (pow((sumNegThresLesser/pathTwoDenom), 2.0)));

          //      cout << "giniTwo is " << giniTwo << endl;
                  weightedGini = (pathOneDenom)*giniOne + (pathTwoDenom)*giniTwo;


           //        cout << "weightedGini is " << weightedGini << endl;

                  if (sumPosThresGreater >= sumNegThresGreater) {
                    direction = 1;
                  }
                  else {
                    direction = -1;
                  }
          //        cout << "direction is " << direction << endl;

                for (int az=0; az<numDatapoints; ++az) {
                  if (a.bestDirection == 1) {
                    tempArray[az] = (dataArray[az][i] >= dataArray[k][i]);
                  }
                  else {
                    tempArray[az] = (dataArray[az][i] < dataArray[k][i]);
                  }
                }
         //       cout << endl;
         //       cout << "our temp array is ";           //temp array is just assigning class 1 to higher(or lower depedning on direction) than our threshold.
         //       for (int ba=0; ba<numDatapoints; ++ba) {
          //          cout << tempArray[ba] << " ";
          //      }
         //       cout << endl;




                for (int n=0; n<numDatapoints; ++n) {
                    if (tempArray[n] == 0) {
                        tempArray[n] = -1;          //convert all our 0s (from falses) to -1s our class.
                    }
                }
            //    cout << "our new temp array is ";           //temp array is just assigning class 1 to higher(or lower depedning on direction) than our threshold.
        //        for (int ba=0; ba<numDatapoints; ++ba) {
        //            cout << tempArray[ba] << " ";
        //        }


                int comparisonArray[numDatapoints];

                for (int p =0; p<numDatapoints; ++p) {
                    if(tempArray[p] == dataArray[p][numFeatures]){
                        comparisonArray[p] = 0;
                    }
                    else {
                        comparisonArray[p] = 1;         //after comparing our predictions from this threshold to actual values
                    }                                   //we will have a comparison array where 1s are our errors, 0s are our matches.
                }
         //       cout << "our new comparison array is (1s are missclassified) ";           //temp array is just assigning class 1 to higher(or lower depedning on direction) than our threshold.
        //        for (int ba=0; ba<numDatapoints; ++ba) {
         //           cout << comparisonArray[ba] << " ";
        //        }
        //        cout << endl;


                float partialSum;
                float errorProportion = 0.0;
                for (int q =0; q<numDatapoints; ++q) {
                    partialSum = comparisonArray[q]*dataArray[q][numFeatures+1];     //summing all our weighted errors
                    errorProportion = errorProportion + partialSum;
                    //cout << "ComparisonArray number " << comparisonArray[q] << endl; //0s are good, 1s are misclassified
                   //cout << "WeightArray is " << weightArray[q] << endl;
                    //cout << "PartialSum is " << partialSum << endl;
                    //cout << "ErrorProportion is " << errorProportion << endl;
                }




                if (weightedGini < minimumGini) {       //if our weighted errors sum is our actual minimum for now
                    minimumGini = weightedGini;
                    a.bestThreshold = dataArray[k][i];
                    a.minimumError = errorProportion;
                    a.bestDirection = direction;
                    a.bestFeature = i;

                }




            }//end of threshold loop

        }//end of feature loop

       // }//end of direction loop

    return a;
}





bestThresReturns discoverBestThresSorted(vector<vector<double> > &dataArray) {

     bestThresReturns a;
     a.bestDirection = 1;
     a.minimumError = 10.0;

    double totalPos = 0.0;
    double totalNeg = 0.0;

    double leftPosThresError = 0.0;
    double rightPosThresError = 0.0;



    int numFeatures = dataArray[0].size()-2;
    int numDatapoints = dataArray.size();


    int posClassArray[numDatapoints];
    int negClassArray[numDatapoints];

    int orderedPosClassArray[numDatapoints];
    int orderedNegClassArray[numDatapoints];

     for (int g=0; g<numDatapoints; ++g) {
            if (dataArray[g][numFeatures] == 1) {
                posClassArray[g] = 1;
                negClassArray[g] = 0;
            }
            else {
                posClassArray[g] = 0;
                negClassArray[g] = 1;
            }

        }

    double partialSumPos, partialSumNeg;


    //loop that finds the total weightings for each of positive and negative classes
    for (int q =0; q<numDatapoints; ++q) {
        partialSumPos = posClassArray[q]*dataArray[q][numFeatures+1];
        partialSumNeg = negClassArray[q]*dataArray[q][numFeatures+1];
        totalPos = totalPos + partialSumPos;
        totalNeg = totalNeg + partialSumNeg;
    }
    //cout << "\nTotal Pos is " << totalPos << endl;
   // cout << "Total Neg is " << totalNeg << endl;




    for(int ab=0; ab<numFeatures; ab++) {   //loop to apply following to all our features
        double sumPosOnTheLine = 0.0;
        double sumNegOnTheLine = 0.0;

         //create array of only the specific feature with its classification
        double tempArray[numDatapoints][3];

        for (int ac = 0; ac<numDatapoints; ac++) {
            tempArray[ac][0] = dataArray[ac][ab];
            tempArray[ac][1] = dataArray[ac][numFeatures];
            tempArray[ac][2] = dataArray[ac][numFeatures+1];
        }

        merge_sort(tempArray, 0, numDatapoints-1, numDatapoints);    //sort this temporary array

        for (int g=0; g<numDatapoints; ++g) {
            if (tempArray[g][1] == 1) {
                orderedPosClassArray[g] = 1;
                orderedNegClassArray[g] = 0;
            }
            else {
                orderedPosClassArray[g] = 0;
                orderedNegClassArray[g] = 1;
            }

        }
       /* cout << "orderedPosClassArray" << endl;
        for (int aq = 0; aq<numDatapoints; aq++) {
            cout << orderedPosClassArray[aq] << " ";
        }
        cout << "orderedNegClassArray" << endl;
        for (int aq = 0; aq<numDatapoints; aq++) {
            cout << orderedNegClassArray[aq] << " ";
        }
        cout << endl; */







       /* cout << "\nSorted Array" << endl;

        for (size_t i=0; i<numDatapoints; ++i) {
            for (size_t j=0; j<3; ++j) {
                 cout << tempArray[i][j] << " ";
             }
             cout << endl;
        } */




        for(int ad=0; ad<numDatapoints; ad++) {

            rightPosThresError = sumPosOnTheLine + (totalNeg - sumNegOnTheLine);
          //  cout << "right error = sumPosOnTheLine + totalNeg - sumNegOnTheLine" << endl;
          //  cout << rightPosThresError << " = " << sumPosOnTheLine << " + " << totalNeg << " - " << sumNegOnTheLine << endl << endl;

            if (tempArray[ad][1] == -1) {
             //   cout << "Previous Sum on the Neg is " << sumNegOnTheLine << endl;
                sumNegOnTheLine = sumNegOnTheLine + orderedNegClassArray[ad] * tempArray[ad][2];
             //   cout << "sumNegOnTheLine = previous + negClassArray * tempArray weighting " << endl;
            //    cout << sumNegOnTheLine << " = previousSumNeg + " << orderedNegClassArray[ad] << " * " << tempArray[ad][2] << endl << endl;
            }
            else {
           //     cout << "Previous Sum on the Pos is " << sumPosOnTheLine << endl;
                sumPosOnTheLine = sumPosOnTheLine + orderedPosClassArray[ad]*tempArray[ad][2];

          //      cout << "SumPosOnTheLine = previous + posClassArray * Weight Array " << endl;
         //       cout << sumPosOnTheLine << " = previousSumPos + " << orderedPosClassArray[ad] << " * "<< tempArray[ad][2] << endl << endl;
            }

            leftPosThresError = sumNegOnTheLine + (totalPos - sumPosOnTheLine);
       //     cout << "left error = sumNegOnTheLine + totalPos - sumPosOnTheLine" << endl;
       //     cout << leftPosThresError << " = " << sumNegOnTheLine << " + " << totalPos << " - " << sumPosOnTheLine << endl << endl;
      //      cout << "in conclusion Sum Negative on the line is " << sumNegOnTheLine << " SumPosontheLineis " << sumPosOnTheLine << endl;
       //      cout << "in conclusion right error is " << rightPosThresError << " and left error is " << leftPosThresError << endl;


            if (a.minimumError > rightPosThresError) {
                a.minimumError = rightPosThresError;
                a.bestThreshold = tempArray[ad][0];
                a.bestFeature = ab;
                a.bestDirection = 1;
            }
            if (a.minimumError > leftPosThresError) {
                a.minimumError = leftPosThresError;
                a.bestThreshold = tempArray[ad][0];
                a.bestFeature = ab;
                a.bestDirection = -1;
            }

       //     cout << "best error is " << a.minimumError << endl << endl;
        }

    }
    return a;
}


bestThresReturns discoverBestThresManual(vector<vector<double> > &dataArray) {

    bestThresReturns a;
   // a.bestThreshold = 0.0;
   // a.bestDirection = 1;
    a.minimumError = 10.0;
   // a.bestFeature = 9999;


    int numFeatures = dataArray[0].size()-2;
    int numDatapoints = dataArray.size();



   /*cout << "numFeatures is " << numFeatures << endl;

  for(int a=0; a<numDatapoints; ++a) {
       for(int b=0; b<numFeatures+2; ++b) {
           cout << dataArray[a][b] << " ";
       }
        cout << endl;
    }

    //array is passed fine
    */
    int directionArray[2] = {1, -1};

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
                        tempArray[n] = -1;          //convert all our 0s (from falses) to -1s our class.
                    }
                }
                //cout << endl;

                int comparisonArray[numDatapoints];

                for (int p =0; p<numDatapoints; ++p) {
                    if(tempArray[p] == dataArray[p][numFeatures]){
                        comparisonArray[p] = 0;
                    }
                    else {
                        comparisonArray[p] = 1;         //after comparing our predictions from this threshold to actual values
                    }                                   //we will have a comparison array where 1s are our errors, 0s are our matches.
                }





                double partialSum;
                double errorProportion = 0.0;
                for (int q =0; q<numDatapoints; ++q) {
                  partialSum = comparisonArray[q]*(dataArray[q][numFeatures+1]);     //summing all our weighted errors, our weights come into play by
                    errorProportion = errorProportion + partialSum;                 //deciding which is minimum error and therefore which threshold to use
                   // cout << "ComparisonArray number " << comparisonArray[q] << endl; //0s are good, 1s are misclassified
                    //cout << "WeightArray is " << weightArray[q] << endl;
                   // cout << "PartialSum is " << partialSum << endl;
                   // cout << "ErrorProportion is " << errorProportion << endl;
                }

                if (errorProportion < a.minimumError) {       //if our weighted errors sum is our actual minimum for now
                    a.minimumError = errorProportion;
                    a.bestThreshold = dataArray[k][i];

                    //can use this with sorted values
                    /*if (k!=0) {
                        cout << dataArray[k][i] << " " << endl;
                        a.bestThreshold = (dataArray[k][i]+dataArray[k-1][i])/2;
                    }
                    else {
                        a.bestThreshold = dataArray[k][i];
                    }*/

                    a.bestDirection = directionArray[j];
                    a.bestFeature = i;

                }


            }//end of threshold loop

        }//end of direction loop

    }//end of feature loop



    return a;


}

bool isItPure(vector<vector<double> > &dataArrayCheck) {


    int numDatapoints = dataArrayCheck.size();

    if (dataArrayCheck.size()==0) {return false;}
    int numFeatures = dataArrayCheck[0].size()-2;

    bool hasPos = false;
    bool hasNeg = false;

    for(int i=0; i<numDatapoints; ++i) {
        if (dataArrayCheck[i][numFeatures]==1) {
            hasPos = true;
            break;
        }
    }
    for(int i=0; i<numDatapoints; ++i) {
        if (dataArrayCheck[i][numFeatures]==-1) {
            hasNeg = true;
            break;
        }
    }

    if((hasNeg == false) || (hasPos == false)) {return true;}
    else {return false;}



}





     // node *search(int key);
     // void destroy_tree();




     // void destroy_tree(node *leaf);
     // void createNewLevel(bestThresReturns prevKey, node *leaf, int currentLevel, int maxLevel, vector<vector<double> > &prevDataArray);
     // node *search(int key, node *leaf);




//declaring a btree object in main automatically creates root pointer since its declaration in class, and constructor sets this pointer to NULL.



void destroy_tree(node *leaf)
{
  if(leaf!=NULL)
  {
    destroy_tree(leaf->left);
    destroy_tree(leaf->right);
    delete leaf;
    leaf=NULL;
  }
}

void createNewLevel(bestThresReturns prevKey, node *leaf, int currentLevel, int maxLevel, vector<vector<double> > &prevDataArray, int method,
                    vector<string> &variableNames, char &suppress) {

    currentLevel++;
    cout << "\nAttempting to access level " << currentLevel << " of max " << maxLevel << endl;
    fout << "\nAttempting to access level " << currentLevel << " of max " << maxLevel << endl;

    if(currentLevel>maxLevel) {return;}



    //cout << "1" << endl;

    vector<vector<double> > tempLeftArray, tempRightArray;

   // cout << "2" << endl;

    int numFeatures = prevDataArray[0].size()-2;
    int numDatapoints = prevDataArray.size();

    bestThresReturns tempReturns;

   // cout << "3" << endl;

    for(int a=0; a<numDatapoints; ++a) {
        vector<double> row;



        if(prevDataArray[a][prevKey.bestFeature]<prevKey.bestThreshold) {
            for(int b=0; b<numFeatures+2; ++b) {
                row.push_back(prevDataArray[a][b]);
            }
            tempLeftArray.push_back(row);
        }
        else {
            for(int c=0; c<numFeatures+2; ++c) {
                row.push_back(prevDataArray[a][c]);
            }
            tempRightArray.push_back(row);
        }

    }
    // cout << "4" << endl;

    int numLeftDatapoints = tempLeftArray.size();
    int numRightDatapoints = tempRightArray.size();

    if (numLeftDatapoints == 0) {
        cout << "Left Array is empty" << endl;
        fout << "Left Array is empty" << endl;
        return;
    }
    if (numRightDatapoints == 0) {
        cout << "Right Array is empty" << endl;
        fout << "Right Array is empty" << endl;
        return;
    }



  //  cout << "5" << endl;






    if (isItPure(tempLeftArray) == false) {
   //     cout << "8" << endl;
        leaf->left=new node;
    //    cout << "9" << endl;
        if (method == 1) {
         leaf->left->key_value = discoverBestThresManual(tempLeftArray);
       }
       else if (method == 2) {
         leaf->left->key_value = discoverBestThresSorted(tempLeftArray);
       }
       else {
        leaf->left->key_value = discoverBestThresGini(tempLeftArray);
       }

    //    cout << "10" << endl;
         fout << "\nOur left array at level " << currentLevel << endl;

    if (suppress == 'n') {
        for(int a=0; a<numFeatures; ++a) {
            fout << variableNames[a] << ",";
        }
        fout << "Class,Weighting" << endl;
        for(int a=0; a<numLeftDatapoints; ++a) {
            for(int b=0; b<numFeatures+2; ++b) {
                fout << tempLeftArray[a][b] << ",";
            }
            fout << endl;
        }
    }

        cout << "On left at level " << currentLevel << ". Set size is " << numLeftDatapoints << ". Generating new threshold:" << endl;
        cout << "Direction: " << leaf->left->key_value.bestDirection << endl;
        cout << "Feature: " << leaf->left->key_value.bestFeature << endl;
        cout << "Threshold: " << leaf->left->key_value.bestThreshold << endl;
        cout << "Error: " << leaf->left->key_value.minimumError << endl;
        fout << "On left at level " << currentLevel << ". Set size is " << numLeftDatapoints << ". Generating new threshold:" << endl;
        fout << "Direction: " << leaf->left->key_value.bestDirection << endl;
        fout << "Feature: " << leaf->left->key_value.bestFeature << endl;
        fout << "Threshold: " << leaf->left->key_value.bestThreshold << endl;
        fout << "Error: " << leaf->left->key_value.minimumError << endl;

        leaf->left->left=NULL;    //Sets the left child of the child node to null
        leaf->left->right=NULL;   //Sets the right child of the child node to null

        createNewLevel(leaf->left->key_value, leaf->left, currentLevel, maxLevel, tempLeftArray, method, variableNames, suppress);
        cout << "Returning to level " << currentLevel << " of max " << maxLevel << endl;
        fout << "Returning to level " << currentLevel << " of max " << maxLevel << endl;
    }
    else {
        cout << "Left path is pure" << endl;
        fout << "Left path is pure" << endl;
    }

     if (isItPure(tempRightArray) == false) {
        leaf->right=new node;

        if (method == 1) {
         leaf->right->key_value = discoverBestThresManual(tempRightArray);
       }
       else if (method == 2) {
         leaf->right->key_value = discoverBestThresSorted(tempRightArray);
       }
       else {
        leaf->right->key_value = discoverBestThresGini(tempRightArray);
       }


         fout << "\nOur right array at level " << currentLevel << endl;

    if (suppress == 'n') {
        for(int a=0; a<numFeatures; ++a) {
            fout << variableNames[a] << ",";
        }
        fout << "Class,Weighting" << endl;

        for(int a=0; a<numRightDatapoints; ++a) {
            for(int b=0; b<numFeatures+2; ++b) {
                fout << tempRightArray[a][b] << ",";
            }
        fout << endl;
        }
    }
        cout << "On right at level " << currentLevel << ". Set size is " << numRightDatapoints << ". Generating new threshold:" << endl;
        cout << "Direction: " << leaf->right->key_value.bestDirection << endl;
        cout << "Feature: " << leaf->right->key_value.bestFeature << endl;
        cout << "Threshold: " << leaf->right->key_value.bestThreshold << endl;
        cout << "Error: " << leaf->right->key_value.minimumError << endl;
        fout << "On right at level " << currentLevel << ". Set size is " << numRightDatapoints << ". Generating new threshold:" << endl;
        fout << "Direction: " << leaf->right->key_value.bestDirection << endl;
        fout << "Feature: " << leaf->right->key_value.bestFeature << endl;
        fout << "Threshold: " << leaf->right->key_value.bestThreshold << endl;
        fout << "Error: " << leaf->right->key_value.minimumError << endl;

        leaf->right->left=NULL;  //Sets the left child of the child node to null
        leaf->right->right=NULL; //Sets the right child of the child node to null

        createNewLevel(leaf->right->key_value, leaf->right, currentLevel, maxLevel, tempRightArray, method, variableNames, suppress);
        cout << "Returning to level " << currentLevel << " of max " << maxLevel << endl;
        fout << "Returning to level " << currentLevel << " of max " << maxLevel << endl;
    }
    else {
        cout << "Right path is pure" << endl;
        fout << "Right path is pure" << endl;
    }


}//end of function

void findPredictions(vector<vector<double> > &dataArray, int predictArray[], node *leaf, int i, int numFeatures, int numDatapoints) {

            if (dataArray[i][leaf->key_value.bestFeature] < leaf->key_value.bestThreshold) {
                if (leaf->left!=NULL) {
                     findPredictions(dataArray, predictArray, leaf->left, i, numFeatures, numDatapoints);
                }
                else {
                    if (leaf->key_value.bestDirection == -1) {
                        predictArray[i] = 1;
                    }
                    else {
                        predictArray[i] = -1;
                    }
                }
            }
            else if (dataArray[i][leaf->key_value.bestFeature] >= leaf->key_value.bestThreshold) {
                if (leaf->right!=NULL) {
                     findPredictions(dataArray, predictArray, leaf->right, i, numFeatures, numDatapoints);
                }
                else {
                    if (leaf->key_value.bestDirection == -1) {
                        predictArray[i] = -1;
                    }
                    else {
                        predictArray[i] = 1;
                    }
                }
            }

    }


/*


  if(key< leaf->key_value)
  {
    if(leaf->left!=NULL)
     insert(key, leaf->left);
    else
    {
      leaf->left=new node;
      leaf->left->key_value=key;
      leaf->left->left=NULL;    //Sets the left child of the child node to null
      leaf->left->right=NULL;   //Sets the right child of the child node to null
    }
  }
  else if(key>=leaf->key_value)
  {
    if(leaf->right!=NULL)
      insert(key, leaf->right);
    else
    {
      leaf->right=new node;
      leaf->right->key_value=key;
      leaf->right->left=NULL;  //Sets the left child of the child node to null
      leaf->right->right=NULL; //Sets the right child of the child node to null
    }
  } */


/*node *btree::search(int key, node *leaf)
{
  if(leaf!=NULL)
  {
    if(key==leaf->key_value)
      return leaf;
    if(key<leaf->key_value)
      return search(key, leaf->left);
    else
      return search(key, leaf->right);
  }
  else return NULL;
}
*/

node *createRootLevel(bestThresReturns key, int maxLevel, vector<vector<double> > &dataArray, int method, vector<string> &variableNames, char &suppress)
{

    node *root=new node;
    root->key_value=key;
    root->left=NULL;
    root->right=NULL;
    int currentLevel = 1;

    if (currentLevel < maxLevel) {
        createNewLevel(key, root, currentLevel, maxLevel, dataArray, method, variableNames, suppress);

    }
    //cout << "key is " << key.bestDirection << " " << key.bestThreshold << " current level is " << currentLevel << " maxLevel is " << maxLevel << endl;

    return root;
}
/*)
node *btree::search(int key)
{
  return search(key, root);
}
*/





int main() {

    string filename, filename2;

    int maxLevel;
    int numBoostingRounds;
    int currentLevel = 1;
    int method;
    char suppress;

    cout << "Enter the name of the training dataset:" << endl;
    cout << ">", cin >> filename;
    ifstream in(filename);
    fout << "Training dataset: " << filename << endl;

    cout << endl;
    cout << "Enter the name of the test dataset:" << endl;
    cout << ">", cin >> filename2;
    fout << "Test dataset: " << filename2 << endl;

     cout << endl;
    cout << "Detailed results will be stored in output.csv.\nSuppress listings of datasets? y or n:" << endl;
     cout << ">", cin >> suppress;


    // default mode is ios::out | ios::trunc
   //if (!fout) {
    //  cerr << "error: open file for output failed!" << endl;
    //  abort();  // in <cstdlib> header


 cout << endl;
    cout << "Enter the number of boosting rounds:" << endl;
    cout << ">", cin >> numBoostingRounds;
    fout << "Number of Boosting Rounds: " << numBoostingRounds << endl;

 cout << endl;
    cout << "Enter the number of tree levels (a level of one is a stump):" << endl;
    cout << ">", cin >> maxLevel;
    fout << "Max Number of Tree Levels: " << maxLevel << endl;

 cout << endl;
    cout << "Choose threshold discovery method number:\n1 Manual (brute force)\n2 Alternative manual using Sort\n3 Gini Impurity" << endl;
    cout << ">", cin >> method;
    fout << "Decision Tree Method: ";
    if (method == 1) {
        fout << "Manual (brute force)" << endl;
    }
    else if (method == 2) {
        fout << "Alternative manual using Sort" << endl;
    }
    else {
        fout << "Gini Impurity" << endl;
    }



    string line, field;
    vector<string> variableNames;
    vector<vector<double> > dataArray; // create 2D array to store our training dataset

    getline(in, line);          //takes from "in" file and stores into "line" until an end of line character is reached, this command by itself
                                //means only the top line is being stored, which we will make sure is the line with all feature names for our dataset.

    stringstream ss(line);  //sets up datatype that allows extraction of strings from our top line.

    while (getline(ss, field, ',')) {          //further divides up our line, now a getline is completed everytime ',' delimiter reached.
        variableNames.push_back(field);        //from our top line we store all feature names in field 1D vector now. "ss" is completed at the end of our
    }                                          //top line and the while command finishes there.



    while (getline (in, line)) { //gets the next line in file, this while command will act until there are no more lines to get

        //local variables inside this while command.
        string val;
        vector<double> row;
        stringstream ss(line);

        while (getline(ss, val,','))    //again separating the lines into comma delimited strings
        {
            row.push_back(stod(val));   //stod function converts numbers in string form into the datatype double. They are then pushed onto
        }                               //the row vector.

        dataArray.push_back(row);  //the line has been stored as a 1D array, we push each of these arrays onto our 2D array.
    }

    in.close();         //our dataset has been successfully stored, no need to have the file stream open anymore.



    int numFeatures = dataArray[0].size()-1;  //counts the number of columns which should be number of features.
                                              //the last column will be directed to be for class so is not part of feature count.

    int numDatapoints = dataArray.size();  //counts rows

    //cout << "full number of datapoints is " << numDatapoints << endl;


    double initialWeight = 1.0/numDatapoints;      //our initial weightings of each datapoint is uniform distribution.


    for (int h=0; h<numDatapoints; ++h) {
        dataArray[h].push_back(initialWeight);   //creating a weighting column on our array
    }

   /*for(int a=0; a<numDatapoints; ++a) {
      for(int b=0; b<numFeatures+2; ++b) {
         cout << dataArray[a][b] << " ";
      }
      cout << endl;
   }*/


    node *boostPointers[numBoostingRounds];
    double alpha;
    double storedAlphas[numBoostingRounds];

    auto start = std::chrono::high_resolution_clock::now();

   for (int boostNum=0; boostNum<numBoostingRounds; boostNum++) {

       cout << "\nBOOST ROUND " << boostNum+1 << endl << endl;
       fout << "\nBOOST ROUND " << boostNum+1 << endl << endl;

       node *root = NULL;

       bestThresReturns a;
      if (method == 1) {
         a =  discoverBestThresManual(dataArray);
       }
       else if (method == 2) {
         a =  discoverBestThresSorted(dataArray);
       }
       else {
        a =  discoverBestThresGini(dataArray);
       }

       cout << "At level 1, set size is " << numDatapoints << endl;
       cout << "Direction: " << a.bestDirection << endl;
       cout << "Feature: " << a.bestFeature << endl;
       cout << "Threshold: " << a.bestThreshold << endl;
       cout << "Error: " << a.minimumError << endl;
       fout << "At level 1 set size is " << numDatapoints << endl;
       fout << "Direction: " << a.bestDirection << endl;
       fout << "Feature: " << a.bestFeature << endl;
       fout << "Threshold: " << a.bestThreshold << endl;
       fout << "Error: " << a.minimumError << endl;

       root = createRootLevel(a, maxLevel, dataArray, method, variableNames, suppress);   //will create entire binary tree of levels by recursive calls

       boostPointers[boostNum] = root;

        /*
        for(int a=0; a<numBoostingRounds; ++a) {

            cout << boostPointers[a]->key_value.bestThreshold << " ";
        }
        cout << endl;
        //best threshold info being stored correctly
        */




        //using the best threshold we've found to classify our dataset
        int predictArray[numDatapoints];

        for (int i=0; i<numDatapoints; i++) {

            findPredictions(dataArray, predictArray, root, i, numFeatures, numDatapoints);

        }

       /* cout << "Actual\t\tPrediction Array" << endl;
    //for (int m=0; m<numDatapoints; m++) {

      //  cout << dataArray[m][numFeatures] << "\t\t" << predictArray[m] << endl;
   // }

    int correct_count = 0;
    for (int m=0; m<numDatapoints; m++) {
            if (predictArray[m] == dataArray[m][numFeatures]) {
                correct_count++;
            }
    }

    cout << "Misclassication Proportion without boosting is " << 1.0 - ((double)correct_count/(double)numDatapoints) << endl; */

        //everytime we've calculated error rate previously it's been for building each level of our decision tree, now we need to find overall
        //error rate for the entire decision tree classifier and use it to calculate it's alpha.

        //if there's only one level on the decision tree we already have our error rate stored and can save some processing.
        //Is it the same? check. Just use 2nd one for level ones also.

        double errorRate = 0.0;

        if(maxLevel == 1) {
            errorRate = root->key_value.minimumError;       //if only 1 level we already have our error stored in the root node of this boost round
        }

        else {

            int comparisonArray[numDatapoints];

            for (int p =0; p<numDatapoints; ++p) {
                if(predictArray[p] == dataArray[p][numFeatures]){
                    comparisonArray[p] = 0;
                }
                else {
                    comparisonArray[p] = 1;         //after comparing our predictions from this threshold to actual values
                }                                   //we will have a comparison array where 1s are our errors, 0s are our matches.
            }

            double partialSum;

            for (int q =0; q<numDatapoints; ++q) {
                  partialSum = comparisonArray[q]*(dataArray[q][numFeatures+1]);     //because of different datapoint weightings, finding error proportion
                  errorRate = errorRate + partialSum;                               //needs to be done datapoint by datapoint.
            }
        }

        //cout << "error rate with weightings is " << errorRate << endl; //calculating error rate



                //need this offset from zero since calculating alpha is impossible otherwise..


        if (errorRate == 0.0) {
            alpha = 0.5*(log((1.0 - errorRate + 0.00001)/(errorRate + 0.00001)));
        }
        else {
            alpha = 0.5*(log((1.0 - errorRate)/(errorRate)));
        }

        cout << "\nRound Alpha is " << alpha << endl;
        fout << "\nRound Alpha is " << alpha << endl;

        storedAlphas[boostNum] = alpha;

        double sumWeights = 0.0;
        for (int i=0; i< numDatapoints; i++) {//updating the weights, all this takes is alpha and actual vs predicted because
                                                            //classes already encoded into 1s and -1s.
            dataArray[i][numFeatures+1] = dataArray[i][numFeatures+1] * exp((-alpha)*dataArray[i][numFeatures]*predictArray[i]);
            sumWeights = sumWeights + dataArray[i][numFeatures+1];
        }

        for (int i=0; i< numDatapoints; i++) {   //normalise the weights
            dataArray[i][numFeatures+1] = dataArray[i][numFeatures+1]/sumWeights;

        }

    /*destroy_tree(root);

    if (root==NULL){
        cout << "our pointer is NULL" << endl;
    }*/

   }//end boosting loop

  /* for(int a=0; a<numDatapoints; ++a) {
       for(int b=0; b<numFeatures+2; ++b) {
          cout << dataArray[a][b] << " ";
    }
     cout << endl;
  } */


    ifstream in2(filename2);


    //ifstream in2("dataset.csv");
    string line2, field2;

    vector<string> variableNames2;

    vector< vector<double> > data2Array;
    getline (in2, line2);
    stringstream ss2(line2);

    while (getline(ss2, field2, ',')) {
        variableNames2.push_back(field2);
    }





    while (getline (in2,line2) )    // get next line in file
    {

        string val;
        vector<double> row;
        stringstream ss2(line2);

        while (getline(ss2, val,','))  // break line into comma delimitted fields
        {
            row.push_back(stof(val));  // add each field to the 1D array
        }

        data2Array.push_back(row);  // add the 1D array to the 2D array
    }

    in2.close();


    numFeatures = data2Array[0].size()-1;
    numDatapoints = data2Array.size();

   /* for (int i=0; i<numDatapoints; i++) {
        for (int j=0; j<numFeatures; j++) {
            cout << data2Array[i][j] << " ";
        }
        cout << endl;
    } */

    int predict2Array[numDatapoints];
    double classifierSumArray[numDatapoints];

    for (int i=0; i<numDatapoints; i++) {
        classifierSumArray[i] = 0.0;
    }

    int finalClassification[numDatapoints];

    //boosting array 0 minimumError, 1 bestThreshold, 2 bestDirection, 3 bestFeature, 4 alpha
    for (int i=0; i<numBoostingRounds; i++) {
        for (int j=0; j<numDatapoints; j++) {
            findPredictions(data2Array, predict2Array, boostPointers[i], j, numFeatures, numDatapoints);   //predict2Array passed by reference
        }//end datapoint loop, have a predict array at end for the boost round

        for (int k=0; k<numDatapoints; k++)
        classifierSumArray[k] =  classifierSumArray[k] + storedAlphas[i] * predict2Array[k];

    }//end boosting round loop
    for (int m=0; m<numDatapoints; m++) {
            if (classifierSumArray[m] >= 0) {
                finalClassification[m] = 1;
            }
            else {
                finalClassification[m] = -1;

            }
    }
    int misClass[numDatapoints];
    int correct_count = 0;
    for (int m=0; m<numDatapoints; m++) {
            if (finalClassification[m] == data2Array[m][numFeatures]) {
                correct_count++;
                misClass[m] = 0;
            }
            else {
                misClass[m] = 1;
            }
    }






        for(int a=0; a<numFeatures; ++a) {
            fout << variableNames2[a] << ",";
        }
        fout << "Class,Predicted,Misclass" << endl;


        for(int a=0; a<numDatapoints; ++a) {
            for(int b=0; b<numFeatures+1; ++b) {
                fout << data2Array[a][b] << ",";
            }
            fout << finalClassification[a] << ",";
            fout << misClass[a] << endl;

        }



    cout << "\nAlphas for each classifier" << endl;
    cout << "Round\tAlpha" << endl;
    fout << "\nAlphas for each classifier" << endl;
    fout << "Round,Alpha" << endl;

    for (int i=0; i<numBoostingRounds; ++i) {
        cout << i+1 << "\t" << storedAlphas[i] << endl;
        fout << i+1 << "," << storedAlphas[i] << endl;
    }



    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;

    cout << "\nElapsed time for method: " << elapsed.count() << " s\n" << endl;
    cout << "Misclassication Proportion is " << 1.0 - ((double)correct_count/(double)numDatapoints) << endl;
    fout << "\nElapsed time for method: " << elapsed.count() << " s\n" << endl;
    fout << "Misclassication Proportion is " << 1.0 - ((double)correct_count/(double)numDatapoints) << endl;


  /*  for (int i=0; i<numDatapoints; i++) {
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
     } */



   }// end main









      //vector<vector<double> > *test;    //this is how to create and set pointers to arrays
      //test = &dataArray;

     /* for(int a=0; a<numDatapoints; ++a) {
        for(int b=0; b<numFeatures+2; ++b) {
            cout << (*test)[a][b] << " ";           //how to treat a pointer like an array with index access
        }
        cout << endl;
    }
*/







