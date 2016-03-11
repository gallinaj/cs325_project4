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
    int neighbor1;
    int neighbor2;
};

struct edge{
    int v1;
    int v2;
    int length;
    bool used;
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
 ** Function: nearestNeighbor
 ** Description:
 ** Parameters: vector<struct city> cities
 ***************************************************************************************/
vector<int> nearestNeighbor (vector<struct city> cities){
    int numCities = cities.size();  // Number of cities used.
    int totalDist = 0;              // Total distance of the tour
    int closestCityDist = INT_MAX;
    int currentCityId = 0;
    int closestCityId;
    vector<int> optTour; // List of city ids in order of visiting

    // Start tour at city 0. Push onto optTour vector as starting location.
    optTour.push_back(currentCityId);
    cities[currentCityId].degree = 2;

    // Perform iterations of adding nearest unused neighbor as next stop on tour a
    // total of numCities times.
    for (int i = 0; i < numCities - 1; i++){
        closestCityDist = INT_MAX;

        // Find the closest city to the current city. Store id of closest city in closestCityId
        // and distance to closest city in closestCityDist
        for(int j = 0; j < numCities; j++){
            if(cities[j].degree == 0 && dist(cities[j],cities[currentCityId]) < closestCityDist){
                closestCityId = j;
                closestCityDist = dist(cities[j],cities[currentCityId]);
            }
        }

        //Increase the degree of closest city to show it was visited
        cities[closestCityId].degree += 2;

        //Add the closest city as the next city on tour and update total distance
        optTour.push_back(closestCityId);
        totalDist += closestCityDist;
        currentCityId = closestCityId;
    }

    totalDist += dist(cities[0],cities[currentCityId]);     //update total distance
    optTour.push_back(totalDist); //add total distance to back of optTour and return
    return optTour;
}


/***************************************************************************************
 ** Function: greedyEdge
 ** Description:
 ** Parameters: vector<struct city> cities
 ***************************************************************************************/
vector<int> greedyEdge (vector<struct city> cities){
    int numCities = cities.size();  // number of cities
    vector <struct edge> edges;     // list of all edges
    vector <int> optTour;           // optimal tour found
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

    int usedEdgeCount = 0;
    int lastEdgeIndex = -1;
    int currentCity;
    int nextCity;

    while (usedEdgeCount < numCities - 1){
        for(int i = lastEdgeIndex + 1; i < (int)edges.size(); i++){
            //Check that edge has not be used and then degree of both incident vertices is less than 2
            if(edges[i].used == false
               && cities[edges[i].v1].degree < 2
               && cities[edges[i].v2].degree < 2){

                //need to check that using edge does not create cycle of length less than n
                //use modified DFS to check for cycles. Only need to check the component of graph
                //that most recent edge was added to.
                bool isCycle = false;
                if(cities[edges[i].v1].degree == 1 && cities[edges[i].v2].degree == 1){
                    currentCity = edges[i].v1;
                    nextCity = cities[currentCity].neighbor1;
                    //int cycleLength = 0;
                    do{
                        if (cities[nextCity].neighbor1 == edges[i].v2 || cities[nextCity].neighbor2 == edges[i].v2){
                            isCycle = true;
                            break;
                        }

                        if (cities[nextCity].neighbor1 == -1 || cities[nextCity].neighbor2 == -1){
                            break;
                        }

                        int temp = nextCity;
                        if(cities[nextCity].neighbor1 == currentCity)
                            nextCity = cities[nextCity].neighbor2;
                        else
                            nextCity = cities[nextCity].neighbor1;
                        currentCity = temp;

                    } while (true);


                }

                if(isCycle)
                    continue;

                edges[i].used = true;
                cities[edges[i].v1].degree++;
                cities[edges[i].v2].degree++;

                if (cities[edges[i].v1].neighbor1 == -1)
                    cities[edges[i].v1].neighbor1 = edges[i].v2;
                else
                    cities[edges[i].v1].neighbor2 = edges[i].v2;

                if (cities[edges[i].v2].neighbor1 == -1)
                    cities[edges[i].v2].neighbor1 = edges[i].v1;
                else
                    cities[edges[i].v2].neighbor2 = edges[i].v1;

                usedEdgeCount++;
                lastEdgeIndex = i;
                break;
            }
        }
    }

    int lastCity1 = -1, lastCity2 = -1;
    for (int i=0; i < numCities; i++)
    {
        if(cities[i].neighbor2 == -1){
            if (lastCity1 == -1)
                lastCity1 = i;
            else  {
                lastCity2 = i;
                break;
            }
        }
    }
    cities[lastCity1].neighbor2 = lastCity2;
    cities[lastCity2].neighbor2 = lastCity1;
    cities[lastCity1].degree++;
    cities[lastCity2].degree++;


    for (int i=0; i < (int)edges.size(); i++) {
        if (edges[i].v1 == lastCity1 && edges[i].v2 == lastCity2){
            edges[i].used = true;
            break;
        }
    }

    currentCity = 0;
    nextCity = cities[currentCity].neighbor1;
    optTour.push_back(currentCity);

    for(int i = 0; i < numCities; i++){
        if (cities[nextCity].neighbor1 != currentCity){
            currentCity = nextCity;
            nextCity = cities[nextCity].neighbor1;
        }
        else {
            currentCity = nextCity;
            nextCity = cities[nextCity].neighbor2;
        }
        optTour.push_back(currentCity);
    }
    //Now edges used in tour are all marked true. Follow edges to construct list of city ids in order
    //push to optTour, compute total distance and return.
    optTour.push_back(distTour(cities,optTour));
    return optTour;
}


/***************************************************************************************
 ** Function: main
 ** Description: driver function to call algorithms on input
 ** Parameters:
 ***************************************************************************************/
int main(int argc, char *args[]){
    string fileName, outputFile, inputStr;
    vector<struct city> cities;
    struct city inputCity;
    int inputValue;

    if (argc > 1){
        fileName = args[1];
    }
    else{
        cout << "Please enter a string." << endl << endl;
        return 1;
    }

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

    // read each line of file, create city structs, and push them to cities vector
    while (inFile >> inputValue){
        inputCity.id = inputValue;
        inFile >> inputCity.x;
        inFile >> inputCity.y;
        inputCity.degree = 0;
        inputCity.neighbor1 = -1;
        inputCity.neighbor2 = -1;
        cities.push_back(inputCity);
    }
    inFile.close();


    //allocate memory for adjacency matrix of graph
    //const int ROWS(10), COLS(10);
    int** citiesGraph;
    citiesGraph = new int* [10];
    for (int i = 0; i < 10; i++)
        citiesGraph[i] = new int[10];

     //Enter values for adjacency matrix of graph
    for (int i = 0; i < 10; i++){
        for (int j = 0; j < 10; j++){
            citiesGraph[i][j] = dist(cities[i],cities[j]);
        }
    }

    /*
    vector<int> optTour = bruteForce(cities);
    cout << "Brute Force" << endl << "Distance: " << optTour.back() << endl << "Order: ";
    for(int i = 0; i < (int)cities.size(); i++)
        cout << optTour[i] << " ";
    cout << endl << endl;
    */

    /*
    optTour = nearestNeighbor(cities);
    cout << "Nearest Neighbor" << endl << "Distance: " << optTour.back() << endl << "Order: ";
    for(int i = 0; i < (int)cities.size(); i++)
        cout << optTour[i] << " ";
    cout << endl << endl;
    */

    vector<int> optTour = greedyEdge(cities);
    outFile << optTour.back() << endl;
    for(int i = 0; i < (int)cities.size(); i++)
        outFile << optTour[i] << endl;

    outFile.close();
    return 0;
}
