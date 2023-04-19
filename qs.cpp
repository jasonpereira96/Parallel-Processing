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

void printa(vector<int>& arr) {
    for (int i=0; i<arr.size(); i++) {
        printf("%d ", arr[i]);
    }
        printf("\n");

}

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
    int pivot = arr[(low + high) / 2];
    
    // Initialize pointers
    int i = low;
    
    // Move pivot element to the end of the range
    std::swap(arr[(low + high) / 2], arr[high]);
    
    // Partition the range
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            std::swap(arr[i], arr[j]);
            i++;
        }
    }
    
    // Move pivot element to its final position
    std::swap(arr[i], arr[high]);
    
    return i;
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

void sort_and_print(vector<int>& arr, int low, int high, int id) {
    vector<int> v;
    printf("Hello from %d \n", id);
    for (int i=low; i<=high; i++) {
        v.push_back(arr[i]);
    }
    insertion_sort(v);
    for (int i=0; i < v.size(); i++) {
        printf("%d ", v[i]);
    }
    printf("\n");
}


int main(int argc, char** argv) {

    // Initialize the MPI environment
    MPI_Init(NULL, NULL);


    int myid;
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    int P;
    MPI_Comm_size(MPI_COMM_WORLD, &P);

    vector<int> data = {2,15,14,3,12,11,1,9,16,7,6,5,4,13,10,8};
    int d = getD(P);
    int mask = pow(2, d) - 1;
    int low = 0;
    int high = data.size() - 1;

    for (int i=d-1; i>=0; i--) {
        mask = mask ^ pow(2, i);

        if ((myid & mask) == 0) {
            if ((myid & pow(2, i)) == 0) {
                int pivot_index = partition(data, low, high);
                int destination = myid ^ pow(2, i);

                int n_elements = high - pivot_index + 1;

                //send data[pivot_index : high] to msg destination
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

                int index = high + 1;
                for (int j = 0; j<n_elements; j++) {
                    // recv elements from a proc and copy them into the section of the array where they belong
                    MPI_Recv(&el, 1, MPI_INT, source, DATA, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    data[--index] = el;
                }
                low = index;
            }
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);

    int token = 566;

    // Sequential token passing that prints all the elements in each processor

    if (myid == 0) {
        sort_and_print(data, low, high, myid);

        // start sending the token from proc 0
        MPI_Send(&token, 1, MPI_INT, myid + 1, TOKEN, MPI_COMM_WORLD);
    } else {

        // recv the token
        MPI_Recv(&token, 1, MPI_INT, myid - 1, TOKEN, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // print out this procs elements
        sort_and_print(data, low, high, myid);
        
        // send the token to the next proc
        if (myid < P - 1) {
            MPI_Send(&token, 1, MPI_INT, myid + 1, TOKEN, MPI_COMM_WORLD);                     
        }
    }

    // Finalize the MPI environment.
    MPI_Finalize();

    return 0;
}