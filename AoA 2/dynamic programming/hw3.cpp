/**
 * BLG 336E - Analysis of Algorithms II
 * Assignment 3
 * Berkay Sancı
 * 504231561
*/

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

/* YOU CAN INCLUDE ANY OTHER NECESSARY LIBRARIES */
#include <cmath>
#include <iomanip>
/* YOU CAN DEFINE EXTRA FUNCTIONS AND MODIFY THE CODE TEMPLATE */

/* YOU HAVE TO CONSIDER THE OUTPUT FORMAT TO TEST YOUR CODE WITH CALICO */


using namespace std;


struct TimeIntervals {
    string floor_name;
    string room_number;
    string start_time;
    string end_time;
    int priority_value;
};


struct Priority {
    string floor_name;
    string room_number;
    int priority_value;   
};

struct Item {
    string item_name;
    int price;
    float need_value;    
};


/* START: WEIGHTED INTERVAL SCHEDULING (for each floor) */

struct Schedule
{
    string floor_name;
    vector<TimeIntervals> intervals;
};

int priority_gain(Schedule schedule){
    vector<TimeIntervals> time_int = schedule.intervals;    //getting intervals from schedule
    sort(time_int.begin(),time_int.end(),[](const TimeIntervals& a, const TimeIntervals& b){return a.end_time<b.end_time;}); //sort intervals by end time
    int time_int_size = time_int.size();    //number of time intervals
    vector<int> p(time_int_size,0); //vector for p values
    for( int j = 0; j < time_int_size; j++){    //in order to find p values using binary search
        int low=0;
        int high=j-1;
        while(low<=high){
            int mid= (low+high)/2;
            if(time_int[mid].end_time<=time_int[j].start_time){
                if(time_int[mid+1].end_time <= time_int[j].start_time){
                    low=mid+1;
                } else{
                    p[j]=mid+1;
                    break;
                }
                    
            } else {
                high = mid - 1;
            }
        }
    }
    vector<int> M(time_int_size+1, 0); //vector to store calculations
    M[0]=0;
    for (int j = 1; j <= time_int_size; ++j) {
        M[j] = max(time_int[j-1].priority_value + M[p[j-1]], M[j - 1]); //recursive calculation of M[j] (since time_int and p vectors start from index 0, j-1 used instead of j in the formula)
    }

    return  M[time_int_size];

}

vector<Schedule> weighted_interval_scheduling(vector<Schedule> schedules){
    vector<Schedule> optimal_schedules;
    for(unsigned int i=0; i<schedules.size();i++){
        Schedule optimal_schedule;  
        vector<TimeIntervals> time_int = schedules[i].intervals;    //getting intervals from schedule
        sort(time_int.begin(),time_int.end(),[](const TimeIntervals& a, const TimeIntervals& b){return a.end_time<b.end_time;}); //sort intervals by end time
        int time_int_size = time_int.size();    //number of time intervals
        vector<int> p(time_int_size,0);
        for( int j = 0; j < time_int_size; j++){    //in order to find p values using binary search
            int low=0;
            int high=j-1;
            while(low<=high){
                int mid= (low+high)/2;
                if(time_int[mid].end_time<=time_int[j].start_time){
                    if(time_int[mid+1].end_time <= time_int[j].start_time){
                        low=mid+1;
                    } else{
                        p[j]=mid+1;
                        break;
                    }
                    
                } else {
                    high = mid - 1;
                }
            }
        }
        vector<TimeIntervals> selected_intervals;
        vector<int> M(time_int_size + 1, 0);    //vector to store calculations
        M[0]=0;
        for (int j = 1; j <= time_int_size; ++j) {
            M[j] = max(time_int[j - 1].priority_value + M[p[j - 1]], M[j - 1]); //recursive calculation of M[j]
        }

        int j = time_int_size;
        while(j>0){ //backtracking to find the optimal intervals
            if(time_int[j - 1].priority_value + M[p[j - 1]]>M[j - 1]){
                selected_intervals.push_back(time_int[j-1]);
                j = p[j-1];
            } else {
                j--;
            }
        }

        reverse(selected_intervals.begin(),selected_intervals.end());
        optimal_schedule.floor_name=schedules[i].floor_name;
        optimal_schedule.intervals=selected_intervals;
        optimal_schedules.push_back(optimal_schedule);        
    }
    return optimal_schedules;

}
// *** END ***


/* START: KNAPSACK (for items) */

// Function to select the most valuable items that can be purchased with a certain budget

