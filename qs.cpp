#include <mpi.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <math.h>
#include <queue>
#include <algorithm>
#include <unordered_map>
#include <ctime> // std::time
#include <unordered_set>
#include <stdarg.h>

using namespace std;

const int METADATA = 2;
const int DATA = 1;
const int TOKEN = 3;

const bool USE_QUICKSELECT = false;
const bool DEBUG_MODE = false;


int p = 0;
vector<vector<int>> alldata;

vector<int> readFile(string filename) {
    ifstream infile(filename);
    
    if (!infile.is_open()) {
        throw std::runtime_error("Error: failed to open input file");
    }
    
    vector<int> numbers;
    string line;
    while (getline(infile, line)) {
        istringstream iss(line);
        string token;
        while (getline(iss, token, ',')) {
            int num;
            try {
                num = stoi(token);
            } catch (const invalid_argument& ia) {
                cerr << "Error: invalid integer value " << token << " in input file " << filename << endl;
                throw std::runtime_error("Error: failed to open input file");
            } catch (const out_of_range& oor) {
                cerr << "Error: integer value " << token << " out of range in input file " << filename << endl;
                throw std::runtime_error("Error: failed to open input file");
            }
            numbers.push_back(num);
        }
    }
    
    infile.close();
    return numbers;
}


