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
    int degree;
};

struct edge{
    int v1;
    int v2;
    int length;
    bool used;
};

/*
int* MST(int* graph[], const int& rows, const int& cols)
{

}
*/

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
 ** Function: nearestNeighbor
 ** Description:
 ** Parameters: vector<struct city> cities
 ***************************************************************************************/
vector<int> nearestNeighbor (vector<struct city> cities){
    int numCities = cities.size();
    int totalDist = 0;
    int closestCityDist = INT_MAX;
    int currentCityId = 0;
    int closestCityId;
    vector<int> optTour;



    optTour.push_back(currentCityId);
    cities[currentCityId].degree = 2;



    for (int i = 0; i < numCities - 1; i++){
        closestCityDist = INT_MAX;

        for(int j = 0; j < numCities; j++){
            if(cities[j].degree == 0 && dist(cities[j],cities[currentCityId]) < closestCityDist){
                closestCityId = j;
                closestCityDist = dist(cities[j],cities[currentCityId]);
            }
        }

        cities[closestCityId].degree += 2;
        optTour.push_back(closestCityId);
        totalDist += closestCityDist;
        currentCityId = closestCityId;
        cout << "Closest City Distance: " << closestCityDist << endl;
    }

    optTour.push_back(0);

    cout << "Closest City Distance: " << dist(cities[0],cities[currentCityId]) << endl;
    totalDist += dist(cities[0],cities[currentCityId]);
    optTour.push_back(totalDist);
    return optTour;
}


/***************************************************************************************
 ** Function: greedyEdge
 ** Description:
 ** Parameters: vector<struct city> cities
 ***************************************************************************************/
vector<int> greedyEdge (vector<struct city> cities){
    int numCities = cities.size();
    vector <struct edge> edges;
    vector <int> optTour;
    struct edge currentEdge;
    currentEdge.used = false;
    struct edge tempEdge;

    //Use cities vector to construct list of all edges with lengths
    for(int i = 0; i < numCities; i++){

        for (int j = i+1; j < numCities; j++){
            currentEdge.v1 = i;
            currentEdge.v2 = j;
            currentEdge.length = dist(cities[i],cities[j]);
            edges.push_back(currentEdge);
        }

    }

    //for testing only - print edges vector
    for(int i = 0; i < (int) edges.size(); i++){
        cout << edges[i].v1 << " " << edges[i].v2 << " " << edges[i].length << " " << edges[i].used << endl;
    }
    cout << endl;

    //use sort to order edge list in ascending order of length; currently insertion sort - change to merge
    for(int i = 0; i < (int)edges.size(); i++){
        for (int j = i+1; j < (int)edges.size(); j++){
            if (edges[j].length < edges[i].length){
                tempEdge = edges[i];
                edges[i] = edges[j];
                edges[j] = tempEdge;
            }
        }
    }

    //for testing only - print edges vector
    for(int i = 0; i < (int) edges.size(); i++){
        cout << edges[i].v1 << " " << edges[i].v2 << " " << edges[i].length << " " << edges[i].used << endl;
    }
    cout << endl;

    int usedEdgeCount = 0;
    while (usedEdgeCount < numCities){
        for(int i = 0; i < (int)edges.size(); i++){
            //Check that edge has not be used and then degree of both incident vertices is less than 2
            if(edges[i].used == false
               && cities[edges[i].v1].degree < 2
               && cities[edges[i].v2].degree < 2){
                //need to check that using edge does not create cycle of length less than n
                //use modified DFS to check for cycles. Only need to check the component of graph
                //that most recent edge was added to.
                edges[i].used = true;
                usedEdgeCount++;
            }
        }
    }

    //Now edges used in tour are all marked true. Follow edges to construct list of city ids in order
    //push to optTour, compute total distance and return.
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
    vector<int> optTour;
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
//    baseName = fileName.substr(0, fileName.find_last_of("."));
    outputFile = baseName + ".tour";

    ifstream inFile;
    inFile.open(fileName.c_str());

    //Validates that input file can be opened.
    if (!inFile.is_open()){
        cout << "Error: Could not open file." << endl << endl;
        return 1;
    }

    ofstream outFile;
    outFile.open(outputFile.c_str());

    while (inFile >> inputValue){
        inputCity.id = inputValue;
        inFile >> inputCity.x;
        inFile >> inputCity.y;
        inputCity.degree = 0;
        cities.push_back(inputCity);
    }



    //construct adjacency matrix of graph
    /*
    const int ROWS(10), COLS(10);
    int citiesGraph[ROWS][COLS];
    for (int i = 0; i < 10; i++){
        for (int j = 0; j < 10; j++){
            citiesGraph[i][j] = dist(cities[i],cities[j]);
        }
    }
    int* citiesGraph_p[ROWS];
	*citiesGraph_p = citiesGraph[0];
    */


    //greedyEdge(cities);

    /*
    cout << "Cities:" << endl;
    for(int i = 0; i < (int)cities.size(); i++)
        cout << cities[i].id << " " << cities[i].x << " " << cities[i].y << endl;

    optTour = nearestNeighbor(cities);
    cout << endl << "Distance: " << optTour.back() << endl << "Order: ";
    for(int i = 0; i < (int)optTour.size(); i++)
        cout << optTour[i] << " ";
    cout << endl;
    /*


    /*
    optTour = bruteForce(cities);
    cout << endl << "Distance: " << distTour(cities,optTour) << endl << "Order: ";
    for(int i = 0; i < (int)cities.size(); i++)
        cout << optTour[i] << " ";
    cout << endl;
    */

    inFile.close();
    outFile.close();

    return 0;
}
