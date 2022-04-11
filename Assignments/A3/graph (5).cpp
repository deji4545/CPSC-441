// Name - Ayodeji Osho
// Class - CPSC 441 T07
// Student ID -30071771


// Reference - Read File Code
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <map>
#include <vector>

#include <stdio.h>
#include <stdlib.h>

using namespace std;
map<string, int> airport_name;
const int ARRSIZE = 21;
int graphMatrix[ARRSIZE][ARRSIZE];
int graphPath[ARRSIZE][ARRSIZE];

// Converts the canadamap.txt file to a matrix
int createMatrix()
{

    //Initialize all elements of 2d array to 0
    for (int i = 0; i < ARRSIZE; i++)
    {
        for (int j = 0; j < ARRSIZE; j++)
        {
            graphMatrix[i][j] = 0;
            graphPath[i][j] = 0;
        }
    }

    // Reference To Read Files - https://www.w3schools.com/cpp/cpp_files.asp
    string myText2;
    ifstream MyReadFile2("canadamap.txt");

    int count = 0;

    while (getline(MyReadFile2, myText2))
    {
        
        string word = "";
        int i;
        int j;
        int theBool = true;

        // Reference to split a string into a sentence - https://www.geeksforgeeks.org/split-a-sentence-into-words-in-cpp/
        for (auto x : myText2)
        {
            if (x == ' ')
            {
                //Give each aiport name a unique number from 1 to 21;
                if (airport_name[word] == 0)
                {
                    count = count + 1;
                    airport_name[word] = count;
                }
                

                if (theBool == true)
                {
                    i = airport_name[word] - 1;
                    theBool = false;
                }
                else
                {
                    j = airport_name[word] - 1;
                }

                word = "";
            }
            else
            {
                word = word + x;
            }
        }
        int distance = stoi(word); // distance between two edges
        
        graphMatrix[i][j] = distance; // distance > 0 represent an edfge
        graphMatrix[j][i] = distance;
    }

    MyReadFile2.close();

    return 0;
}

// Takes a number assigned to an airport name and retrieve the string from the hashmap
string code_to_airtport_name(int airport_code)
{
    for (auto i : airport_name)
        if (i.second == airport_code)
        {
            return i.first;
        }
    return "AAA";
}

// global variables used by multiple functions
map<int, int> airport_distance_map; // stores the shortest distance from the source vertex
vector <int> vect_Arr[ARRSIZE]; //An array of vectors that stores the path from source vertex to every node
vector<int> discover_node_vect; //set of nodes whose least-cost-path are definitely known

// Checks if a node is in the set of nodes whose least-cost-path are definitely known
bool discoverNode(int val)
{
    for (auto item : discover_node_vect)
    {
        if (val == item)
        {
            return true;
        }
    }

    return false;
}


// Called by djikstra algoithm to update shortest distance for a vertex
// The paths saved in vect_Arr are also updated as well
int update_distance(int node)
{
    for (int j = 0; j < ARRSIZE; j++)
    {
        int distance = graphMatrix[node][j];
        if (distance != 0 && j != node)
        {
            if (airport_distance_map[node] + distance <= airport_distance_map[j])
            {
                airport_distance_map[j] = airport_distance_map[node] + distance;

                vect_Arr[j].clear();
                  for (auto i = vect_Arr[node].begin(); i != vect_Arr[node].end(); ++i){
                      vect_Arr[j].push_back(*i);
                  }
                vect_Arr[j].push_back(j);
            }
        }
    }
    return 0;
}

// Runs the shortest distance algorithm
int dijkstra(){

    int initial_node =0;
    int end;
    int minimum = 5000;
    vect_Arr[0].push_back(0);

    for (int k = 0; k < ARRSIZE; k++)
    {
        airport_distance_map[k] = 5000; //Infinite Distance
    }
    airport_distance_map[0] = 0;
    

    while (discover_node_vect.size() < 21){
         update_distance(initial_node);

         int previous_discovered_node = initial_node;

        for (auto i : airport_distance_map){
        if (i.second < minimum && discoverNode(i.first) == false)
        {
            minimum = i.second;
            initial_node = i.first;            
        }
        }

        discover_node_vect.push_back(initial_node);

        minimum = 5000;
    }
    return 0;
}

// Displays all shorthest path
int display_path(){

    cout << "\nShortest Path From YYC To Every Airport Destination:\n" << endl;
    int vect_size = 0;
    int count =0;
    
    for (size_t num = 0; num < ARRSIZE; num++)
    {
        
        for (auto k = vect_Arr[num].begin(); k != vect_Arr[num].end(); ++k){
            vect_size++;
            cout << code_to_airtport_name(*k + 1);
            if(vect_size != vect_Arr[num].size()){
                cout << "-->";
            }
        }

        for(int p =0; p < vect_Arr[num].size(); p++){
            if(p+1 < vect_Arr[num].size()){
                 int row = vect_Arr[num].at(p);
                 int col = vect_Arr[num].at(p+ 1);
                 count = count + graphMatrix[row][col];
            }
        }



        cout << "   " << count <<"\n";
        vect_size = 0;
        count =0;
        
    } 

    return 0;

}

int main()
{
    createMatrix();
    dijkstra();
    display_path();
    
    return 0;
}
