//Berkay Sancı 504231561

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define N 100000
#define ARRAY_SIZE 200000

int array[ARRAY_SIZE];
int max_of_subarrays[N];



void* find_the_largest(void *arg){
	int thread_no = (int)arg;
	int first_index = thread_no * (ARRAY_SIZE/N);
	int last_index = first_index + ARRAY_SIZE/N;
	int max_value=0;
	for(int index=first_index; index<last_index; index++){
		if(array[index]>max_value){
			max_value=array[index];
		}
	}
	
	max_of_subarrays[thread_no]=max_value;
	pthread_exit(NULL);

}

int main(int argc, char *argv[]){
	int i,t;
	pthread_t threads[N];
	int max_all = 0;


    for(i=0;i<ARRAY_SIZE;i++){
	    array[i]=i;
	}
	
	for(t=0;t<N;t++){
		pthread_create(&threads[t], NULL, find_the_largest, (void*)t);
	}
	
	for(t=0;t<N;t++){
		pthread_join(threads[t],NULL);
	}
	
	for(t=0;t<N;t++){
		if(max_of_subarrays[t]>max_all){
			max_all = max_of_subarrays[t];
		}
	}
	
	printf("The largest element in the array is %d\n",max_all);
    pthread_exit(NULL);
}
