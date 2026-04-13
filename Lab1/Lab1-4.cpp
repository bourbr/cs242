#include <iostream>
#include <vector>
#include <chrono>
#include <numeric>
#include <iomanip>
#include <algorithm>
#include <string>
using namespace std;
using Clock = chrono::high_resolution_clock;
using ms    = chrono::duration<double, milli>;

// O(n²) — selection sort (we only care about time, not the sorted result)
void selectionSort(vector<int> v) {   // pass by value intentionally
    int n = v.size();
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++)
            if (v[j] < v[minIdx]) minIdx = j;
        swap(v[i], v[minIdx]);
    }
}

// O(n) — linear scan
void linearScan(const vector<int>& v) {
     long long sum = 0;    // volatile prevents the compiler optimizing this away
    for (int x : v) sum += x;
}

// O(log n) — binary search, worst case
int binarySearch(const vector<int>& v, int target) {
    int lo = 0, hi = (int)v.size() - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if      (v[mid] == target) return mid;
        else if (v[mid] <  target) lo = mid + 1;
        else                       hi = mid - 1;
    }
    return -1;
}

template<typename Fn>
double timeMs(Fn fn, int reps = 5) {
    auto t0 = Clock::now();
    for (int i = 0; i < reps; i++) fn();
    return ms(Clock::now() - t0).count() / reps;
}

int main() {
    // Selection sort is slow — keep n small for it
    vector<int> sortSizes = {500, 1000, 2000, 4000, 8000};

    cout << "\n--- Complexity Comparison (milliseconds) ---\n\n";
    cout << setw(8)  << "n"
         << setw(18) << "O(n²) sort"
         << setw(18) << "O(n) scan"
         << setw(18) << "O(log n) search"
         << "\n";
    cout << string(62, '-') << "\n";

    for (int n : sortSizes) {
        vector<int> arr(n);
        iota(arr.begin(), arr.end(), 0);

        double t_sort = timeMs([&]{ selectionSort(arr); });
        double t_scan = timeMs([&]{ linearScan(arr); }, 50);
        double t_bin  = timeMs([&]{ binarySearch(arr, n); }, 50000);

        cout << setw(8)  << n
             << setw(18) << fixed << setprecision(3) << t_sort
             << setw(18) << t_scan
             << setw(18) << t_bin
             << "\n";
    }

    cout << "\n--- Predicted vs. Observed Scaling (O(n²) sort) ---\n";
    cout << "(Each row doubles n — O(n²) should multiply time by ~4)\n\n";
    cout << setw(8) << "n" << setw(18) << "Time (ms)" << setw(20) << "Ratio vs prev n" << "\n";
    cout << string(46, '-') << "\n";

    double prev = -1;
    for (int n : sortSizes) {
        vector<int> arr(n);
        iota(arr.begin(), arr.end(), 0);
        double t = timeMs([&]{ selectionSort(arr); });
        string ratio = (prev < 0) ? "—" : to_string(t / prev).substr(0, 4) + "x";
        cout << setw(8) << n << setw(18) << fixed << setprecision(3) << t
             << setw(20) << ratio << "\n";
        prev = t;
    }

    cout << "\n";
    return 0;
}
