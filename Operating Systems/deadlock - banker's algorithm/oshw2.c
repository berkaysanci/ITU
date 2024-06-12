//Berkay Sancı
//504231561

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {            //Struct for processes
    int id;
    int *allocations;
    int *requests;
} Process;

int *resources = NULL;              //Arrays are created dynamically since the size can vary with different input files
Process *processes = NULL;
int num_resources;
int num_processes;

void assign_id() {                                            //ids are assigned. this step is not very essential but I did it.
    for (int i = 0; i < num_processes; i++) {
        processes[i].id = i + 1;
    }
}

int count_resources(const char *filename) {                 //number of resources are counted by reading the resource file. number of processes will be equal.
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open resources file");
        exit(EXIT_FAILURE);
    }

    char line[256];
    if (fgets(line, sizeof(line), file) == NULL) {
        perror("Failed to read resources file");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    fclose(file);

    // Count the number of elements in the line
    int count = 0;
    char *token = strtok(line, " ");
    while (token != NULL) {
        count++;
        token = strtok(NULL, " ");
    }
    return count;
}

void read_resources(const char *filename) {             //resources are read.
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open resources file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < num_resources; i++) {
        fscanf(file, "%d", &resources[i]);
    }

    fclose(file);
}

void read_allocations(const char *filename) {           //allocations are read.
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open allocations file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < num_processes; i++) {
        for (int j = 0; j < num_resources; j++) {
            fscanf(file, "%d", &processes[i].allocations[j]);
        }
    }

    fclose(file);
}

void read_requests(const char *filename) {          //requests are read.
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open requests file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < num_processes; i++) {
        for (int j = 0; j < num_resources; j++) {
            fscanf(file, "%d", &processes[i].requests[j]);
        }
    }

    fclose(file);
}

void print_info() {                                //function to print information of each process.
    for (int i = 0; i < num_processes; i++) {
        printf("Information for process : P%d: \n", processes[i].id);
        printf("Allocated resources : ");
        for (int j = 0; j < num_resources; j++) {
            printf("R%d:%d ", j + 1, processes[i].allocations[j]);
        }
        printf("\nResource request : ");
        for (int j = 0; j < num_resources; j++) {
            printf("R%d:%d ", j + 1, processes[i].requests[j]);
        }
        printf("\n\n");
    }
}

void detect_deadlock() {                            //function to apply Banker's algorithm
    int allocated_resources[num_processes][num_resources];
    int max_resources[num_processes][num_resources];
    int available_resources[num_resources];
    int finished[num_processes];

    for (int i = 0; i < num_processes; i++) {
        for (int j = 0; j < num_resources; j++) {
            allocated_resources[i][j] = processes[i].allocations[j];
            max_resources[i][j] = processes[i].requests[j] + processes[i].allocations[j];
        }
    }

    for (int i = 0; i < num_resources; i++) {               //available resources are calculated
        available_resources[i] = resources[i];
        for (int j = 0; j < num_processes; j++) {
            available_resources[i] -= allocated_resources[j][i];
        }
    }


    for (int k = 0; k < num_processes; k++) {           //finished matrix is initialized with 0.
        finished[k] = 0;                       
    }

    int need[num_processes][num_resources];                 //need matrix is calculated
    for (int i = 0; i < num_processes; i++) {
        for (int j = 0; j < num_resources; j++) {
            need[i][j] = max_resources[i][j] - allocated_resources[i][j];
        }
    }

    int y = 0;
    int running_order[num_processes];           //array to store running order
    int running_ind = 0;        //index for running order array

    for (int k = 0; k < num_processes; k++) {
        for (int i = 0; i < num_processes; i++) {
            if (finished[i] == 0) {
                int flag = 0;
                for (int j = 0; j < num_resources; j++) {
                    if (need[i][j] > available_resources[j]) {
                        flag = 1;
                        break;
                    }
                }

                if (flag == 0) {
                    running_order[running_ind++] = processes[i].id;         //store running order
                    for (y = 0; y < num_resources; y++) {
                        available_resources[y] += allocated_resources[i][y];
                    }
                    finished[i] = 1;
                }
            }
        }
    }

    printf("Running order for processes: ");
    for (int i = 0; i < running_ind; i++) {
        printf("P%d ", running_order[i]);
    }
    printf("\n");

    // Check for deadlock
    int deadlock = 0;
    for (int i = 0; i < num_processes; i++) {
        if (finished[i] == 0) {
            deadlock = 1;
            break;
        }
    }

    if (deadlock) {
        printf("There is a deadlock. ");
        int first = 1;
        for (int i = 0; i < num_processes; i++) {
            if (!finished[i]) {
                if (!first) {
                    printf(" and ");
                }
                printf("P%d", processes[i].id);
                first = 0;
            }
        }
        printf(" are the cause of deadlock.\n");
    } else {
        printf("No deadlock detected. Processes can run to completion.\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <resources.txt> <allocations.txt> <requests.txt>\n", argv[0]);
        return EXIT_FAILURE;
    }

    num_resources = count_resources(argv[1]);       //finding the number of resources by reading resources file
    num_processes = num_resources;


    resources = (int *)malloc(num_resources * sizeof(int));             //dynamically allocating resources array
    if (resources == NULL) {
        perror("Failed to allocate memory for resources");
        return EXIT_FAILURE;
    }


    processes = (Process *)malloc(num_processes * sizeof(Process));         //dynamically allocation processes array
    if (processes == NULL) {
        perror("Failed to allocate memory for processes");
        return EXIT_FAILURE;
    }

  
    for (int i = 0; i < num_processes; i++) {                                               //dyanmically allocating allocations and requests arrays for each process
        processes[i].allocations = (int *)malloc(num_resources * sizeof(int));
        processes[i].requests = (int *)malloc(num_resources * sizeof(int));
        if (processes[i].allocations == NULL || processes[i].requests == NULL) {
            perror("Failed to allocate memory for process allocations or requests");
            return EXIT_FAILURE;
        }
    }

    assign_id();
    read_resources(argv[1]);
    read_allocations(argv[2]);
    read_requests(argv[3]);
    print_info();
    detect_deadlock();

    for (int i = 0; i < num_processes; i++) {           //freeing the allocated memory
        free(processes[i].allocations);
        free(processes[i].requests);
    }
    free(processes);
    free(resources);

    return 0;
}

