/**
 * BLG 336E - Analysis of Algorithms II
 * Assignment 2
 * Berkay Sancı
 * 504231561
*/

#include <iostream>   // For input/output
#include <cmath>      // For distance calculations
#include <vector>     // For vector operations
#include <algorithm>  // For sorting operations
#include <limits>     // For min(), max() functions
#include <fstream>    // To read/write files
#include <chrono>     // For time measurement

//(These libraries are sufficient for this assignment.)

using namespace std;

// Structure to represent a point in 2D space
struct Point {
    double x, y;
};

// Helper function to calculate distance between two points
double distance(Point p1, Point p2) {
    double distance = sqrt(pow(p2.x-p1.x,2)+pow(p2.y-p1.y,2)); //Equation to find the Eucledian distance between two points
    return distance;

}

// Helper function to compare points based on x-coordinate, can use this for sorting.
bool compareX(Point p1, Point p2) {
    return p1.x<p2.x;   //If p2.x is greater than p1.x return true, else false.

}

// Helper function to compare points based on y-coordinate, can use this for sorting.
bool compareY(Point p1, Point p2) {
    return p1.y<p2.y;   //If p2.y is greater than p1.y return true, else false.

}

// Function to find the closest pair of points using brute force.
pair<Point, Point> bruteForceClosestPair(vector<Point>& points, int start, int end) {
    pair<Point, Point> bruteForce_closest;
    int i,j;
    double min_distance = numeric_limits<double>::max();    //setting min_distance as max double value
    for(i=start;i<end-1;i++){                               //calculating distance between each point and the other points in the vector
        for(j=i+1;j<end;j++){
            double new_dist = distance(points[i],points[j]);    
            if(new_dist<min_distance){                         
                min_distance=new_dist;                      //set new min_dist
                bruteForce_closest.first=points[i];         //set this two points
                bruteForce_closest.second=points[j];        //as closest pair

            }

        }
    }
    return bruteForce_closest;

}



// The main objective of the assignment. This is the function to find the closest pair of points recursively using divide and conquer.
pair<Point, Point> closestPair(vector<Point>& points, int start, int end) {
    
    pair<Point, Point> closest_dac;
    if(end-start<=3){                                       //if there are less than or equal to 3 points, problem is solved by brute force
        return bruteForceClosestPair(points,start,end);
    }

    sort(points.begin()+start,points.begin()+end,compareX);   //sort the points by their x value. since we are given start and end indices, these indices are added to beginning index
    int mid_point=(start+end)/2;                              //middle index
    double x_separation=(points[mid_point].x+points[mid_point-1].x)/2.0;    //x coordinate of seperation line. since no points are desired on the line, 
                                                                            //average of mid index and the previous is taken
    pair<Point, Point> closestpair_left=closestPair(points,start,mid_point);       //finding the closest pair on the left side of separation line recursively
    pair<Point, Point> closestpair_right=closestPair(points,mid_point,end);        //finding the closest pair on the right side of separation line recursively
    double dist_left=distance(closestpair_left.first,closestpair_left.second);     //distance between the points in closest pair on the left side
    double dist_right=distance(closestpair_right.first,closestpair_right.second);  //distance between the points in closest pair on the right side
    double dist_min = min(dist_left,dist_right);                                   //take minimum of these two values in order to determine strip length
    vector<Point> strip;                                                           //vector for the points in the strip
    for(int i=start;i<end;i++){
        if(abs(points[i].x - x_separation) < dist_min){                            //if x difference of a point and seperation line is smaller than strip length
            strip.push_back(points[i]);                                            //put that point into the strip vector
        }
    }

    sort(strip.begin(),strip.end(),compareY);                                      //sorting the points in strip by y values
    pair<Point,Point> closest_pair_strip;

    for (size_t i = 0; i < strip.size(); i++) {                                    //for each point in strip, calculate distance of the points within next 15 positions.
        for (size_t j = i + 1; j < min(i + 16, strip.size()); j++) {               //min of i+16 and strip size is taken so that vector size is not exceeded
            double new_dist = distance(strip[i], strip[j]);
            if (new_dist < dist_min) {                                             //if the distance is smaller than strip length
                dist_min = new_dist;                                               //set it as minimum distance
                closest_pair_strip.first = strip[i];                               //set the points as closest pair in the strip
                closest_pair_strip.second = strip[j];
        }
    }
}

    if(dist_min<min(dist_left,dist_right)){                                       //if the minimum distance is smaller than the minimum of left and right distances
        closest_dac = closest_pair_strip;                                         //then the closest pair is the closest pair in strip
    } else if(dist_left<dist_right){                                              //else if minimum distance in the left side is smaller than the right side
        closest_dac = closestpair_left;                                           //then the closest pair is the closest pair in the left side
    } else{
        closest_dac = closestpair_right;                                          //else the closest pair is the closest pair in the right side
    }

    return closest_dac;


}

