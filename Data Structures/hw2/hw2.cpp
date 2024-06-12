//////////////////////////////////////////
// The code from Zeynep Gurler's HW is altered for the application. Sincere thanks!
//////////////////////////////////////////
/*
Berkay Sancı
504231561
*/

#include<iostream>
#include<string>
#include<cstdlib>
#include<time.h>
#include "doublelinklist.h"

using namespace std;

struct attack{   // class for pokemon attacks
	private:
		string name;
		int pp;
		int accuracy;
		int damage;
		int first;
	public:
		attack(const string&, int, int, int, int);
		int get_first();
		int get_accuracy();
		int get_damage();
		int get_pp();
		string get_name();
};

attack::attack(const string& name_in, int p, int a, int d, int f){
	name = name_in;
	pp = p;
	accuracy = a;
	damage = d;
	first = f;
}

string attack::get_name(){
	return name;
}

int attack::get_pp(){
	return pp;
}

int attack::get_damage(){
	return damage;
}

int attack::get_first(){
	return first;
}

int attack::get_accuracy(){
	return accuracy;
}

struct pokemon{  // class for pokemons
	int pp;
	int hp;
	string name;
	DoublyList<attack*> attacks;
	pokemon(const string &name_in, int, int); 
	string get_name();
	void set(char, int to_reduce);
};

pokemon::pokemon(const string &name_in, int p, int h){
	name = name_in;
	pp = p;
	hp = h;
}


void pokemon::set(char which, int to_reduce){
	if(which == 'p'){
		pp += to_reduce;
	}else{
		hp -= to_reduce;
	}
}


struct node{  // class for the nodes of the graph
	string ifef;  // attack is efficient or nonefficent
	string curattack;  // attack name
	int num;  // node num
	pokemon* pikachu;  // current pikachu
	pokemon* blastoise;  // currrent blastoise
	char status;  // whose turn?
	int level;  // level of the node
	bool isleaf;  // node is a leaf or not
	double prob;  // prob of the node happening
	 
	DoublyList<node*> child;  // children nodes of the node
	node* parent;
	//Member functions goes there...
	node(pokemon* pikachu_p, pokemon*blastoise_b);
	void print(){
		cout<<"P_HP:"<<pikachu->hp<<" P_PP:"<<pikachu->pp<<" B_HP:"<<blastoise->hp<<" B_PP:"<<blastoise->pp<<" PROB:"<<prob<<endl;
	}

};

node::node(pokemon* pikachu_p, pokemon*blastoise_b){
	pikachu=pikachu_p;
	blastoise=blastoise_b;
}

struct graph{
	//Graph definitions goes there...
	node* root;
	int levelcount=0;
    graph(){
        root = nullptr;
    }
	node* get_root(){return root;};
};

double calculate_prob(node* node, pokemon* pokemon){
	double prob = node -> prob;
	double max_att = 0;
	for(int i=0; i<pokemon->attacks.elemcount; i++){
		if(pokemon->attacks.get(i)->get_first()<= node->level && pokemon->attacks.get(i)->get_pp()<=pokemon->pp ){ //first usage should be on or before this level and pp should be sufficient
			max_att+=1;
		}
	}
	return prob/max_att;
}

