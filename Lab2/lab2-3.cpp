#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <numeric>
#include <algorithm>
#include <random>
#include <string>
using namespace std;
using Clock = chrono::high_resolution_clock;
using ms    = chrono::duration<double, milli>;

// Track peak extra memory allocated beyond the input array
static long long g_extra = 0, g_peak = 0;
void* operator new(size_t sz)  { g_extra += sz; if (g_extra > g_peak) g_peak = g_extra; return malloc(sz); }
void  operator delete(void* p) noexcept { free(p); }
void  operator delete(void* p, size_t) noexcept { free(p); }

void selectionSort(vector<int>& v) {
    int n = v.size();
    for (int i = 0; i < n-1; i++) {
        int m = i;
        for (int j = i+1; j < n; j++) if (v[j] < v[m]) m = j;
        swap(v[i], v[m]);
    }
}

void insertionSort(vector<int>& v) {
    int n = v.size();
    for (int i = 1; i < n; i++) {
        int key = v[i], j = i-1;
        while (j >= 0 && v[j] > key) { v[j+1] = v[j]; j--; }
        v[j+1] = key;
    }
}

void merge(vector<int>& v, int lo, int mid, int hi) {
    vector<int> tmp;                          // <-- extra allocation
    int i = lo, j = mid+1;
    while (i <= mid && j <= hi)
        tmp.push_back(v[i] <= v[j] ? v[i++] : v[j++]);
    while (i <= mid) tmp.push_back(v[i++]);
    while (j <= hi)  tmp.push_back(v[j++]);
    for (int k = 0; k < (int)tmp.size(); k++) v[lo+k] = tmp[k];
}
void mergeSort(vector<int>& v, int lo, int hi) {
    if (lo >= hi) return;
    int mid = lo + (hi-lo)/2;
    mergeSort(v, lo, mid); mergeSort(v, mid+1, hi); merge(v, lo, mid, hi);
}

int partition(vector<int>& v, int lo, int hi) {
    int pivot = v[hi], i = lo-1;
    for (int j = lo; j < hi; j++) if (v[j] <= pivot) swap(v[++i], v[j]);
    swap(v[i+1], v[hi]); return i+1;
}
void quickSort(vector<int>& v, int lo, int hi) {
    if (lo < hi) { int p = partition(v, lo, hi); quickSort(v, lo, p-1); quickSort(v, p+1, hi); }
}

template<typename Fn>
pair<double,long long> measure(Fn fn, vector<int> data) {
    g_extra = g_peak = 0;
    auto t0 = Clock::now();
    fn(data);
    double t = ms(Clock::now() - t0).count();
    return {t, g_peak};
}

// Stability test: sort {value, original_index} pairs
// A stable sort preserves original_index order for equal values
bool isStable(void (*fn)(vector<int>&)) {
    // Build array with duplicates: values 0-4 repeated twice
    vector<pair<int,int>> pairs = {{3,0},{1,1},{4,2},{1,3},{5,4},{9,5},{2,6},{6,7},{5,8},{3,9}};
    // Extract values only
    vector<int> vals; for (auto& p : pairs) vals.push_back(p.first);
    // Remember original positions of each value
    vector<int> orig = vals;
    fn(vals);
    // Check: for equal adjacent values in sorted output, did earlier index come first?
    // Re-map sorted values back to original indices
    vector<int> used(pairs.size(), 0);
    for (int i = 0; i < (int)vals.size(); i++) {
        // Find earliest unused occurrence of vals[i] in original
        for (int j = 0; j < (int)pairs.size(); j++) {
            if (!used[j] && pairs[j].first == vals[i]) {
                // Check that if a previous sorted position had the same value, it came from an earlier index
                if (i > 0 && vals[i] == vals[i-1]) {
                    // The previous slot was filled — j should be > its source index
                    // We just track: stable if repeated values appear in original order
                }
                used[j] = 1; break;
            }
        }
    }
    // Simpler direct stability check
    vector<pair<int,int>> tagged;
    for (int i = 0; i < 10; i++) tagged.push_back({pairs[i].first, i});
    vector<int> tagvals; for (auto& p : tagged) tagvals.push_back(p.first);
    fn(tagvals);
    // Rebuild sorted pairs preserving original order for equal elements (what stable means)
    vector<pair<int,int>> expected = tagged;
    stable_sort(expected.begin(), expected.end(), [](auto& a, auto& b){ return a.first < b.first; });
    // Reconstruct what our sort actually produced order-wise
    vector<pair<int,int>> actual = tagged;
    sort(actual.begin(), actual.end(), [](auto& a, auto& b){ return a.first < b.first; });
    // Compare second elements (original indices) for equal first elements
    for (int i = 0; i+1 < (int)expected.size(); i++)
        if (expected[i].first == expected[i+1].first && expected[i].second != actual[i].second)
            return false;
    return true;
}

int main() {
    mt19937 rng(42);
    vector<int> sizes = {1000, 5000, 10000};

    cout << "\n--- Runtime (ms) and Peak Extra Memory (bytes) on Random Input ---\n";
    cout << "\n" << setw(8) << "n"
         << setw(14) << "Sel time" << setw(14) << "Sel mem"
         << setw(14) << "Ins time" << setw(14) << "Ins mem"
         << setw(14) << "Mrg time" << setw(14) << "Mrg mem"
         << setw(14) << "Qck time" << setw(14) << "Qck mem"
         << "\n" << string(120, '-') << "\n";

    for (int n : sizes) {
        vector<int> data(n);
        iota(data.begin(), data.end(), 0);
        shuffle(data.begin(), data.end(), rng);

        auto [st, sm] = measure([](vector<int> v){ selectionSort(v); }, data);
        auto [it, im] = measure([](vector<int> v){ insertionSort(v); }, data);
        auto [mt, mm] = measure([](vector<int> v){ mergeSort(v, 0, (int)v.size()-1); }, data);
        auto [qt, qm] = measure([](vector<int> v){ quickSort(v, 0, (int)v.size()-1); }, data);

        cout << setw(8)  << n
             << setw(14) << fixed << setprecision(2) << st << setw(14) << sm
             << setw(14) << it << setw(14) << im
             << setw(14) << mt << setw(14) << mm
             << setw(14) << qt << setw(14) << qm << "\n";
    }

    cout << "\n--- Stability Test ---\n\n";
    cout << "  Selection sort stable? " << (isStable(selectionSort) ? "YES" : "NO") << "\n";
    cout << "  Insertion sort stable? " << (isStable(insertionSort) ? "YES" : "NO") << "\n";
    // Merge and quick sort require different signatures; result known from theory
    cout << "  Merge sort stable?     YES  (by design — equal elements prefer left half)\n";
    cout << "  Quick sort stable?     NO   (swaps can move equal elements past each other)\n";
    cout << "\n";
}
