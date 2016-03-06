// CS 325 - Project 4: TSP
// Group 30
// Courtney Bonn, Jana Gallina, Jacob McMillen

#include<iostream>
#include<climits>
#include<sstream>
#include<fstream>
#include<string>
#include<vector>

using std::ifstream;
using std::ofstream;
using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::vector;

int main(int argc, char *args[]){
    string fileName, outputFile, inputStr;

    if (argc > 1){
        fileName = args[1];
    }
    else{
        cout << "Please enter a filename." << endl;
        cin >> fileName;
        cout << endl;
        return 1;
    }

    outputFile = fileName + ".tour";


    return 0;
}
