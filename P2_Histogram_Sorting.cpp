#include <iostream>
using namespace std;
#include <vector>
#include <algorithm>
#include <omp.h>

#define NUM_BINS 10  // Number of histogram bins
#define NUM_THREADS 4 // Number of OpenMP threads

// Function to compute the bin index for a given value
int getBinIndex(int value, int minValue, int maxValue) {
    return (NUM_BINS * (value - minValue)) / (maxValue - minValue + 1);
}

// Parallel Histogram Sort
void histogramSort(vector<int>& arr) {
    int n = arr.size();
    if (n == 0) return;

    int minValue = *min_element(arr.begin(), arr.end());
    int maxValue = *max_element(arr.begin(), arr.end());

    // Step 1: Compute histogram and bins in parallel
    vector<int> histogram(NUM_BINS, 0);
    vector<vector<int>> bins(NUM_BINS);

    #pragma omp parallel for num_threads(NUM_THREADS)
    for (int i = 0; i < n; i++) {
        int binIndex = getBinIndex(arr[i], minValue, maxValue);
        #pragma omp atomic
        histogram[binIndex]++; // Increment histogram count
        #pragma omp critical
        bins[binIndex].push_back(arr[i]); // Add to corresponding bin
    }

    // Step 2: Sort each bin in parallel
    #pragma omp parallel for num_threads(NUM_THREADS)
    for (int i = 0; i < NUM_BINS; i++) {
        sort(bins[i].begin(), bins[i].end());
    }

    // Step 3: Merge sorted bins back to the original array
    int index = 0;
    for (int i = 0; i < NUM_BINS; i++) {
        for (int val : bins[i]) {
            arr[index++] = val;
        }
    }
}

int main() {
    vector<int> arr = {23, 45, 12, 89, 5, 34, 78, 11, 90, 67, 55, 32, 43, 21};

    cout << "Original array: ";
    for (int num : arr) cout << num << " ";
    cout << "\n";

    histogramSort(arr);

    cout << "Sorted array: ";
    for (int num : arr) cout << num << " ";
    cout << "\n";

    return 0;
}
