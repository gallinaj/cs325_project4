// CS 325 - Project 4: Traveling Sales Person
// Group 30
// Courtney Bonn, Jana Gallina, Jacob McMillen

#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include <cmath>

#include <climits>
#include <fstream>
using std::ifstream;
using std::ofstream;

#include<string>
using std::string;

#include<vector>
using std::vector;

#include<algorithm>
using std::min;


struct city{
    int id;
    int x;
    int y;
};

/***************************************************************************************
 ** Function: dist
 ** Description: Applies standard distance formula to coordinates of two input cities
 ** Parameters: struct city c1, struct city c2
 ***************************************************************************************/
int dist(struct city c1, struct city c2){
    return round(sqrt(pow((c2.x-c1.x),2) + pow((c2.y-c1.y),2)));
}

/***************************************************************************************
 ** Function: distTour
 ** Description: Given a vector of cities and ordering vector return length of tour
 ** Parameters: vector<struct city> cities, vector<int> order
 ***************************************************************************************/
int distTour(vector<struct city> cities, vector<int> order){
    int totalDistance = 0;
    int numCities = cities.size();

    for (int i = 0; i < numCities - 1; i++)
        totalDistance += dist(cities[order[i]],cities[order[i+1]]);

    totalDistance += dist(cities[order[numCities - 1]],cities[order[0]]);
    return totalDistance;
}

/***************************************************************************************
 ** Function: bruteForce
 ** Description: Given vector of cities, checks all permutations and returns the one with
 **              the minimum distance
 ** Parameters: vector<struct city> cities
 ***************************************************************************************/
vector<int> bruteForce (vector<struct city> cities){
    int numCities = cities.size();
    int minDist = INT_MAX;
    int tempDist;

    vector<int> currentTour;    //Vector giving order of current tour under consideration
    vector<int> optTour;        //Copy of current shortest tour

    // initialize currentTour and optTour to [0,1,2,...,numCities-1]
    for(int i = 0; i < numCities; i++){
        currentTour.push_back(i);
        optTour.push_back(i);
    }

    // run through all permutations of currentTour, find distance of each and store min
    do {
        tempDist = distTour(cities,currentTour);
        if (tempDist < minDist){
            minDist = tempDist;
            for(int i = 0; i < numCities; i++)
                optTour[i] = currentTour[i];
        }
    } while(std::next_permutation(currentTour.begin(),currentTour.end()));

    optTour.push_back(minDist);
    return optTour;
}


/***************************************************************************************
 ** Function: main
 ** Description: driver function to call algorithms on input
 ** Parameters:
 ***************************************************************************************/
int main(int argc, char *args[]){
    string fileName, baseName, outputFile, inputStr;
    vector<struct city> cities;
    struct city inputCity;
    int inputValue;
/*
    if (argc > 1){
        fileName = args[1];
    }
    else{
        cout << "Please enter a string." << endl << endl;
        cin >> fileName;
    }
*/
    fileName = "a.txt";
    //baseName = fileName.substr(0, fileName.find_last_of("."));
    outputFile = fileName + ".tour";

    ifstream inFile;
    inFile.open(fileName.c_str());

    //Validates that input file can be opened.
    if (!inFile.is_open())
    {
        cout << "Error: Could not open file." << endl << endl;
        return 1;
    }

    ofstream outFile;
    outFile.open(outputFile.c_str());

    while (inFile >> inputValue){
        inputCity.id = inputValue;
        inFile >> inputCity.x;
        inFile >> inputCity.y;
        cities.push_back(inputCity);
    }

    inFile.close();
    outFile.close();

    cout << "Cities:" << endl;
    for(int i = 0; i < (int)cities.size(); i++)
        cout << cities[i].id << " " << cities[i].x << " " << cities[i].y << endl;

    vector<int> optTour = bruteForce(cities);
    cout << endl << "Distance: " << distTour(cities,optTour) << endl << "Order: ";
    for(int i = 0; i < (int)cities.size(); i++)
        cout << optTour[i] << " ";
    cout << endl;

    return 0;
}