void create_children(node* x_node, int max_level){
	if(x_node->level==max_level){
		return;
	}
	if(x_node->status=='P'){	//this is for pokemon attacks
		double prob_effective = calculate_prob(x_node, x_node->pikachu); //probability in case of the attack is effective
		for(int i=0; i<x_node-> pikachu->attacks.elemcount; i++){ //for each attack
			if(x_node -> pikachu->attacks.get(i)->get_first()<= x_node->level && (x_node -> pikachu->attacks.get(i)->get_pp()*(-1))<= x_node->pikachu->pp && x_node->pikachu->hp>0){ //since pp's are in negative, it is multiplied with -1.
				if(x_node->pikachu->attacks.get(i)->get_accuracy()!=100){ //if the accuracy is not 100, then there will be effective and ineffective children
					pokemon* new_pikachu = new pokemon("pikachu",x_node->pikachu->pp+x_node->pikachu->attacks.get(i)->get_pp(),x_node->pikachu->hp); //new pikachu
					pokemon* new_blastoise = new pokemon("blastoise",x_node->blastoise->pp, x_node->blastoise->hp); //new blastoise
					new_pikachu->attacks=x_node->pikachu->attacks;	//making the connections for new pokemons
					new_blastoise->attacks=x_node->blastoise->attacks;
					node* ineffective_child = new node(new_pikachu,new_blastoise); //new ineffective child containing the new pokemons
					ineffective_child->parent=x_node;
					ineffective_child->curattack=x_node->pikachu->attacks.get(i)->get_name();
					ineffective_child->ifef="False"; //since it is not efficient
					ineffective_child->level=x_node->level+1;
					ineffective_child->status='B'; //now it will be blastoise's turn
					ineffective_child->prob=prob_effective-(prob_effective*x_node->pikachu->attacks.get(i)->get_accuracy()/100); //if effectiveness is 80, then 80/100 of effectiveness is subtracted from effective probability
					if(x_node->level+1==max_level){
						ineffective_child->isleaf=true;
					} else{
						ineffective_child->isleaf=false;
					}
					x_node->child.addBack(ineffective_child);
					create_children(ineffective_child, max_level);
				}
				pokemon* new_pikachu = new pokemon("pikachu",x_node->pikachu->pp+x_node->pikachu->attacks.get(i)->get_pp(),x_node->pikachu->hp); //now the effective children
				pokemon* new_blastoise = new pokemon("blastoise",x_node->blastoise->pp,x_node->blastoise->hp-x_node->pikachu->attacks.get(i)->get_damage());
				new_pikachu->attacks=x_node->pikachu->attacks;
				new_blastoise->attacks=x_node->blastoise->attacks;
				node* effective_child = new node(new_pikachu,new_blastoise);
				effective_child->parent=x_node;
				effective_child->curattack=x_node->pikachu->attacks.get(i)->get_name();
				effective_child->level=x_node->level+1;
				effective_child->ifef="True";
				effective_child->status='B';
				if(x_node->pikachu->attacks.get(i)->get_accuracy()!=100){
					effective_child->prob = prob_effective * x_node->pikachu->attacks.get(i)->get_accuracy()/100; //if the effectiveness is 80, then probability is 80/100 of effective probability
				} else{
					effective_child->prob=prob_effective;
				}
				if(x_node->level+1==max_level){
					effective_child->isleaf=true;
				} else{
					effective_child->isleaf=false;
				}
				x_node->child.addBack(effective_child);
				create_children(effective_child,max_level);
			}
		}
	} else{	//doing all the same for blastoise attacks. later I realised that all blastoise attacks are effective, so I don't need ineffective part but I had very little time and just kept it like this.
		double prob_effective = calculate_prob(x_node, x_node->blastoise);
		for(int i=0; i<x_node-> blastoise->attacks.elemcount; i++){	//for each attack
			if(x_node -> blastoise->attacks.get(i)->get_first()<= x_node->level && (x_node -> blastoise->attacks.get(i)->get_pp()*(-1))<= x_node->blastoise->pp ){
				if(x_node->blastoise->attacks.get(i)->get_accuracy()!=100){
					pokemon* new_pikachu = new pokemon("pikachu",x_node->pikachu->pp,x_node->pikachu->hp);
					pokemon* new_blastoise = new pokemon("blastoise",x_node->blastoise->pp+x_node->blastoise->attacks.get(i)->get_pp(),x_node->blastoise->hp);
					new_pikachu->attacks=x_node->pikachu->attacks;
					new_blastoise->attacks=x_node->blastoise->attacks;
					node* ineffective_child = new node(new_pikachu,new_blastoise);
					ineffective_child->parent=x_node;
					ineffective_child->curattack=x_node->blastoise->attacks.get(i)->get_name();
					ineffective_child->ifef="False";
					ineffective_child->level=x_node->level+1;
					ineffective_child->status='P';
					ineffective_child->prob=prob_effective-(prob_effective*x_node->blastoise->attacks.get(i)->get_accuracy()/100);
					if(x_node->level+1==max_level){
						ineffective_child->isleaf=true;
					} else{
						ineffective_child->isleaf=false;
					}
					x_node->child.addBack(ineffective_child);
					create_children(ineffective_child, max_level);
				}
				pokemon* new_pikachu = new pokemon("pikachu",x_node->pikachu->pp,x_node->pikachu->hp-x_node->blastoise->attacks.get(i)->get_damage());
				pokemon* new_blastoise = new pokemon("blastoise",x_node->blastoise->pp+x_node->blastoise->attacks.get(i)->get_pp(),x_node->blastoise->hp);
				new_pikachu->attacks=x_node->pikachu->attacks;
				new_blastoise->attacks=x_node->blastoise->attacks;
				node* effective_child = new node(new_pikachu,new_blastoise);
				effective_child->parent=x_node;
				effective_child->curattack=x_node->blastoise->attacks.get(i)->get_name();
				effective_child->level=x_node->level+1;
				effective_child->ifef="True";
				effective_child->status='P';
				if(x_node->blastoise->attacks.get(i)->get_accuracy()!=100){
					effective_child->prob = prob_effective * x_node->blastoise->attacks.get(i)->get_accuracy()/100;
				} else{
					effective_child->prob=prob_effective;
				}
				if(x_node->level+1==max_level){
					effective_child->isleaf=true;
				} else{
					effective_child->isleaf=false;
				}
				x_node->child.addBack(effective_child);
				create_children(effective_child,max_level);
			}
		}
	}		
}

