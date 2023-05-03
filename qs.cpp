#include <mpi.h>
#include <stdio.h>
#include <unistd.h> 
#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>
#include <queue>
#include <algorithm>
#include <unordered_map>
#include <ctime>        // std::time
#include <unordered_set>

using namespace std;

const int METADATA = 2;
const int DATA = 1;
const int TOKEN = 3;

const bool USE_QUICKSELECT = true;

void printa(vector<int>& arr) {
    for (int i = 0; i < arr.size(); i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int pow(int x, int p) {
    if (p == 0) return 1;
    if (p == 1) return x;

    int tmp = pow(x, p / 2);
    if (p % 2 == 0) return tmp * tmp;
    else return x * tmp * tmp;
}

void swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

bool compLessThan(int a, int b) {
    return a < b;
}

bool compGreaterThan(int a, int b) {
    return a > b;
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
        quicksort(arr, low, p - 1);
        quicksort(arr, p + 1, high);
    }
}

void insertion_sort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

vector<int> kSmallestQuickselect(vector<int>& v, int N, int K) {
    std::vector<int> v2(v);
    std::nth_element(v2.begin(), v2.begin() + K - 1, v2.end(), compLessThan);
    std::vector<int> res(K);

    for (int i = 0; i < K; i++) {
        res[i] = v2[i];
    }
    return res;
}

vector<int> kLargestQuickselect(vector<int>& v, int N, int K) {
    std::vector<int> v2(v);
    std::nth_element(v2.begin(), v2.begin() + K - 1, v2.end(), compGreaterThan);
    std::vector<int> res(K);

    for (int i = 0; i < K; i++) {
        res[i] = v2[i];
    }
    return res;
}

vector<int> kLargestHeap(vector<int>& v, int N, int K)
{
    vector<int> res;
    if (N == 0) {
        return res;
    }

    priority_queue<int, vector<int>, greater<int> > pq;
    for (int i = 0; i < N; ++i) {
        pq.push(v[i]);
        if (pq.size() > K) {
            pq.pop();
        }
    }
    while (!pq.empty()) {
        res.push_back(pq.top());
        pq.pop();
    }
    return res;
}

vector<int> kSmallestHeap(vector<int>& v, int N, int K)
{
    vector<int> res;
    if (N == 0) {
        return res;
    }
    priority_queue<int> pq;
    for (int i = 0; i < N; ++i) {
        pq.push(v[i]);
        if (pq.size() > K) {
            pq.pop();
        }
    }
    while (!pq.empty()) {
        res.push_back(pq.top());
        pq.pop();
    }
    return res;
}

vector<int> kLargest(vector<int>& v, int N, int K) {
    if (USE_QUICKSELECT) {
        // cout << "Using Quickselect" << endl;
        return kLargestQuickselect(v, N, K);
    }
    return kLargestHeap(v, N, K);
}

vector<int> kSmallest(vector<int>& v, int N, int K) {
    if (USE_QUICKSELECT) {
        // cout << "Using Quickselect" << endl;
        return kSmallestQuickselect(v, N, K);
    }
    return kSmallestHeap(v, N, K);

}

void sort_and_print_old(vector<int>& arr, int low, int high, int id) {
    vector<int> v;
    printf("Hello from %d \n", id);
    for (int i = low; i <= high; i++) {
        v.push_back(arr[i]);
    }
    insertion_sort(v);

    ofstream output_file("output.txt", id == 0 ? std::ios_base::out : std::ios_base::app);
    if (output_file.is_open()) {
        if (id == 0) {
            output_file << "Sorted data" << endl;
        }
        for (int i = 0; i < v.size(); i++) {
            output_file << v[i] << endl;
        }
        output_file.close();
    }
    else {
        cout << "Unable to open file";
    }


    // Tester code - get largest/smallest k elements   
        // int optimal = 2;
        // int k = v.size() - 2;
        // vector<int> k_ele;
        // if (k > 0){
        //     k_ele = kSmallest(v, v.size(), k);
        //     printf("smallest %d elements for %d are \n", k, id);
        //     for (int i=0; i < k_ele.size(); i++) {
        //         printf("%d", k_ele[i]);
        //     }
        //     printf("\n");
        // }
}


void sort_and_print(vector<int>& arr, int id) {
    vector<int> v;
    printf("Hello from %d \n", id);
    insertion_sort(arr);
    for (int i = 0; i < arr.size(); i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}


vector<int> remove(vector<int>& v, vector<int>& extraElements, bool withDuplicates = true) {
    vector<int> result;
    std::unordered_set<int> s(extraElements.begin(), extraElements.end());

    if (withDuplicates) {
        for (int element : v) {
            if (s.count(element) != 1) {
                result.push_back(element);
            }
        }
    }
    else {
        std::unordered_set<int> removedElements;

        for (int element : v) {
            if (s.count(element) == 1 && removedElements.count(element) == 0) { // it needs to be removed and is not removed yet
                removedElements.insert(element);
            }
            else {
                result.push_back(element);
            }
        }
    }
    return result;
}

vector<int> loadbalancing(int totalElements, int totalProcessors, vector<int>& arr, int low, int high, int pid) {
    //printf("Starting load balancing");
    MPI_Request request;
    vector<int> localArray;
    for (int i = low; i <= high; i++) {
        localArray.push_back(arr[i]);
    }
    printf("\nMYID = %d, unbalanced data = %d\n", pid, localArray.size());
    // for(int i=0; i<localArray.size(); i++) {
    //     printf("%d ", localArray[i]);
    // }
    //int totalElements = 0;
    int optimalSize = totalElements / totalProcessors;
    int docontinue = 1;
    int mycontinue = 0;
    int extraElementsSize = 0;
    int numberOfElements = 0;
    int counter = 0;
    //Left to right load balancing
    while (docontinue > 0)
    {
        counter++;
        if (pid == 0)
        {
            // printf("\n----------------------\n");
             //printf("Optimal Size = %d\n",optimalSize);
            extraElementsSize = localArray.size() - optimalSize;
            if (extraElementsSize < 0) {
                extraElementsSize = 0;
            }
            // Send the number of extraElements elements to the next processor
            int dest = pid + 1;
            MPI_Send(&extraElementsSize, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            //printf("\nMYID = %d; Extra Elements = %d; send to %d\n",pid,extraElementsSize, pid+1);
        }
        if (pid > 0) {
            // Receive and send number of extra elements
            MPI_Recv(&numberOfElements, 1, MPI_INT, pid - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            //printf("\nMYID = %d; Recv Extra Elements = %d; from %d\n",pid,numberOfElements, pid-1);
            if (pid < totalProcessors - 1) {
                extraElementsSize = localArray.size() - optimalSize + numberOfElements;
                if (extraElementsSize < 0) {
                    extraElementsSize = 0;
                }
                // Send the number of extraElements elements to the next processor
                // If you don't have enough elements, send 0
                if (extraElementsSize > localArray.size()) {
                    //printf("\nMYID = %d; Extra Elements = %d; send to %d; but don't have them; localArray = %d",pid,extraElementsSize, pid+1, localArray.size());
                    int newextraElementsSize = 0;
                    MPI_Send(&newextraElementsSize, 1, MPI_INT, pid + 1, 0, MPI_COMM_WORLD);
                    //printf("\n----------------------\n");
                }
                else
                {
                    MPI_Send(&extraElementsSize, 1, MPI_INT, pid + 1, 0, MPI_COMM_WORLD);
                    //printf("\nMYID = %d; Extra Elements = %d; send to %d\n",pid,extraElementsSize, pid+1);
                }
            }
        }
        if (extraElementsSize > 0.2 * optimalSize)
        {
            mycontinue = 1;
        }
        else
        {
            mycontinue = 0;
        }
        MPI_Barrier(MPI_COMM_WORLD);
        int sum = 0;
        MPI_Reduce(&mycontinue, &docontinue, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
        MPI_Bcast(&docontinue, 1, MPI_INT, 0, MPI_COMM_WORLD);
        //printf("\nMYID %d reached Barrier 1\n", pid);
        //printf("\nMYID = %d, docontinue = %d\n", pid, docontinue);
        MPI_Barrier(MPI_COMM_WORLD);
        if (counter > 9) {
            break;
        }
        /*
        Send extraElements elements to the next processor
        */
        if (docontinue > 0) {
            // Find elements to send
            if (extraElementsSize > 0) {
                if (extraElementsSize <= localArray.size()) {
                    //>> 1. Find the k greatest elements where k = extraElementsSize
                    int extraElements[extraElementsSize];
                    vector<int> extraElementsVec;
                    extraElementsVec = kLargest(localArray, localArray.size(), extraElementsSize);
                    std::copy(extraElementsVec.begin(), extraElementsVec.end(), extraElements);
                    //>> 2. Remove extraElements elements from localArray
                    //localArray = remove(localArray, extraElements);
                    //printf("\nMYID = %d; Send Actual Elements to %d: ",pid, pid+1);
                    // for (int i = 0; i < extraElementsSize; i++)
                    // {
                    //     //printf("%d ", extraElementsVec[i]);
                    //     localArray.pop_back();//This needs to be replaced with actual code to remove those specific elements
                    // }
                    localArray = remove(localArray, extraElementsVec, true);
                    //printf("\n");
                    //>> 3. Send extraElements to next processor
                    MPI_Isend(&extraElements, extraElementsSize, MPI_INT, pid + 1, 2, MPI_COMM_WORLD, &request);
                    //MPI_Send(&extraElements, extraElementsSize, MPI_INT, pid + 1, 2, MPI_COMM_WORLD);
                }
            }
            if (numberOfElements > 0) {
                int recvElements[numberOfElements];
                // Recieve those elements
                //printf("\nMYID = %d; Waiting to Recv Actual Elements from %d: ",pid, pid - 1);
                //MPI_Irecv(&recvElements, numberOfElements, MPI_INT, pid - 1, 2, MPI_COMM_WORLD,&request);
                MPI_Recv(&recvElements, numberOfElements, MPI_INT, pid - 1, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                // Merge recieved elements to the start of the array
                if (sizeof(recvElements) > 0) {
                    //debugging code
                    for (int k = 0; k < numberOfElements; k++)
                    {
                        for (int l = 0; l < localArray.size(); l++)
                        {
                            if (recvElements[k] > localArray[l])
                            {
                                printf("\nERROR: MYID = %d RecvElements[%d] = %d > localArray[%d] = %d\n", pid, k, recvElements[k], l, localArray[l]);
                            }
                        }
                    }
                    // debuggin code ends

                    //printf("\nMYID = %d; Recv Actual Elements from %d: ",pid, pid - 1);
                    for (int i = 0; i < numberOfElements; i++) {
                        //printf("%d ", recvElements[i]);
                        localArray.insert(localArray.begin(), recvElements[i]);
                        //localArray.push_back(recvElements[i]);//this is wrong -- appends to end of vector
                    }
                    printf("\n");
                }
            }
        }
        //printf("\nMYID %d reached Barrier 2; counter = %d, localarray = %d\n", pid, counter, localArray.size());
        MPI_Barrier(MPI_COMM_WORLD);
    }
    //MPI_Barrier(MPI_COMM_WORLD);
    printf("\nStarting right to left load balancing. MYID = %d; LocalArray = %d\n", pid, localArray.size());
    /*---------------------------------Right to left load balancing---------------------------------

    We now proceed with load balancing in the reverse direction. This is done to ensure that no processor is left with a large number of elements.

    -----------------------------------------------------------------------------------------------*/
    mycontinue = 0;
    extraElementsSize = 0;
    numberOfElements = 0;
    docontinue = 1;
    counter = 0;
    while (docontinue > 0)
    {
        counter++;
        if (pid == totalProcessors - 1)
        {
            //printf("\n----------------------\n");
            //printf("\nOptimal Size = %d\n",optimalSize);
            extraElementsSize = localArray.size() - optimalSize;
            if (extraElementsSize < 0) {
                extraElementsSize = 0;
            }
            // Send the number of extraElements elements to the next processor
            int dest = pid - 1;
            MPI_Send(&extraElementsSize, 1, MPI_INT, pid - 1, 0, MPI_COMM_WORLD);
            //printf("\nMYID = %d; Extra Elements = %d; send to %d\n",pid,extraElementsSize, pid-1);
        }
        if (pid < totalProcessors - 1) {
            // Receive and send number of extra elements
            MPI_Recv(&numberOfElements, 1, MPI_INT, pid + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            //printf("\nMYID = %d; Recv Extra Elements = %d; from %d\n",pid,numberOfElements, pid+1);
            if (pid > 0) {
                extraElementsSize = localArray.size() - optimalSize + numberOfElements;
                if (extraElementsSize < 0) {
                    extraElementsSize = 0;
                }
                // Send the number of extraElements elements to the next processor
                MPI_Send(&extraElementsSize, 1, MPI_INT, pid - 1, 0, MPI_COMM_WORLD);
                //printf("\nMYID = %d; Extra Elements = %d; send to %d\n",pid,extraElementsSize, pid-1);
            }
        }
        if (extraElementsSize > 0.2 * optimalSize)
        {
            mycontinue = 1;
        }
        else
        {
            mycontinue = 0;
        }
        MPI_Barrier(MPI_COMM_WORLD);
        int sum = 0;
        MPI_Reduce(&mycontinue, &docontinue, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
        MPI_Bcast(&docontinue, 1, MPI_INT, 0, MPI_COMM_WORLD);
        //printf("\nMYID %d reached Barrier 3\n", pid);
        //printf("\nMYID = %d, docontinue = %d\n", pid, docontinue);
        MPI_Barrier(MPI_COMM_WORLD);
        //printf("\nMYID = %d, docontinue = %d\n", pid, docontinue);
        //add code here to send and receive elements
        if (counter > 9) {
            break;
        }
        if (docontinue > 0) {
            // Find elements to send
            if (extraElementsSize > 0) {
                if (extraElementsSize <= localArray.size()) {
                    //>> 1. Find the k greatest elements where k = extraElementsSize
                    int extraElements[extraElementsSize];
                    vector<int> extraElementsVec;
                    extraElementsVec = kSmallest(localArray, localArray.size(), extraElementsSize);
                    std::copy(extraElementsVec.begin(), extraElementsVec.end(), extraElements);
                    //>> 2. Remove extraElements elements from localArray
                    //localArray = remove(localArray, extraElements);
                    //>> 3. Send extraElements to next processor
                    //printf("\nMYID = %d; Send Actual Elements to %d: ",pid, pid-1);
                    // for (int i = 0; i < extraElementsSize; i++)
                    // {
                    //     //printf("%d ", extraElementsVec[i]);
                    //     localArray.erase(localArray.begin());
                    // }
                    localArray = remove(localArray, extraElementsVec, true);
                    //printf("\n");
                    MPI_Isend(&extraElements, extraElementsSize, MPI_INT, pid - 1, 2, MPI_COMM_WORLD, &request);
                    //MPI_Send(&extraElements, extraElementsSize, MPI_INT, pid - 1, 2, MPI_COMM_WORLD);
                }
            }
            if (numberOfElements > 0) {
                int recvElements[numberOfElements];
                // Recieve those elements
                //printf("\nMYID = %d; Waiting to Recv Actual Elements from %d: ",pid, pid+1);
                //MPI_Irecv(&recvElements, numberOfElements, MPI_INT, pid + 1, 2, MPI_COMM_WORLD,&request);
                MPI_Recv(&recvElements, numberOfElements, MPI_INT, pid + 1, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                // Merge recieved elements to the start of the array
                if (sizeof(recvElements) > 0) {
                    //debugging code
                    for (int k = 0; k < numberOfElements; k++)
                    {
                        for (int l = 0; l < localArray.size(); l++)
                        {
                            if (recvElements[k] < localArray[l])
                            {
                                printf("\nERROR: MYID = %d RecvElements[%d] = %d < localArray[%d] = %d from ID = %d\n", pid, k, recvElements[k], l, localArray[l], pid + 1);
                            }
                        }
                    }
                    // debuggin code ends
                   //printf("\nMYID = %d; Recv Actual Elements from %d: ",pid, pid-1);
                    for (int i = 0; i < numberOfElements; i++) {
                        //printf("%d ", recvElements[i]);
                        localArray.push_back(recvElements[i]);//This needs to be replaced with actual code to remove those specific elements
                    }
                    //printf("\n");
                    //localArray = mergeStart(recvElements, localArray);
                }
            }
        }
        //printf("\nMYID %d reached Barrier 4; counter = %d, localarray = %d\n", pid, counter, localArray.size());
        MPI_Barrier(MPI_COMM_WORLD);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    printf("\n-------------------------------------------------\n");
    printf("\nMYID = %d, localArray = %d\n", pid, localArray.size());
    // for(int j=0; j<localArray.size(); j++) {
    //         printf("%d ", localArray[j]);
    // }
    printf("\n-------------------------------------------------\n");
    MPI_Barrier(MPI_COMM_WORLD);
    return localArray;
}

vector<int> generateRandomUnique(int numberOfElements) {
    vector<int> data;
    for (int i = 0; i < numberOfElements; i++) {
        data.push_back(i + 1);
    }
    // using built-in random generator:
    std::random_shuffle(data.begin(), data.end());
    return data;
}

vector<int> generatRandomElements(int numberOfElements, bool unique = true) {
    if (unique) {
        return generateRandomUnique(numberOfElements);
    }
    vector<int> data;
    for (int i = 0; i < numberOfElements; i++) {
        data.push_back(rand() % 1000);
    }
    return data;
}

int main(int argc, char** argv) {

    // Set the seed, optional
    std::srand(unsigned(std::time(0)));


    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    int myid;
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    int P;
    MPI_Comm_size(MPI_COMM_WORLD, &P);

    int numberOfElements = 16; // default value
    vector<int> data;

    // read the value of numberOfElements
    numberOfElements = atoi(argv[1]);

    if (myid == 0) {
        data = generatRandomElements(numberOfElements);
    }
    else {
        for (int i = 0; i < numberOfElements; i++) {
            data.push_back(-1);
        }
    }

    int d = (int)log2(P);
    int mask = pow(2, d) - 1;
    int low = 0;
    int high = data.size() - 1;

    for (int i = d - 1; i >= 0; i--) {
        mask = mask ^ pow(2, i);

        if ((myid & mask) == 0) {
            if ((myid & pow(2, i)) == 0) {
                int pivot_index = partition(data, low, high);
                int destination = myid ^ pow(2, i);

                int n_elements = high - pivot_index + 1;

                //send data[pivot_index : high] to msg destination
                MPI_Send(&n_elements, 1, MPI_INT, destination, METADATA, MPI_COMM_WORLD);

                // send the elements themselves
                MPI_Send(&data[pivot_index], n_elements, MPI_INT, destination, DATA, MPI_COMM_WORLD);

                high = pivot_index - 1;

            }
            else {
                int source = myid ^ pow(2, i);
                int n_elements = -1;


                // receive the count of elements from msg source
                MPI_Recv(&n_elements, 1, MPI_INT, source, METADATA, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                int* els = new int[n_elements];

                int index = high + 1;

                // recv elements from a proc and copy them into the section of the array where they belong
                MPI_Recv(els, n_elements, MPI_INT, source, DATA, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                for (int j = 0; j < n_elements; j++) {
                    data[--index] = els[j];
                }
                low = index;

                // free the dynamically allocated array
                delete[] els;
            }
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    vector<int> balanceddata;
    balanceddata = loadbalancing(data.size(), P, data, low, high, myid);
    printf("\nMYID = %d, balanced data = %d\n", myid, balanceddata.size());
    int token = 566;

    // Sequential token passing that prints all the elements in each processor

    if (myid == 0) {
        //sort_and_print(data, low, high, myid);
        sort_and_print(balanceddata, myid);
        // start sending the token from proc 0
        MPI_Send(&token, 1, MPI_INT, myid + 1, TOKEN, MPI_COMM_WORLD);
    }
    else {

        // recv the token
        MPI_Recv(&token, 1, MPI_INT, myid - 1, TOKEN, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // print out this procs elements
        //sort_and_print(data, low, high, myid);
        sort_and_print(balanceddata, myid);
        // send the token to the next proc
        if (myid < P - 1) {
            MPI_Send(&token, 1, MPI_INT, myid + 1, TOKEN, MPI_COMM_WORLD);
        }

        // print out a complete message
        if (myid == (P - 1)) {
            printf("Sorting complete. \n");
        }
    }

    // Finalize the MPI environment.
    MPI_Finalize();

    return 0;
}