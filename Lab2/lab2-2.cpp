#include <iostream>
#include <vector>
#include <iomanip>
#include <numeric>
#include <algorithm>
#include <random>
using namespace std;

long long comparisons;

// (same sort implementations as Program 1 — paste them here)
void selectionSort(vector<int> v) {
    int n = v.size();
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) { comparisons++; if (v[j] < v[minIdx]) minIdx = j; }
        swap(v[i], v[minIdx]);
    }
}

void insertionSort(vector<int> v) {
    int n = v.size();
    for (int i = 1; i < n; i++) {
        int key = v[i], j = i - 1;
        while (j >= 0) { comparisons++; if (v[j] > key) { v[j+1] = v[j]; j--; } else break; }
        v[j + 1] = key;
    }
}

void merge(vector<int>& v, int lo, int mid, int hi) {
    vector<int> tmp; int i = lo, j = mid + 1;
    while (i <= mid && j <= hi) { comparisons++; if (v[i] <= v[j]) tmp.push_back(v[i++]); else tmp.push_back(v[j++]); }
    while (i <= mid) tmp.push_back(v[i++]);
    while (j <= hi)  tmp.push_back(v[j++]);
    for (int k = 0; k < (int)tmp.size(); k++) v[lo + k] = tmp[k];
}

void mergeSort(vector<int>& v, int lo, int hi) {
    if (lo >= hi) return;
    int mid = lo + (hi - lo) / 2;
    mergeSort(v, lo, mid); mergeSort(v, mid+1, hi); merge(v, lo, mid, hi);
}

int partition(vector<int>& v, int lo, int hi) {
    int pivot = v[hi], i = lo - 1;
    for (int j = lo; j < hi; j++) { comparisons++; if (v[j] <= pivot) swap(v[++i], v[j]); }
    swap(v[i+1], v[hi]); return i + 1;
}

void quickSort(vector<int>& v, int lo, int hi) {
    if (lo < hi) { int p = partition(v, lo, hi); quickSort(v, lo, p-1); quickSort(v, p+1, hi); }
}

struct Result { long long sel, ins, ms, qs; };

Result measure(vector<int> data) {
    Result r;
    comparisons = 0; selectionSort(data);                                r.sel = comparisons;
    comparisons = 0; insertionSort(data);                                r.ins = comparisons;
    comparisons = 0; { vector<int> t = data; mergeSort(t, 0, (int)t.size()-1); } r.ms = comparisons;
    comparisons = 0; { vector<int> t = data; quickSort(t, 0, (int)t.size()-1); } r.qs = comparisons;
    return r;
}

void printRow(const string& label, Result r) {
    cout << setw(16) << label
         << setw(14) << r.sel
         << setw(14) << r.ins
         << setw(14) << r.ms
         << setw(14) << r.qs << "\n";
}

int main() {
    const int N = 1000;
    mt19937 rng(42);

    vector<int> sorted(N), rev(N), rand_data(N);
    iota(sorted.begin(), sorted.end(), 0);
    iota(rev.begin(), rev.end(), 0); reverse(rev.begin(), rev.end());
    iota(rand_data.begin(), rand_data.end(), 0);
    shuffle(rand_data.begin(), rand_data.end(), rng);

    cout << "\n--- Comparison Counts by Input Shape (n = 1000) ---\n\n";
    cout << setw(16) << "Input"
         << setw(14) << "Selection"
         << setw(14) << "Insertion"
         << setw(14) << "Merge"
         << setw(14) << "Quick"
         << "\n" << string(72, '-') << "\n";

    printRow("Random",         measure(rand_data));
    printRow("Already sorted", measure(sorted));
    printRow("Reverse sorted", measure(rev));
    cout << "\n";
}