// Function to remove a pair of points from the point vector. Returns the newly formed vector.
vector<Point> removePairFromVector(vector<Point>& point_vector, pair<Point,Point> point_pair) {

    //Since I got an 'no matched operands "=="' error in my first attempt, I decided to do it with erase and remove if using lambda function.
    //If a point in the vector has the same x and y values with any of the points in point pair, remove the point from point vector.
    point_vector.erase(remove_if(point_vector.begin(), point_vector.end(),[&](Point point)
                                {return (point.x == point_pair.first.x && point.y == point_pair.first.y) ||
                                (point.x == point_pair.second.x && point.y == point_pair.second.y);}),
                                point_vector.end());

    return point_vector;


}

// This function should wrap up the entire algorithm. It should:
//    1- Find the closest pair
//    2- Add the pair to the "pairs" vector
//    3- Remove the paired result from the map
// at each iteration.
// Then it should set "unconnected" variable as the final, unconnected city (if it exists).
void findClosestPairOrder(vector<Point> points) {
    using namespace std::chrono;
    auto start = high_resolution_clock::now();     //start measuring time
    vector<pair<Point,Point>> pairs; //add your pairs to this vector
    Point unconnected = {-1,-1}; //set this as the unconnected point if it exists, leave as is otherwise.
    while(!points.empty()){                     //Keep iterating unless there is no point left in the vector
        if(points.size()==1){                   //If there is just one point left, since it cannot be connected
            Point last_point = points.front();      
            unconnected={last_point.x,last_point.y};    //Set the point as unconnected
            break;
        }
        pair<Point,Point> new_pair= closestPair(points,0,points.size());    //apply divide and conquer function to find the next closest pair
        if(new_pair.first.y > new_pair.second.y || (new_pair.first.y == new_pair.second.y && new_pair.first.x > new_pair.second.x)){
            swap(new_pair.first,new_pair.second);       //make sure that the point with lower y and if ys are equal, the one with lower x is printed first.
        }
        pairs.push_back(new_pair);                      //push back new pair into pairs vector
        removePairFromVector(points,new_pair);          //remove the points from the points vector
    }

    //before printing, please make sure that within each pair, the city with the smaller y coordinate is printed first...
    //...if the y coordinates are the same, print the city with the smaller x coordinate first.

    //This part prints the output, don't modify.    
    for(size_t i = 0; i < pairs.size(); i++){
        cout << "Pair " << i+1 << ": " << pairs[i].first.x << ", " << pairs[i].first.y << " - " << pairs[i].second.x << ", " << pairs[i].second.y << endl;
    }
    if (unconnected.x != -1){
        cout << "Unconnected " << unconnected.x << ", " << unconnected.y;
    }
    auto stop = chrono::high_resolution_clock::now();                           // stop measuring time
    auto duration = duration_cast<nanoseconds>(stop - start);                   // calculate the duration
    cout << "Time taken: " << duration.count() << " nanoseconds" << endl;

}

//Read the coordinates from the file and convert them to a vector. Return the vector as a "vector<Point>"
vector<Point> readCoordinatesFromFile(const string& filename) {   
    vector<Point> points;
    double a,b;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open file." << endl;
        return {};
    }

    while (file >> a >> b) {
        Point point;                //for each line create a point
        point.x=a;                  //assign the first value to x
        point.y=b;                  //second to y
        points.push_back(point);    //push back the point to the vector
    }
    return points;
}

// Main function. Ideally, you don't need to modify this.
int main(int argc, char* argv[]) {
    vector<Point> points = readCoordinatesFromFile(argv[1]);
    findClosestPairOrder(points);
    return 0;
}