int partition(vector<int>& arr, int low, int high, vector<int> & pivots) {
    // Use middle element as pivot
    // int pivot = arr[(low + high) / 2];

    if (p > pivots.size() - 1) {
        return -1;
    }
    int pivot = pivots[p++];
    int pivotIndex = find(arr.begin(), arr.end(), pivot) - arr.begin();

    // cout << "pvitor: " << pivot << endl;
    // cout << "pvitorIndex: " << pivotIndex << endl;


    // Initialize pointers
    int i = low;

    // Move pivot element to the end of the range
    std::swap(arr[pivotIndex], arr[high]);

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


void quicksort(vector<int>& arr, int low, int high, vector<int> & pivots) {
    if (low < high) {
        int p = partition(arr, low, high, pivots);
        vector<int> v;
        for (int i = low; i <= p; i++) {
            cout << arr[i] << " ";
            v.push_back(arr[i]);
        }
        alldata.push_back(v);
        cout << endl;
        if (p == -1) return;
        // quicksort(arr, low, p - 1);
        quicksort(arr, p + 1, high, pivots);
    }
}

void dprintf(const char* format, ...) {
    if (DEBUG_MODE) {
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    }
}

void printa(vector<int> &arr)
{
    for (int i = 0; i < arr.size(); i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int pow(int x, int p)
{
    if (p == 0)
        return 1;
    if (p == 1)
        return x;

    int tmp = pow(x, p / 2);
    if (p % 2 == 0)
        return tmp * tmp;
    else
        return x * tmp * tmp;
}

void swap(int &a, int &b)
{
    int temp = a;
    a = b;
    b = temp;
}

bool compLessThan(int a, int b)
{
    return a < b;
}

bool compGreaterThan(int a, int b)
{
    return a > b;
}


int _partition(vector<int>& arr, int low, int high) {
    // Use middle element as pivot
    int pivot = arr[(low + high) / 2];

    // Initialize pointers
    int i = low;

    // Move pivot element to the end of the range
    std::swap(arr[(low + high) / 2], arr[high]);

    // Partition the range
    for (int j = low; j < high; j++)
    {
        if (arr[j] <= pivot)
        {
            std::swap(arr[i], arr[j]);
            i++;
        }
    }

    // Move pivot element to its final position
    std::swap(arr[i], arr[high]);

    return i;
}


void _quicksort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int p = _partition(arr, low, high);
        _quicksort(arr, low, p - 1);
        _quicksort(arr, p + 1, high);
    }
}

void insertion_sort(vector<int> &arr)
{
    int n = arr.size();
    for (int i = 1; i < n; i++)
    {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

vector<int> kSmallestQuickselect(vector<int> &v, int N, int K)
{
    std::vector<int> v2(v);
    std::nth_element(v2.begin(), v2.begin() + K - 1, v2.end(), compLessThan);
    std::vector<int> res(K);

    for (int i = 0; i < K; i++)
    {
        res[i] = v2[i];
    }
    return res;
}

vector<int> kLargestQuickselect(vector<int> &v, int N, int K)
{
    std::vector<int> v2(v);
    std::nth_element(v2.begin(), v2.begin() + K - 1, v2.end(), compGreaterThan);
    std::vector<int> res(K);

    for (int i = 0; i < K; i++)
    {
        res[i] = v2[i];
    }
    return res;
}

vector<int> kLargestHeap(vector<int> &v, int N, int K)
{
    vector<int> res;
    if (N == 0)
    {
        return res;
    }

    priority_queue<int, vector<int>, greater<int>> pq;
    for (int i = 0; i < N; ++i)
    {
        pq.push(v[i]);
        if (pq.size() > K)
        {
            pq.pop();
        }
    }
    while (!pq.empty())
    {
        res.push_back(pq.top());
        pq.pop();
    }
    return res;
}

vector<int> kSmallestHeap(vector<int> &v, int N, int K)
{
    vector<int> res;
    if (N == 0)
    {
        return res;
    }
    priority_queue<int> pq;
    for (int i = 0; i < N; ++i)
    {
        pq.push(v[i]);
        if (pq.size() > K)
        {
            pq.pop();
        }
    }
    while (!pq.empty())
    {
        res.push_back(pq.top());
        pq.pop();
    }
    return res;
}

vector<int> kLargest(vector<int> &v, int N, int K)
{
    if (USE_QUICKSELECT)
    {
        // cout << "Using Quickselect" << endl;
        return kLargestQuickselect(v, N, K);
    }
    return kLargestHeap(v, N, K);
}

vector<int> kSmallest(vector<int> &v, int N, int K)
{
    if (USE_QUICKSELECT)
    {
        // cout << "Using Quickselect" << endl;
        return kSmallestQuickselect(v, N, K);
    }
    return kSmallestHeap(v, N, K);
}

void sort_and_print_old(vector<int> &arr, int low, int high, int id)
{
    vector<int> v;
    dprintf("Hello from %d \n", id);
    for (int i = low; i <= high; i++) {
        v.push_back(arr[i]);
    }
    insertion_sort(v);

    ofstream output_file("output.txt", id == 0 ? std::ios_base::out : std::ios_base::app);
    if (output_file.is_open())
    {
        if (id == 0)
        {
            output_file << "Sorted data" << endl;
        }
        for (int i = 0; i < v.size(); i++)
        {
            output_file << v[i] << endl;
        }
        output_file.close();
    }
    else
    {
        cout << "Unable to open file";
    }
}

void sort_and_print(vector<int> &arr, int id, int lbflag)
{
    vector<int> v;
    dprintf("Hello from %d \n", id);
    insertion_sort(arr);
    for (int i = 0; i < arr.size(); i++) {
        dprintf("%d ", arr[i]);
    }
    // my code
    ofstream output_file(lbflag?"Sorted-LB.txt":"Sorted-No-LB.txt", std::ios_base::app);
    if (output_file.is_open())
    {
        output_file << "Processor " << id << ":" << endl;
        for (int i = 0; i < arr.size(); i++)
        {
            if ( i==arr.size()-1){
                output_file << arr[i];
            }else{
                output_file << arr[i] << ", ";
            }         
        }
        output_file << endl;
        output_file.close();
    }
    else
    {
        cout << "Unable to open file";
    }
}

vector<int> remove2(vector<int> &v, vector<int> &extraElements)
{
    unordered_map<int, int> counts;
    for (int el : extraElements)
    {
        if (!counts.count(el))
        {
            counts[el] = 0;
        }
        counts[el]++;
    }

    vector<int> res;
    for (int el : v)
    {
        if (counts[el] && counts[el] > 0)
        {
            counts[el]--;
        }
        else
        {
            res.push_back(el);
        }
    }
    return res;
}

vector<int> remove(vector<int> &v, vector<int> &extraElements, bool withDuplicates = true)
{
    vector<int> result;
    std::unordered_set<int> s(extraElements.begin(), extraElements.end());

    if (withDuplicates)
    {
        for (int element : v)
        {
            if (s.count(element) != 1)
            {
                result.push_back(element);
            }
        }
    }
    else
    {
        std::unordered_set<int> removedElements;

        for (int element : v)
        {
            if (s.count(element) == 1 && removedElements.count(element) == 0)
            { // it needs to be removed and is not removed yet
                removedElements.insert(element);
            }
            else
            {
                result.push_back(element);
            }
        }
    }
    return result;
}
vector<int> loadbalancing(int totalElements, int totalProcessors, vector<int>& localArray, int pid) { //int low, int high,
    MPI_Request request;
    int optimalSize = totalElements / totalProcessors;
    int docontinue = 1;
    int mycontinue = 0;
    int extraElementsSize = 0;
    int numberOfElements = 0;
    int counter = 0;
    // Left to right load balancing
    while (docontinue > 0)
    {
        counter++;
        if (pid == 0)
        {
            extraElementsSize = localArray.size() - optimalSize;
            if (extraElementsSize < 0)
            {
                extraElementsSize = 0;
            }
            // Send the number of extraElements elements to the next processor
            int dest = pid + 1;
            MPI_Send(&extraElementsSize, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        }
        if (pid > 0)
        {
            // Receive and send number of extra elements
            MPI_Recv(&numberOfElements, 1, MPI_INT, pid - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (pid < totalProcessors - 1) {
                extraElementsSize = localArray.size() - optimalSize + numberOfElements;
                if (extraElementsSize < 0)
                {
                    extraElementsSize = 0;
                }
                // Send the number of extraElements elements to the next processor
                // If you don't have enough elements, send 0
                if (extraElementsSize > localArray.size()) {
                    int newextraElementsSize = 0;
                    MPI_Send(&newextraElementsSize, 1, MPI_INT, pid + 1, 0, MPI_COMM_WORLD);
                }
                else
                {
                    MPI_Send(&extraElementsSize, 1, MPI_INT, pid + 1, 0, MPI_COMM_WORLD);
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
        if (counter > 9) {
            break;
        }
        /*
        Send extraElements elements to the next processor
        */
        if (docontinue > 0)
        {
            // Find elements to send
            if (extraElementsSize > 0)
            {
                if (extraElementsSize <= localArray.size())
                {
                    //>> 1. Find the k greatest elements where k = extraElementsSize
                    int extraElements[extraElementsSize];
                    vector<int> extraElementsVec;
                    extraElementsVec = kLargest(localArray, localArray.size(), extraElementsSize);
                    //>> 2. Remove extraElements elements from localArray
                    localArray = remove2(localArray, extraElementsVec);
                    //>> 3. Send extraElements to next processor
                    MPI_Send(extraElementsVec.data(), extraElementsSize, MPI_INT, pid + 1, 2, MPI_COMM_WORLD);
                }
            }
            if (numberOfElements > 0) {
                vector<int> recvElements(numberOfElements);
                MPI_Recv(recvElements.data(), numberOfElements, MPI_INT, pid - 1, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                localArray.insert(localArray.end(), recvElements.begin(), recvElements.end());
            }
        }
        //MPI_Barrier(MPI_COMM_WORLD);
    }
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
            extraElementsSize = localArray.size() - optimalSize;
            if (extraElementsSize < 0)
            {
                extraElementsSize = 0;
            }
            // Send the number of extraElements elements to the next processor
            MPI_Send(&extraElementsSize, 1, MPI_INT, pid - 1, 0, MPI_COMM_WORLD);
        }
        if (pid < totalProcessors - 1)
        {
            // Receive and send number of extra elements
            MPI_Recv(&numberOfElements, 1, MPI_INT, pid + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (pid > 0) {
                // Send the number of extraElements elements to the next processor
                extraElementsSize = localArray.size() - optimalSize + numberOfElements;
                if (extraElementsSize < 0)
                {
                    extraElementsSize = 0;
                }
                if (extraElementsSize > localArray.size()) {
                    int newextraElementsSize = 0;
                    MPI_Send(&newextraElementsSize, 1, MPI_INT, pid - 1, 0, MPI_COMM_WORLD);
                }
                else
                {
                    MPI_Send(&extraElementsSize, 1, MPI_INT, pid - 1, 0, MPI_COMM_WORLD);
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
        if (counter > 9) {
            break;
        }
        if (docontinue > 0)
        {
            // Find elements to send
            if (extraElementsSize > 0 && extraElementsSize <= localArray.size())
            {
                //>> 1. Find the k greatest elements where k = extraElementsSize
                int extraElements[extraElementsSize];
                vector<int> extraElementsVec;
                extraElementsVec = kSmallest(localArray, localArray.size(), extraElementsSize);
                //>> 2. Remove extraElements elements from localArray
                localArray = remove2(localArray, extraElementsVec);
                //>> 3. Send extraElements to next processor
                MPI_Send(extraElementsVec.data(), extraElementsSize, MPI_INT, pid - 1, 2, MPI_COMM_WORLD);
            }
            if (numberOfElements > 0) {
                vector<int> recvElements(numberOfElements);
                // Recieve those elements
                MPI_Recv(recvElements.data(), numberOfElements, MPI_INT, pid + 1, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                // Merge recieved elements to the vector
                localArray.insert(localArray.end(), recvElements.begin(), recvElements.end());
            }
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);
    return localArray;
}

vector<int> generateRandomUnique(int numberOfElements)
{
    vector<int> data;
    for (int i = 0; i < numberOfElements; i++)
    {
        data.push_back(i + 1);
    }
    // using built-in random generator:
    std::random_shuffle(data.begin(), data.end());
    return data;
}

vector<int> generatRandomElements(int numberOfElements, bool unique = true)
{
    if (unique)
    {
        return generateRandomUnique(numberOfElements);
    }
    vector<int> data;
    for (int i = 0; i < numberOfElements; i++)
    {
        data.push_back(rand() % 1000);
    }
    return data;
}

vector<int> slice(vector<int> &v, int low, int high)
{
    if (low > high)
    {
        cout << "Invalid indexes to slice" << endl;
        throw std::runtime_error("Invalid indexes to slice");
    }
    return std::vector<int>(v.begin() + low, v.end() - (v.size() - high - 1));
}

int main(int argc, char **argv)
{

    // Set the seed, optional
    std::srand(6);

    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    int myid;
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    int P;
    MPI_Comm_size(MPI_COMM_WORLD, &P);

    int numberOfElements = 16; // default value
    vector<int> data;
    double start, end;
    // read the value of numberOfElements
    // numberOfElements = atoi(argv[1]);
    int lbflag = atoi(argv[3]);//0 = no lb; 1 = lb
    

    int N;
    if (myid == 0) {
        data = readFile(argv[1]);
        vector<int> pivots = readFile(argv[2]);
        // vector<int> max_index = readFile(argv[3]);
        N= data.size();

        quicksort(data, 0, data.size() - 1, pivots);

        for (int i=1; i < alldata.size(); i++) {
            int size = alldata[i].size();
            MPI_Send(&size, 1, MPI_INT, i, METADATA, MPI_COMM_WORLD);
            MPI_Send(&alldata[i][0], size, MPI_INT, i, DATA, MPI_COMM_WORLD);
        }
        // proc 0 needs only the first set
        data = alldata[0];        

        // data = generatRandomElements(numberOfElements, false);
        // data = {2,15,14,3,12,11,1,9,16,7,6,5,4,13,10,8};
        // data = {8,2,3,4,5,6,7,1,9,10,11,12,13,14,15,16};
    } else {
        MPI_Recv(&numberOfElements, 1, MPI_INT, 0, METADATA, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        int* els = new int[numberOfElements];
        MPI_Recv(els, numberOfElements, MPI_INT, 0, DATA, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        for (int i=0; i<numberOfElements; i++) {
            data.push_back(els[i]);
        }
        delete[] els;
    }

    
    MPI_Barrier(MPI_COMM_WORLD);
    vector<int> balanceddata;
    if (myid == 0) {
        start = MPI_Wtime();
    }
    // load balancing
    // balanceddata = loadbalancing(data.size(), P, data, low, high, myid);

    // without load balancing
    balanceddata = data;

    //with load balancing
    if (lbflag == 1)
    {
        balanceddata = loadbalancing(data.size(), P, balanceddata, myid);
    }
    
    cout << "After LB" << endl;
    for (int i=0; i<balanceddata.size(); i++) {
        cout << balanceddata[i] << " ";
    }
    cout << endl;

    printf("\nMYID = %d, balanced data = %ld\n", myid, balanceddata.size());
    int token = 566;

    // Sequential token passing that prints all the elements in each processor

    if (myid == 0)
    {
        ofstream output_file(lbflag?"Sorted-LB.txt":"Sorted-No-LB.txt", std::ios_base::out);

        if (output_file.is_open())
        {
            output_file << "N= " << N << ", P=" << P << ",s="
                        << " ,"
                        << "load imbalance metric=" <<lbflag <<endl;
            output_file.close();
        }
        else
        {
            cout << "Unable to open file";
        }

        if (balanceddata.size() > 0)
        {
            sort_and_print(balanceddata, myid,lbflag);
        }
        else {
            dprintf("Hello from %d, nothing to print \n", myid);
        }
        // start sending the token from proc 0
        MPI_Send(&token, 1, MPI_INT, myid + 1, TOKEN, MPI_COMM_WORLD);
    }
    else
    {

        // recv the token
        MPI_Recv(&token, 1, MPI_INT, myid - 1, TOKEN, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // print out this procs elements
        
        if (balanceddata.size() > 0) {
            sort_and_print(balanceddata, myid,lbflag);
        }
        else {
            dprintf("Hello from %d, nothing to print \n", myid);
        }

        // send the token to the next proc
        if (myid < P - 1)
        {
            MPI_Send(&token, 1, MPI_INT, myid + 1, TOKEN, MPI_COMM_WORLD);
        }

        // print out a complete message
        if (myid == (P - 1))
        {
            printf("Sorting complete. \n");
        }
    }
    if (myid == 0)
    {
        end = MPI_Wtime();
        printf("\nTime taken = %f\n", end - start);
    }
    // Finalize the MPI environment.
    MPI_Finalize();

    return 0;
}
