#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;


int main()
{
    string filename;

    cout << "Enter a filename please: " << endl;
    cin >> filename;
    ifstream in(filename.c_str());

    //ifstream in("dataset.csv");

    string line, field;

    vector<string> variableNames;

    vector< vector<int> > array;  // the 2D array
                   // array of values for one line only

    getline (in, line);
    stringstream ss(line);
    while (getline(ss, field, ',')) {
        variableNames.push_back(field);
    }





    while (getline (in,line) )    // get next line in file
    {

        string val;
        vector<int> row;
        stringstream ss(line);

        while (getline(ss, val,','))  // break line into comma delimitted fields
        {
            row.push_back(stoi(val));  // add each field to the 1D array
        }

        array.push_back(row);  // add the 1D array to the 2D array
    }

    // print out what was read in

    for (size_t i=0; i<array.size(); ++i)
    {
        for (size_t j=0; j<array[i].size(); ++j)
        {
            cout << array[i][j] << " "; // (separate fields by |)
        }
        cout << "\n";
    }

    cout << array[0][1]+array[1][1] << endl;

    cout << variableNames[0] << endl;



    return 0;
}

#changed!

