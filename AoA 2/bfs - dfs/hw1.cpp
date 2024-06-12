/**
 * BLG 336E - Analysis of Algorithms II
 * Assignment 1
 * Berkay Sancı
 * 504231561
*/

/* * * * * * * * * * * * * * * * * *
 * DO NOT MODIFY UNLESS REQUESTED  *
* * * * * * * * * * * * * * * * * */

#include <iostream>         
#include <vector>
#include <fstream>
#include <sstream>       
#include <stack>
#include <queue>
#include <chrono>       // For time measurement

/* INCLUDE ANY OTHER NECESSARY LIBRARIES HERE */
/* START YOUR CODE HERE */
#include<bits/stdc++.h>

/* END YOUR CODE HERE */

using namespace std;
using namespace std::chrono;

/* USE HERE IF YOU NEED TO DEFINE EXTRA FUNCTIONS */
/* START YOUR CODE HERE */
bool compare_resourcetypes(const pair<int, int>& a, const pair<int, int>& b) {
    if (a.first != b.first) {
        return a.first > b.first; // sort first values in descending order
    }
    return a.second < b.second; // if first values are equal, sort second values in ascending order
}

/* END YOUR CODE HERE */

/**
 * Perform Depth-First Search (DFS) traversal on a map.
 * 
 * @param map The map represented as a 2D vector of integers.
 * @param row The starting row index for the DFS traversal.
 * @param col The starting column index for the DFS traversal.
 * @param resource The resource value to search for during the traversal.
 * @return The size of the colony found during the DFS traversal.
 */
int dfs(vector<vector<int>>& map, int row, int col, int resource) {

    /* START YOUR CODE HERE */
    int m = map.size();     //number of total rows
    int n = map[0].size();  //number of total columns
    unsigned int i;         
    int size=0; //size of the colony
    vector<pair<int, int>> directions{{0,1},{0,-1},{1,0},{-1,0}}; //to check adjacent cells (up,down,right,left)
  
    stack<pair<int,int>> index_stack;   //to keep the coordinates of the valid cells
    if(row < 0 || row >= m || col < 0 || col >= n || map[row][col]==-1 || map[row][col]!=resource) //for the starting cell, if these are the cases return size=0
    {
        return 0;
    }
    index_stack.push({row,col}); //push the coordinate pair into stack
    map[row][col]=-1; //make the cell visited
    while(!index_stack.empty()){
        pair< int,  int> curr = index_stack.top(); //process the top element
        int row_index = curr.first; 
        int column_index = curr.second;
        index_stack.pop(); //pop the element
        size++;
        for (i=0; i<directions.size(); i++) //checking all adjacent cells 
        {
            int new_row_index = (row_index + directions[i].first+m)%m; //row of each adjacent cell (modulo operator used since the map is circular)
            int new_column_index = (column_index + directions[i].second+n)%n; //column of each adjacent cell (modulo operator used since the map is circular)
            if ( map[new_row_index][new_column_index]==resource)    //if the cell has the same value
            {
                index_stack.push(make_pair(new_row_index, new_column_index));   //push to the stack
                map[new_row_index][new_column_index]=-1;        //make cell visited
            }
        } 
    }
    return size;

    /* END YOUR CODE HERE */

}


/**
 * Perform Breadth-First Search (BFS) traversal on a map.
 * 
 * @param map The map represented as a 2D vector of integers.
 * @param row The starting row index for the BFS traversal.
 * @param col The starting column index for the BFS traversal.
 * @param resource The resource value to search for during the traversal.
 * @return The size of the colony found during the BFS traversal.
 */
int bfs(vector<vector<int>>& map, int row, int col, int resource) {
    
    /* START YOUR CODE HERE */
    int m = map.size();     //number of total rows
    int n = map[0].size();      //number of total columns
    unsigned int i;
    int size=0;     //size of the colony
    vector<pair<int,int>> directions{{0,1},{0,-1},{1,0},{-1,0}};        //to check adjacent cells (up,down,right,left)
    queue<pair<int,int>> index_queue;       //to keep the coordinates of valid cells
    if(row < 0 || row >= m || col < 0 || col >= n || map[row][col]==-1 || map[row][col]!=resource)      //for the starting cell, if these are the cases return size=0
    {
        return 0;
    }  

    index_queue.push(make_pair(row,col));       //push the coordinate pair into queue
    map[row][col]=-1;       //make cell visited

    while (!index_queue.empty())
    {
        int row_index = index_queue.front().first;      //process the element in the front
        int column_index = index_queue.front().second;
        index_queue.pop();      //pop the element
        size++;     //increment size

        for (i=0; i<directions.size(); i++)     //check all adjacent cells
        {
            int new_row_index = (row_index + directions[i].first+m)%m;      //row of each adjacent cell (modulo operator used since the map is circular)
            int new_column_index = (column_index + directions[i].second+n)%n;       //column of each adjacent cell (modulo operator used since the map is circular)
            if ( map[new_row_index][new_column_index]==resource)         //if the cell has the same value
            {
                index_queue.push(make_pair(new_row_index, new_column_index));       //push to the queue
                map[new_row_index][new_column_index]=-1;        //make cell visited
            }
        } 
    }
    return size;

    /* END YOUR CODE HERE */

}

