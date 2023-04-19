#include <mpi.h>
#include <stdio.h>
#include <unistd.h> 
#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

const int METADATA = 2;
const int DATA = 1;
const int TOKEN = 3;

int getD(int p) {
    switch(p) {
        case 1: return 0;
        case 2: return 1;
        case 4: return 2;
        case 8: return 3;
    }
    printf("Error \n");
    return -1;
}


int pow(int x, int p) {
  if (p == 0) return 1;
  if (p == 1) return x;
  
  int tmp = pow(x, p/2);
  if (p%2 == 0) return tmp * tmp;
  else return x * tmp * tmp;
}

void swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

int partition(vector<int>& arr, int low, int high) {    
    // Use middle element as pivot
    int pivot = arr[low + (high - low) / 2];
    
    // Initialize pointers
    int i = low - 1;
    int j = high + 1;
    
    while (true) {
        // Move i pointer to the right until arr[i] >= pivot
        do {
            i++;
        } while (arr[i] < pivot);
        
        // Move j pointer to the left until arr[j] <= pivot
        do {
            j--;
        } while (arr[j] > pivot);
        
        // If i and j have crossed, the partition is complete
        if (i >= j) {
            return j;
        }
        
        // Swap arr[i] and arr[j] to maintain the partition invariant
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}


int partition1(vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    
    swap(arr[i+1], arr[high]);
    return i+1;
}

void quicksort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int p = partition(arr, low, high);
        quicksort(arr, low, p-1);
        quicksort(arr, p+1, high);
    }
}

void insertion_sort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j+1] = arr[j];
            j--;
        }
        arr[j+1] = key;
    }
}


void main2() {
    int myid;
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    int P;
    MPI_Comm_size(MPI_COMM_WORLD, &P);

    vector<int> data = {2,15,14,3,12,11,1,9,16,7,6,5,4,13,10,8};
    int d = getD(P);
    int mask = pow(2, d) - 1;
    int low = 0;
    int high = data.size() - 1;

    // printf("ok  \n");

    for (int i=d-1; i>=0; i--) {
        mask = mask ^ pow(2, i);
        // printf("in the loop  \n");

        if ((myid & mask) == 0) {
                // printf("here1  \n");

            if ((myid & pow(2, i)) == 0) {
                // printf("here2  \n");
                int pivot_index = partition(data, low, high);
                printf("id: %d, pivot index: %d, Pivot: %d \n", myid, pivot_index, data[pivot_index]);
                int destination = myid ^ pow(2, i);

                int n_elements = high - pivot_index + 1;
                printf("sending id: %d, pivot_index: %d , n_elements: %d\n", myid, pivot_index, n_elements);

                //send data[pivot_index + 1 : high] to msg destination
                MPI_Send(&n_elements, 1, MPI_INT, destination, METADATA, MPI_COMM_WORLD);
                for (int j = pivot_index; j <= high; j++) {
                    MPI_Send(&data[j], 1, MPI_INT, destination, DATA, MPI_COMM_WORLD);
                }

                high = pivot_index - 1;
            } else {
                int source = myid ^ pow(2, i);
                int n_elements = -1;
                int el;

                // receive data from msg source
                MPI_Recv(&n_elements, 1, MPI_INT, source, METADATA, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                printf("recv id: %d, source: %d, n_elements: %d \n", myid, source, n_elements);

                int index = high+1;
                for (int j = 0; j<n_elements; j++) {
                    MPI_Recv(&el, 1, MPI_INT, source, DATA, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    data[--index] = el;
                    printf("el: %d \n", el);
                }
                low = index;
            }
        }
        // mask = mask ^ pow(2, i);
    }
    MPI_Barrier(MPI_COMM_WORLD);

    int token = 566;

    if (myid == 0) {
        printf("Hello from 0 \n");
        MPI_Send(&token, 1, MPI_INT, myid + 1, TOKEN, MPI_COMM_WORLD);

        for (int j=low; j<=high; j++) {
            printf("%d ", data[j]);
        }
        printf("\n");

    } else {
        MPI_Recv(&token, 1, MPI_INT, myid - 1, TOKEN, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("Hello from %d \n", myid);


        for (int j=low; j<=high; j++) {
            printf("%d ", data[j]);
        }
        printf("\n");


        if (myid < P - 1) {
            MPI_Send(&token, 1, MPI_INT, myid + 1, TOKEN, MPI_COMM_WORLD);                     
        }
    }
    
}

int main(int argc, char** argv) {
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Get the name of the processor
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    // Print off a hello world message
    // printf("Hello world from processor %s, rank %d out of %d processors\n",
           // processor_name, world_rank, world_size);

    // cout << "I am C++" << endl;

    /*vector<int> arr = {5, 4, 3, 2, 1};
    quicksort(arr, 0, arr.size()-1);
    for (int i = 0; i < arr.size(); i++) {
        cout << arr[i] << " ";
    }
    cout << endl;*/

    main2();

    // Finalize the MPI environment.
    MPI_Finalize();
}