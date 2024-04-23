#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>

using namespace std;
using namespace std::chrono;

// Sequential Bubble Sort
void bubbleSort(vector<int> &arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// Parallel Bubble Sort
void parallelBubbleSort(vector<int> &arr) {
    int n = arr.size();
    int i, j;

    #pragma omp parallel for private(i, j) shared(arr)
    for (i = 0; i < n - 1; ++i) {
        for (j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void merge(vector<int> &arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; ++i)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; ++j)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            ++i;
        } else {
            arr[k] = R[j];
            ++j;
        }
        ++k;
    }

    while (i < n1) {
        arr[k] = L[i];
        ++i;
        ++k;
    }

    while (j < n2) {
        arr[k] = R[j];
        ++j;
        ++k;
    }
}

// Sequential Merge Sort
void mergeSort(vector<int> &arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// Parallel Merge Sort Helper
void parallelMergeSortHelper(vector<int> &arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        #pragma omp task
        parallelMergeSortHelper(arr, left, mid);
        #pragma omp task
        parallelMergeSortHelper(arr, mid + 1, right);
        #pragma omp taskwait
        merge(arr, left, mid, right);
    }
}

// Parallel Merge Sort
void parallelMergeSort(vector<int> &arr) {
    #pragma omp parallel
    {
        #pragma omp single
        {
            parallelMergeSortHelper(arr, 0, arr.size() - 1);
        }
    }
}

// Utility function to print array elements
void printArray(const vector<int> &arr) {
    for (int i = 0; i < arr.size(); ++i) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

int main() {
    int SIZE;
    cout << "Enter the size of the array: ";
    cin >> SIZE;

    vector<int> arr(SIZE);
    cout << "Enter " << SIZE << " integers:" << endl;
    for (int i = 0; i < SIZE; ++i) {
        cin >> arr[i];
    }

    vector<int> arr_copy = arr; // Make a copy for each sorting method

    // Measure sequential bubble sort time
    auto start = high_resolution_clock::now();
    bubbleSort(arr_copy);
    auto stop = high_resolution_clock::now();
    auto seqBubbleSortTime = duration_cast<microseconds>(stop - start);

    // Print sorted array
    cout << "Sequential Bubble Sort: ";
    printArray(arr_copy);

    arr_copy = arr; // Reset array for parallel bubble sort
    // Measure parallel bubble sort time
    start = high_resolution_clock::now();
    parallelBubbleSort(arr_copy);
    stop = high_resolution_clock::now();
    auto parBubbleSortTime = duration_cast<microseconds>(stop - start);

    // Print sorted array
    cout << "Parallel Bubble Sort: ";
    printArray(arr_copy);

    arr_copy = arr; // Reset array for sequential merge sort
    // Measure sequential merge sort time
    start = high_resolution_clock::now();
    mergeSort(arr_copy, 0, arr_copy.size() - 1);
    stop = high_resolution_clock::now();
    auto seqMergeSortTime = duration_cast<microseconds>(stop - start);

    // Print sorted array
    cout << "Sequential Merge Sort: ";
    printArray(arr_copy);

    arr_copy = arr; // Reset array for parallel merge sort
    // Measure parallel merge sort time
    start = high_resolution_clock::now();
    parallelMergeSort(arr_copy);
    stop = high_resolution_clock::now();
    auto parMergeSortTime = duration_cast<microseconds>(stop - start);

    // Print sorted array
    cout << "Parallel Merge Sort: ";
    printArray(arr_copy);

    // Output the results
    cout << "Sequential Bubble Sort Time: " << seqBubbleSortTime.count() << " microseconds" << endl;
    cout << "Parallel Bubble Sort Time: " << parBubbleSortTime.count() << " microseconds" << endl;
    cout << "Sequential Merge Sort Time: " << seqMergeSortTime.count() << " microseconds" << endl;
    cout << "Parallel Merge Sort Time: " << parMergeSortTime.count() << " microseconds" << endl;

    return 0;
}