void create_graph(graph* graph, pokemon* pikachu, pokemon* blastoise, int graph_level){
	node* new_node=new node(pikachu,blastoise);
	new_node->prob=1;	//root probability should be 1
	new_node->status='P'; //first turn should be pikachu
	new_node->level=0;
	graph->root=new_node;
	create_children(graph->root,graph_level);
	graph->levelcount=graph_level;
}

void print_levels(node* node, int maxLevel){
    if(node->level == maxLevel){
		node->print();
	} else if(!(node->child.elemcount==0)){
		for(int i=0; i< node->child.elemcount; i++){
			print_levels(node->child.get(i), maxLevel); //keep printing the nodes at each level until max level is reached
		}
	} else {
		return;
	}
}

template <typename T>
struct Queue {
    private:
        DoublyList<T> data;
    public:
		int elemcount = 0;
        void enqueue(T& e);
        void dequeue();
        T& front();
		bool isEmpty() {
			return elemcount==0;
		}
};

template <typename T>
void Queue<T>::enqueue(T& new_element)
{
    data.addBack(new_element);
    elemcount++;
}

template <typename T>
void Queue<T>::dequeue()
{
    data.removeFront();
    elemcount--;
}

template <typename T>
T& Queue<T>::front()
{return data.head->data;}


template <typename T>
struct Stack {
    private:
        DoublyList<T> data;
    public:
		int elemcount = 0;
        void push(T& e);
        T pop();
        T& top();
		bool isEmpty() {
			return elemcount==0;
		}
};

template <typename T>
void Stack<T>::push(T& new_element)
{
    data.addFront(new_element);
    elemcount++;
}

template <typename T>
T Stack<T>::pop()
{
    T result = data.head->data;
    data.removeFront();
    elemcount--;
    return result;
}

template <typename T>
T& Stack<T>::top()
{return data.head->data;}

int find_easiestlevel(node* b_node, string type){
	Queue<node*> queue;
	node* last_node = b_node;
	int minimum_level;

	if(type=="pikachu"){
		for(int i=0; i<b_node->child.elemcount; i++){	
			node* n_node= b_node->child.get(i);
			queue.enqueue(n_node);	//push the children of root node into stack

		}
		while(!queue.isEmpty()){
			node* curr_node = queue.front();
			queue.dequeue();
			if(curr_node->blastoise->hp<=0){
				minimum_level=curr_node->level;	//when a node found in which game ends, break
				break;
				
			}
			for(int i=0; i<curr_node->child.elemcount;i++){
				node* n_node= curr_node->child.get(i);	//else push the children into stack
				queue.enqueue(n_node);
			}

		}
	} else{	//same for blastoise

		for(int i=0; i<b_node->child.elemcount; i++){
			node* n_node= b_node->child.get(i);
			queue.enqueue(n_node);

		}
		while(!queue.isEmpty()){
			node* curr_node = queue.front();
			queue.dequeue();
			if(curr_node->pikachu->hp<=0){
				minimum_level=curr_node->level;
				break;
			}
			for(int i=0; i<curr_node->child.elemcount;i++){
				node* n_node = curr_node->child.get(i);
				queue.enqueue(n_node);
			}

		}
	}
	return minimum_level; //return the minimum level of end node
}

