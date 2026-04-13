#include <iostream>
#include <vector>
#include <chrono>
#include <numeric>
#include <iomanip>
#include <cmath>
using namespace std;
using Clock = chrono::high_resolution_clock;
using us    = chrono::duration<double, micro>;

int linearSearch(const vector<int>& v, int target) {
    for (int i = 0; i < (int)v.size(); i++)
        if (v[i] == target) return i;
    return -1;
}

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

// Run fn() REPS times and return average microseconds.
template<typename Fn>
double timeIt(Fn fn, int reps = 500) {
    auto t0 = Clock::now();
    for (int i = 0; i < reps; i++) fn();
    auto t1 = Clock::now();
    return us(t1 - t0).count() / reps;
}

int main() {
    vector<int> sizes = {1000, 10000, 100000, 1000000, 10000000};

    cout << "\n--- Runtime Measurement (microseconds, worst-case search) ---\n\n";
    cout << setw(12) << "n"
         << setw(18) << "Linear (us)"
         << setw(18) << "Binary (us)"
         << setw(18) << "Speedup"
         << "\n";
    cout << string(66, '-') << "\n";

    for (int n : sizes) {
        vector<int> arr(n);
        iota(arr.begin(), arr.end(), 0);

        int target = n;   // not in array — worst case for both

        double linTime = timeIt([&]{ linearSearch(arr, target); });
        double binTime = timeIt([&]{ binarySearch(arr, target); });

        cout << setw(12) << n
             << setw(18) << fixed << setprecision(2) << linTime
             << setw(18) << binTime
             << setw(18) << setprecision(1) << (linTime / binTime) << "x"
             << "\n";
    }

    cout << "\n--- Runtime Measurement (microseconds, best-case search) ---\n\n";
    cout << setw(12) << "n"
         << setw(18) << "Linear (us)"
         << setw(18) << "Binary (us)"
         << "\n";
    cout << string(48, '-') << "\n";

    for (int n : sizes) {
        vector<int> arr(n);
        iota(arr.begin(), arr.end(), 0);

        double linTime = timeIt([&]{ linearSearch(arr, 0); });      // first element
        double binTime = timeIt([&]{ binarySearch(arr, arr[n/2]); }); // midpoint

        cout << setw(12) << n
             << setw(18) << fixed << setprecision(2) << linTime
             << setw(18) << binTime
             << "\n";
    }

    cout << "\n";
    return 0;
}
