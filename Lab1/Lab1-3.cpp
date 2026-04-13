#include <iostream>
#include <vector>
#include <iomanip>
#include <numeric>
using namespace std;

// A simple allocator that tracks total bytes allocated on the heap.
static long long g_allocated = 0;

void* operator new(size_t size) {
    g_allocated += size;
    return malloc(size);
}
void operator delete(void* ptr) noexcept { free(ptr); }
void operator delete(void* ptr, size_t) noexcept { free(ptr); }

long long measure(int n) {
    g_allocated = 0;
    {
        vector<int> arr(n);
        iota(arr.begin(), arr.end(), 0);
        // Searching doesn't allocate — all work is done in-place.
        // This measures the cost of *storing* the data structure.
    }
    return g_allocated;
}

// Recursive binary search — allocates a stack frame per call.
long long recursiveBinSearch(const vector<int>& v, int target, int lo, int hi) {
    if (lo > hi) return 0;
    int mid = lo + (hi - lo) / 2;
    if      (v[mid] == target) return 0;
    else if (v[mid] <  target) return 1 + recursiveBinSearch(v, target, mid+1, hi);
    else                       return 1 + recursiveBinSearch(v, target, lo, mid-1);
}

int main() {
    vector<int> sizes = {1000, 10000, 100000, 1000000};

    cout << "\n--- Memory Usage: Storing the Array ---\n\n";
    cout << setw(10) << "n"
         << setw(20) << "Heap bytes (array)"
         << setw(22) << "Bytes per element"
         << "\n";
    cout << string(52, '-') << "\n";

    for (int n : sizes) {
        long long bytes = measure(n);
        cout << setw(10) << n
             << setw(20) << bytes
             << setw(22) << fixed << setprecision(1) << (double)bytes / n
             << "\n";
    }

    cout << "\n--- Call Stack Depth: Iterative vs. Recursive Binary Search ---\n\n";
    cout << setw(10) << "n"
         << setw(22) << "Iter stack frames"
         << setw(24) << "Recur stack frames (worst)"
         << "\n";
    cout << string(56, '-') << "\n";

    for (int n : sizes) {
        vector<int> arr(n);
        iota(arr.begin(), arr.end(), 0);

        // Iterative: always 1 stack frame (the function itself)
        long long iterFrames = 1;

        // Recursive: depth = number of recursive calls until base case
        long long recurFrames = 1 + recursiveBinSearch(arr, n, 0, n - 1);

        cout << setw(10) << n
             << setw(22) << iterFrames
             << setw(24) << recurFrames
             << "\n";
    }

    cout << "\n";
    return 0;
}
