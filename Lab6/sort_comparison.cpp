#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <chrono>
#include <iomanip>
#include <random>
using namespace std;
using Clock = chrono::high_resolution_clock;

// -------- Heapsort (in-place, O(1) aux) --------
void siftDown(vector<int>& a, int i, int n) {
    while (true) {
        int best = i, l = 2*i+1, r = 2*i+2;
        if (l < n && a[l] > a[best]) best = l;
        if (r < n && a[r] > a[best]) best = r;
        if (best == i) break;
        swap(a[i], a[best]); i = best;
    }
}
void heapsort(vector<int>& a) {
    int n = a.size();
    for (int i = n/2-1; i >= 0; i--) siftDown(a, i, n);
    for (int e = n-1; e > 0; e--) { swap(a[0], a[e]); siftDown(a, 0, e); }
}

// -------- Mergesort (O(n) aux via single scratch buffer) --------
void mergesort(vector<int>& a, int lo, int hi, vector<int>& aux) {
    if (hi - lo < 2) return;
    int mid = (lo + hi) / 2;
    mergesort(a, lo, mid, aux);
    mergesort(a, mid, hi, aux);
    copy(a.begin()+lo, a.begin()+hi, aux.begin()+lo);
    merge(aux.begin()+lo, aux.begin()+mid,
          aux.begin()+mid, aux.begin()+hi,
          a.begin()+lo);
}

double timeSort(vector<int> data, const string& algo, vector<int>& aux) {
    auto t0 = Clock::now();
    if      (algo == "heap")  heapsort(data);
    else if (algo == "merge") mergesort(data, 0, data.size(), aux);
    else                      sort(data.begin(), data.end());
    return chrono::duration<double, milli>(Clock::now() - t0).count();
}

int main() {
    mt19937 rng(42);
    for (int n : {10000, 100000, 500000}) {
        vector<int> rnd(n), asc(n), desc(n);
        iota(asc.begin(), asc.end(), 0);
        desc = asc; reverse(desc.begin(), desc.end());
        for (auto& x : rnd) x = rng();
        vector<int> aux(n);

        cout << "=== n = " << n << " ===\n\n";
        cout << setw(12) << "Input"
             << setw(18) << "heapsort (ms)"
             << setw(18) << "mergesort (ms)"
             << setw(18) << "std::sort (ms)"
             << "\n" << string(66, '-') << "\n";

        for (auto& [label, src] :
             vector<pair<string,vector<int>&>>{
                 {"random", rnd}, {"sorted", asc}, {"reverse", desc}}) {
            cout << setw(12) << label
                 << setw(18) << fixed << setprecision(2) << timeSort(src,"heap",aux)
                 << setw(18) << timeSort(src,"merge",aux)
                 << setw(18) << timeSort(src,"std",aux) << "\n";
        }

        cout << "\nAuxiliary space:\n"
             << "  heapsort:  O(1)    " << sizeof(int) << " bytes\n"
             << "  mergesort: O(n)    " << n*sizeof(int) << " bytes  ("
             << n*sizeof(int)/1024 << " KB)\n"
             << "  std::sort: O(log n) stack frames\n\n";
    }
}
