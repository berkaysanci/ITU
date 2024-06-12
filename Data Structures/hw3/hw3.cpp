/*  DATA STRUCTURES HW3
    Berkay Sancı
    504231561   */


#include <iostream>
#include <vector>
#include <fstream>
#include <map>


#include <string>           //to erase "\r" in name2mid file
#include <algorithm>        //for sorting operations
#include <queue>            //for shortest path
#include <set>

using namespace std;

struct Node {
    string MID;
    vector<Node*> adj;
    vector<string> relation;
};


map<string, Node*> graph_map = {};
map<string, string> name_map = {};      //to keep the MIDs and corresponding names

void assign_names(const string& filename){
    ifstream infile(filename);
    string line;
    while (getline(infile, line))
    {
        line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
        string MID = line.substr(0, line.find("\t"));
        string remain = line.substr(line.find("\t")+1,line.length()-MID.length());
        string name = remain.substr(0, remain.find("\t"));
        if(name_map.find(MID)==name_map.end()){             //since the first appearence is wanted to be used
            name_map[MID]=name;                             //assigning MIDs and their names on name map
        }
    }
}



void assign_nodes(const string& filename){
    ifstream infile(filename);
    string line;
    while (getline(infile, line))
    {
        string ent1 = line.substr(0, line.find("\t"));
        string remain = line.substr(line.find("\t")+1,line.length()-ent1.length());
        string relationship = remain.substr(0, remain.find("\t"));
        remain = remain.substr(remain.find("\t")+1, remain.length()-relationship.length());
        string ent2 = remain.substr(0, remain.find("\r"));          // "\t" is changed with "\r" for the correct reading of the file

        Node* ent1_node, *ent2_node;

        if (graph_map.find(ent1) == graph_map.end()) {
            ent1_node = new Node;
            ent1_node->MID = ent1;
            graph_map[ent1] = ent1_node;
        } else {
            ent1_node = graph_map[ent1];
        }

        if (graph_map.find(ent2) == graph_map.end()) {
            ent2_node = new Node;
            ent2_node->MID = ent2;
            graph_map[ent2] = ent2_node;
        } else {
            ent2_node = graph_map[ent2];
        }

        ent2_node->adj.push_back(ent1_node);
        ent2_node->relation.push_back(relationship);  
        ent1_node->adj.push_back(ent2_node);
        ent1_node->relation.push_back(relationship);                   
    }
}

void print_neighbors(string mid){
    Node* node = graph_map[mid];                                        //take the node having the input MID using graph map
    cout << node->adj.size() << " neighbors" << endl;                   //size of adj vector is the count of neighbors

    for(unsigned int i=0; i<node->adj.size(); i++){
        string neighbor_mid = node->adj[i]->MID;                        //take MID of each neighbor
        string neighbor_name = name_map[neighbor_mid];                  //and their names using name map
        cout <<neighbor_mid<< " " << neighbor_name << endl;         

    }
}

void most_central_entities() {
    vector<pair<int, string>> degrees;                                  //to keep the MID and number of neighbor pairs
    for (const auto& entry : graph_map) {
        degrees.push_back({entry.second->adj.size(), entry.first});     //for each entry in graph map, push these values to the vector
    }

    sort(degrees.rbegin(), degrees.rend());                             //sort the vector in descending order neighbor counts
    cout << "Most central 10 entities: "<<endl;
    for (int i = 0; i < 10; ++i) {
        cout<<degrees[i].second<<" "<<name_map[degrees[i].second]<<endl;
    }

}


void find_shortest_path(const string& mid1, const string& mid2) {
    map<string, int> distance;                                              //to keep distances of MIDs from the source MID(mid1)
    map<string, string> parent;                                             //to keep the parent of MIDs for the path
    queue<string> queue;                                                    //queue to apply BFS
    vector<string> path;                                                    //vector of MIDs in the shortest path

    distance[mid1] = 0;                                                     //source MID's distance is set to 0
    queue.push(mid1);                                                           

    while (!queue.empty()) {                                                //BFS is applied
        string current = queue.front();
        queue.pop();
        Node* node = graph_map[current];
        for(unsigned int i=0; i<node->adj.size();++i) {
            if (distance.find(node->adj[i]->MID) == distance.end()) {     //if the MID of adj is not in distance map, it means that it is not visited
                distance[node->adj[i]->MID] = distance[current] + 1;      //it's distance is set to current + 1
                parent[node->adj[i]->MID] = current;                      //and the parent is current MID
                queue.push(node->adj[i]->MID);

            }
        }
    }

    if (distance.find(mid2) == distance.end()) {
        cerr<<"Distance not found."<<endl;
    }

    for (string it = mid2; it != ""; it = parent[it]) {                 //backtracking from target MID(mid2) to source MID(mid1)
        path.push_back(it);
    }
    reverse(path.begin(), path.end());                                  //reversing the vector because we start printing from source to target

    cout<<"Shortest distance between "<<mid1<<" and "<<mid2<<" : "<<path.size()-1<<endl;
    cout<<"Shortest path: "<<endl;
    for(int i=0; i<path.size(); ++i){
        cout<<path[i]<<" "<<name_map[path[i]]<<endl;
    }
}


int main(int argc, char* argv[]){

    assign_nodes("freebase.tsv");
    assign_names("mid2name.tsv");
    string part=argv[1];
    string mid1, mid2, mid3;

    if(part=="part1"){
        mid1 = argv[2];
        print_neighbors(mid1);
    } else if(part=="part2"){
        most_central_entities();
    } else if(part=="part3"){
        mid2=argv[2];
        mid3=argv[3];
        find_shortest_path(mid2,mid3);
    } else{
        cerr<<"Invalid part."<<endl;
    }

}