float knapsack_values(const vector<Item>& Items, int budget) {
    vector<Item> selected_items;
    int n = Items.size();
    vector<vector<float>> K(budget+1,vector<float>(n+1,0));     //2d vector budgetxnumber of items
    for (int j = 0; j <= n; j++) {
        K[0][j] = 0;                                //initialize first row with 0
    }   
    
    for (int i = 0; i <= budget; i++) {
        K[i][0] = 0;                            //initialize first column with 0
    }
    for(int i=1; i<=budget; i++){   //bottom up dynamic programming algorithm
        for(int j=1; j<=n; j++){
            K[i][j]=K[i][j-1];
            if(Items[j-1].price<=i){
                K[i][j]=max(K[i][j],K[i-Items[j-1].price][j-1]+Items[j-1].need_value);
            }
        }
    }
    float result=round(K[budget][n]*10)/10.0; //rounding to avoid wrong results (36.65 instead of 36.7)
    return result;

}

vector<Item> knapsack(const vector<Item>& Items, int budget) {
    vector<Item> selected_items;
    int n = Items.size();
    vector<vector<float>> K(budget+1,vector<float>(n+1,0)); //2d vector budgetxnumber of items
    for (int j = 0; j <= n; j++) {
        K[0][j] = 0;                    //initialize first row with 0
    }
    
    for (int i = 0; i <= budget; i++) {
        K[i][0] = 0;                   //initialize first column with 0
    }
    for(int i=1; i<=budget; i++){       //bottom up dynamic programming algorithm
        for(int j=1; j<=n; j++){
            K[i][j]=K[i][j-1];
            if(Items[j-1].price<=i){
                K[i][j]=max(K[i][j],K[i-Items[j-1].price][j-1]+Items[j-1].need_value);
            }
        }
    }
    int budget_left = budget;
    for (int j = n; j > 0 && K[budget_left][j] > 0; j--) {  //backtracking to find the items
        if (K[budget_left][j] != K[budget_left][j - 1]) {
            selected_items.push_back(Items[j - 1]);
            budget_left -= Items[j - 1].price;
        }
    }

    return selected_items;
}


// *** END ***

vector<TimeIntervals> read_time_intervals(const string& filename){
    vector<TimeIntervals> intervals;
    ifstream infile(filename);
    if (!infile.is_open()) {
        cout << "Failed to open the file." << endl;
        return intervals;
    }

    string line;
    getline(infile, line);

    while (getline(infile, line)) {
        stringstream ss(line);
        TimeIntervals interval;
        ss >> interval.floor_name >> interval.room_number >> interval.start_time >> interval.end_time;
  
        intervals.push_back(interval);
    }

    infile.close();
    return intervals;
}

vector<Priority> read_priorities(const string& filename){
    vector<Priority> priorities;
    ifstream infile(filename);
    if (!infile.is_open()) {
        cout << "Failed to open the file." << endl;
        return priorities;
    }

    string line;
    getline(infile, line);

    while (getline(infile, line)) {
        stringstream ss(line);
        Priority priority;
        ss >> priority.floor_name >> priority.room_number >> priority.priority_value;
        priorities.push_back(priority);
    }

    infile.close();
    return priorities;    
}

vector<Item> read_items(const string& filename){
    vector<Item> items;
    ifstream infile(filename);
    if (!infile.is_open()) {
        cout << "Failed to open the file." << endl;
        return items;
    }
    string line;
    getline(infile, line);

    while (getline(infile, line)) {
        stringstream ss(line);
        Item item;
        ss >> item.item_name >> item.price >> item.need_value;
        items.push_back(item);
    }

    infile.close();
    return items;      
}

