#include <stdio.h>
#include <unistd.h> 
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>

using namespace std;

int p = 0;
vector<vector<int>> alldata;

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


int main() {
    /*vector<int> data = {
        5, 199, 330, 751, 569, 405, 594, 518, 
        1085, 1275, 1004, 1223, 1449, 1285, 
        1544, 1726, 1853, 1868, 
        2184, 2057, 2081, 
        2514, 2387, 2411, 
        2642, 2800, 
        2959, 
        3174, 3047, 3071};

    vector<int> pivots = {751, 1449, 1868, 2184, 2514, 2800, 2959, 3174};

    vector<int> max_index = {7, 13, 17, 20, 23, 25, 26, 29};
    */

    vector<int> data = readFile()

    quicksort(data, 0, data.size() - 1, pivots);

    cout << "all data" << endl;
    for (vector<int>& v : alldata) {
        for (int el : v) {
            cout << el << " ";
        }
        cout << endl;
    }
}