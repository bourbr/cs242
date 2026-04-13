
#include <iostream>
#include <vector>
#include <iomanip>
#include <numeric>
using namespace std;

// Returns the number of comparisons made to find target.
// Returns -1 if not found (count still recorded).
int linearSearch(const vector<int>& v, int target, long long& comparisons) {
    comparisons = 0;
    for (int i = 0; i < (int)v.size(); i++) {
        comparisons++;
        if (v[i] == target) return i;
    }
    return -1;
}

int binarySearch(const vector<int>& v, int target, long long& comparisons) {
    comparisons = 0;
    int lo = 0, hi = (int)v.size() - 1;

    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;

        comparisons++; // == comparison
        if (v[mid] == target) return mid;

        comparisons++; // < comparison
        if (v[mid] < target) lo = mid + 1;
        else hi = mid - 1;
    }
    return -1;
}

void printRow(int n, long long linBest, long long linWorst, long long linAvg,
                      long long binBest, long long binWorst, long long binAvg) {
    cout << setw(10) << n
         << setw(12) << linBest  << setw(12) << linWorst << setw(12) << linAvg
         << setw(12) << binBest  << setw(12) << binWorst << setw(12) << binAvg
         << "\n";
}

int main() {
    vector<int> sizes = {10, 100, 1000, 10000, 100000};

    cout << "\n--- Operation Counts: Linear Search vs. Binary Search ---\n\n";
    cout << setw(10) << "n"
         << setw(12) << "Lin Best" << setw(12) << "Lin Worst" << setw(12) << "Lin Avg"
         << setw(12) << "Bin Best" << setw(12) << "Bin Worst" << setw(12) << "Bin Avg"
         << "\n";
    cout << string(82, '-') << "\n";

    for (int n : sizes) {
        // Sorted array: [0, 1, 2, ..., n-1]
        vector<int> arr(n);
        iota(arr.begin(), arr.end(), 0);

        long long comparisons;

        // Best case: target is the first element (linear) or exact midpoint (binary)
        linearSearch(arr, 0, comparisons);
        long long linBest = comparisons;

        binarySearch(arr, arr[(n - 1) / 2], comparisons);
        long long binBest = comparisons;

        // Worst case: target is not in the array
        linearSearch(arr, n, comparisons);   // n is one past the end
        long long linWorst = comparisons;

        binarySearch(arr, n, comparisons);
        long long binWorst = comparisons;

        // Average case: search for each element once, take the mean
        long long totalLin = 0, totalBin = 0;
        int sampleStep = max(1, n / 200);    // sample up to 200 values
        int samples = 0;
        for (int i = 0; i < n; i += sampleStep) {
            linearSearch(arr, arr[i], comparisons);
            totalLin += comparisons;
            binarySearch(arr, arr[i], comparisons);
            totalBin += comparisons;
            samples++;
        }
        long long linAvg = totalLin / samples;
        long long binAvg = totalBin / samples;

        printRow(n, linBest, linWorst, linAvg, binBest, binWorst, binAvg);
    }

    cout << "\n";
    return 0;
}
