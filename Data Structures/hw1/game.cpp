#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <iterator>
#include "doublelinklist.h"
#include "objects.h"
#include <cstdlib>
#include <ctime>
using namespace std;

template <typename T>
struct Stack{
    private:
        DoublyList<T> data;
        int elemcount=0;
    public:
        void push(T& e);
        T pop();
        T& top();

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


struct Pair{
    int first;       //first value of the pair, action in this case
    int second;      //second value of the pair, object in this case
};

void shuffle(Pair arr[], int n) {
    srand(time(0));                     //in order to get random values
    for (int i=n-1; i>0; i--) {
        int j = rand() % (i + 1);       
        Pair temp = arr[i];             //changing the indexes of elements
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

int main()
{
    GameState first_state;
    first_state.create_init_state();        //creating initial state
    Stack<GameState> state_stack;           //stacks to push states
    state_stack.push(first_state);          //initial state is pushed onto stack

    while(1){
        GameState current_state = state_stack.top();        //get the element on the top of the stack
        state_stack.pop();                                  //pop the element
        current_state.print_situation();                    //print situation

        if(current_state.win==1){                           //if win flag is true, end the loop
            cout << "Game won!" << endl;
            break;
        }



        Pair pairs[5*(current_state.rooms.get(current_state.room_id)->room_objects.elemcount)];         //create object and action pairs
        int number_of_pairs=0;                                                      
        for(int i=0; i<current_state.rooms.get(current_state.room_id)->room_objects.elemcount ;i++){
            for(int j=0; j<5; j++){
                pairs[number_of_pairs].first=j+1;                                                       //putting actions into pairs
                pairs[number_of_pairs].second=i;                                                        //putting objects into pairs
                number_of_pairs++;                                                                         //increasing the index of the array
            }
        }
 
        shuffle(pairs,number_of_pairs);                                                                 //shuffling the pairs

        for(int i=0;i<number_of_pairs;i++){
            if(current_state.advance(pairs[i].first,pairs[i].second)>0){                                //checking if the object and action pair result in a positive move
                state_stack.push(current_state);                                                    //pushing the state onto the stack
            }
        }
                          
    }
    return 0;

        
}