int main(int argc, char *argv[])
{
    string case_no = argv[1];
    string case_name = "case_" + case_no;
    string path = "./inputs/" + case_name;

    int total_budget = 200000; 

    vector<TimeIntervals> intervals = read_time_intervals(path+"/room_time_intervals.txt");
    vector<Priority> priorities = read_priorities(path+"/priority.txt");
    vector<Item> items = read_items(path+"/items.txt");


    for(unsigned int i=0; i<intervals.size(); i++){
        for(unsigned int j=0; j<priorities.size(); j++){
            if(intervals[i].floor_name==priorities[j].floor_name && intervals[i].room_number==priorities[j].room_number){
                intervals[i].priority_value=priorities[j].priority_value;
            }
        }
    }

    if(case_no=="1" || case_no=="3"){
        vector<Schedule> schedules,opt_schedules;
        Schedule schedule_1,schedule_2,schedule_3;
        vector<TimeIntervals> time_int1, time_int2, time_int3;
        vector<Item> selected_items = knapsack(items,total_budget);
        schedule_1.floor_name="Floor_1";
        schedule_2.floor_name="Floor_2";
        schedule_3.floor_name="Floor_3";
        for(unsigned int i=0; i<intervals.size();i++){
            if(intervals[i].floor_name==schedule_1.floor_name){
                time_int1.push_back(intervals[i]);
            } else if(intervals[i].floor_name==schedule_2.floor_name){
                time_int2.push_back(intervals[i]);
            } else{
                time_int3.push_back(intervals[i]);
            }
        }
        schedule_1.intervals=time_int1;
        schedule_2.intervals=time_int2;
        schedule_3.intervals=time_int3;
        schedules.push_back(schedule_1);
        schedules.push_back(schedule_2);
        schedules.push_back(schedule_3);
        opt_schedules=weighted_interval_scheduling(schedules);

        cout<<"Best Schedule for Each Floor"<<"\n"<<endl;
        for(unsigned int i=0; i<opt_schedules.size();i++){            
            cout<<opt_schedules[i].floor_name<<" --> Priority Gain: "<<priority_gain(opt_schedules[i])<<endl;
            for(unsigned int j=0; j<opt_schedules[i].intervals.size();j++){
                cout<<opt_schedules[i].floor_name<<"\t"<<opt_schedules[i].intervals[j].room_number<<"\t"<<opt_schedules[i].intervals[j].start_time<<"\t"<<opt_schedules[i].intervals[j].end_time<<endl;
            }
            cout<<"\n";
        }

        cout<<"Best Use of Budget"<<endl;
        cout<<"Total Value --> "<< fixed << setprecision(1)<<knapsack_values(items,total_budget)<<"\n"<<endl;
        for(unsigned int i=0; i<selected_items.size();i++){
            cout<<selected_items[i].item_name<<endl;
        }
    } else if(case_no=="2"){
        vector<Schedule> schedules, opt_schedules;
        Schedule schedule_1,schedule_2,schedule_3, schedule_4;
        vector<TimeIntervals> time_int1, time_int2, time_int3, time_int4;
        vector<Item> selected_items = knapsack(items,total_budget);
        schedule_1.floor_name="Floor_1";
        schedule_2.floor_name="Floor_2";
        schedule_3.floor_name="Floor_3";
        schedule_4.floor_name="Floor_4";
        for(unsigned int i=0; i<intervals.size();i++){
            if(intervals[i].floor_name==schedule_1.floor_name){
                time_int1.push_back(intervals[i]);
            } else if(intervals[i].floor_name==schedule_2.floor_name){
                time_int2.push_back(intervals[i]);
            } else if(intervals[i].floor_name==schedule_3.floor_name){
                time_int3.push_back(intervals[i]);
            } else{
                time_int4.push_back(intervals[i]);
            }
        }
        schedule_1.intervals=time_int1;
        schedule_2.intervals=time_int2;
        schedule_3.intervals=time_int3;
        schedule_4.intervals=time_int4;
        schedules.push_back(schedule_1);
        schedules.push_back(schedule_2);
        schedules.push_back(schedule_3);
        schedules.push_back(schedule_4);
        opt_schedules=weighted_interval_scheduling(schedules);

        cout<<"Best Schedule for Each Floor"<<"\n"<<endl;
        for(unsigned int i=0; i<opt_schedules.size();i++){
            cout<<opt_schedules[i].floor_name<<" --> Priority Gain: "<<priority_gain(opt_schedules[i])<<endl;
            for(unsigned int j=0; j<opt_schedules[i].intervals.size();j++){
                cout<<opt_schedules[i].intervals[j].floor_name<<"\t"<<opt_schedules[i].intervals[j].room_number<<"\t"<<opt_schedules[i].intervals[j].start_time<<"\t"<<opt_schedules[i].intervals[j].end_time<<endl;
            }
            cout<<"\n";
        }
        cout<<"Best Use of Budget"<<endl;
        cout<<"Total Value --> "<<fixed << setprecision(1)<<knapsack_values(items,total_budget)<<"\n"<<endl;
        for(unsigned int i=0; i<selected_items.size();i++){
            cout<<selected_items[i].item_name<<endl;
        }
    } else{
        cerr<<"Invalid case."<<endl;
    }


    return 0;
}