void find_easiestpath(node* b_node, string type){
	Queue<node*> queue;
	Stack<node*> stack;
	node* last_node = b_node;
	int minimum_level = find_easiestlevel(b_node, type);
	int max_prob=-1;	//in order to find the node with the max probability at a level

	//queue.enqueue(b_node);
	if(type=="pikachu"){
		for(int i=0; i<b_node->child.elemcount; i++){
			node* n_node= b_node->child.get(i);
			queue.enqueue(n_node);

		}
		while(!queue.isEmpty()){
			node* curr_node = queue.front();
			queue.dequeue();
			if(curr_node->blastoise->hp<=0 && curr_node->prob>max_prob && curr_node->level==minimum_level){
				last_node=curr_node;	//check the nodes at a level and if it satisfies the above requirements, assign it as last node
				max_prob=curr_node->prob; //assign its prob as max prob, until the one with max prob is found			
			}
			for(int i=0; i<curr_node->child.elemcount;i++){
				node* n_node= curr_node->child.get(i);
				queue.enqueue(n_node);
			}

		}
	} else{ //same for blastoise

		for(int i=0; i<b_node->child.elemcount; i++){
			node* n_node= b_node->child.get(i);
			queue.enqueue(n_node);

		}
		while(!queue.isEmpty()){
			node* curr_node = queue.front();
			queue.dequeue();
			if(curr_node->pikachu->hp<=0 && curr_node->prob>max_prob && curr_node->level == minimum_level){
				last_node=curr_node;
				max_prob=curr_node->prob;
			}
			for(int i=0; i<curr_node->child.elemcount;i++){
				node* n_node = curr_node->child.get(i);
				queue.enqueue(n_node);
			}

		}
	}
	node* i;

	for(i=last_node; i!=b_node; i=i->parent){
		stack.push(i); //starting from the last node of the easiest path, push their parents until the root node reached
	}

	while(!stack.isEmpty()){					//then print the elements in the stack until its empty
		node* curr_node_st = stack.top();
		stack.pop();
		string poke;
		string iseffective;
		if(curr_node_st->status=='B'){
			poke="Pikachu";
		} else{
			poke="Blastoise";
		}
		if(curr_node_st->ifef=="True"){
			iseffective="effective";
		} else{
			iseffective="ineffective";
		}
		cout << poke <<  " used " << curr_node_st->curattack << ": " << iseffective << endl;
	
	}
	cout<<"Level count: "<<last_node->level<<endl;
	cout<<"Probability: "<<last_node->prob<<endl;
}


int main(int argc, char** argv){

	string part = argv[1];
	int max;
	string which;
	char* pika_or_blastoise;

	if(part == "part1"){
		max = atoi(argv[2]);
	}else if(part == "part2"){
		which = argv[2];
	}
	else
		return -1;
	pokemon* pikachu = new pokemon("pikachu", 100, 200);
	pokemon* blastoise = new pokemon("blastoise", 100, 200);
	
	attack* thundershock = new attack("thundershock", -10, 100, 40, 0);
	attack* skullbash = new attack("skullbash", -15, 70, 50, 0);
	attack* slam = new attack("slam", -20, 80, 60, 0);
	attack* pskip = new attack("skip", 100, 100, 0, 3);
	pikachu->attacks.addBack(thundershock);
	pikachu->attacks.addBack(skullbash);
	pikachu->attacks.addBack(slam);
	pikachu->attacks.addBack(pskip);

	attack* tackle = new attack("tackle", -10, 100, 30, 0);
	attack* watergun = new attack("watergun", -20, 100, 40, 0);
	attack* bite = new attack("bite", -25, 100, 60, 0);
	attack* bskip = new attack("skip", 100, 100, 0, 3);
	blastoise->attacks.addBack(tackle);
	blastoise->attacks.addBack(watergun);
	blastoise->attacks.addBack(bite);
	blastoise->attacks.addBack(bskip);

	graph* new_graph = new graph();
	create_graph(new_graph, pikachu, blastoise, 10);

	if(part=="part1"){
		print_levels(new_graph->root, max);

	} else if(part=="part2"){
		find_easiestpath(new_graph->root,which);
	} else{
		return -1;
	}


}