/**
 * Finds the top-k largest colonies in a given map.
 *
 * @param map The map represented as a 2D vector of integers.
 * @param useDFS A boolean flag indicating whether to use Depth-First Search (DFS) or Breadth-First Search (BFS) algorithm.
 * @param k The number of top colonies to find.
 * @return A vector of pairs representing the size and resource type of the top-k largest colonies.
 */
vector<pair<int, int>> top_k_largest_colonies(vector<vector<int>>& map, bool useDFS, unsigned int k) {

    auto start = high_resolution_clock::now();      // Start measuring time
    
    /* START YOUR CODE HERE */
    if(map.empty()){
        return {};
    }

    int m = map.size();
    int n = map[0].size();
    int i,j;
    size_t comp_i;
    vector<pair<int,int>> resource_size_pair;


    if (useDFS){
        for(i=0; i<m; i++){
            for(j=0; j<n; j++){
                if(map[i][j]!=-1){
                    int resource=map[i][j];     //resource is the value of map[i][j]
                    resource_size_pair.push_back({dfs(map,i,j,resource),resource});     //apply dfs with the coordinates and resource until all cells are visited
                                                                                        //and push back size,resource pairs into result vector
                }
            }
        }

    }

    else{
        for(i=0; i<m; i++){
            for(j=0; j<n; j++){
                if(map[i][j]!=-1){
                    int resource=map[i][j];     //resource is the value of map[i][j]
                    resource_size_pair.push_back({bfs(map,i,j,resource),resource});     //apply bfs with the coordinates and resource until all cells are visited
                                                                                        //and push back size,resource pairs into result vector
                }
            }
        }
    }
    



    sort(resource_size_pair.begin(), resource_size_pair.end(),compare_resourcetypes);   //sort result vector

    for(comp_i=resource_size_pair.size(); comp_i>k; comp_i--){
        
        resource_size_pair.pop_back();      //since largest k pairs are wanted, starting from size of the vector to k value, pop elements from back of the vector

        }

    /* END YOUR CODE HERE */

    auto stop = high_resolution_clock::now();                       // Stop measuring time
    auto duration = duration_cast<nanoseconds>(stop - start);       // Calculate the duration
    cout << "Time taken: " << duration.count() << " nanoseconds" << endl;

    /* START YOUR CODE HERE */

    return resource_size_pair;
    /* END YOUR CODE HERE */

}

// Main function
int main(int argc, char* argv[]) {

    // Check the command line arguments
    if (argc != 4) {

        // It should be: <1 or 0> <k> <filename.txt>
        cerr << "Usage: " << argv[0] << " <1 or 0> <k> <filename.txt>" << endl;
        return 1;
    }

    // Get the arguments
    bool useDFS = (atoi(argv[1]) == 1);
    int k = atoi(argv[2]);

    // Read the map from the text file
    string filename = argv[3];
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file." << endl;
        return 1;
    }

    vector<vector<int>> sector_map;
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        vector<int> row;
        int num;
        while (iss >> num) {
            row.push_back(num);
        }
        sector_map.push_back(row);
    }
    file.close();   

    // Find the largest colony
    vector<pair<int, int>> result = top_k_largest_colonies(sector_map, useDFS, k);

    // Print the result
    cout << "Algorithm: " << (useDFS ? "DFS" : "BFS") << endl;
    cout << "Map: " << filename << endl;

    // If no colonies are found
    if (result.empty()) {
        cout << "No colonies found." << endl;
        return 0;
    }

    // Print the top-k colonies
    for (size_t i = 0; i < result.size(); ++i) {
        cout << "Colony " << i + 1 << ": Size = " << result[i].first << ", Resource Type = " << result[i].second << endl;
    }

    return 0;
}