#include <iostream>
#include <vector>
#include <iomanip>
#include <numeric>
#include <algorithm>
#include <random>
using namespace std;

long long comparisons;

void selectionSort(vector<int> v) {
    int n = v.size();
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            comparisons++;
            if (v[j] < v[minIdx]) minIdx = j;
        }
        swap(v[i], v[minIdx]);
    }
}

void insertionSort(vector<int> v) {
    int n = v.size();
    for (int i = 1; i < n; i++) {
        int key = v[i], j = i - 1;
        while (j >= 0) {
            comparisons++;
            if (v[j] > key) { v[j + 1] = v[j]; j--; }
            else break;
        }
        v[j + 1] = key;
    }
}

void merge(vector<int>& v, int lo, int mid, int hi) {
    vector<int> tmp;
    int i = lo, j = mid + 1;
    while (i <= mid && j <= hi) {
        comparisons++;
        if (v[i] <= v[j]) tmp.push_back(v[i++]);
        else               tmp.push_back(v[j++]);
    }
    while (i <= mid) tmp.push_back(v[i++]);
    while (j <= hi)  tmp.push_back(v[j++]);
    for (int k = 0; k < (int)tmp.size(); k++) v[lo + k] = tmp[k];
}

void mergeSort(vector<int>& v, int lo, int hi) {
    if (lo >= hi) return;
    int mid = lo + (hi - lo) / 2;
    mergeSort(v, lo, mid);
    mergeSort(v, mid + 1, hi);
    merge(v, lo, mid, hi);
}

int partition(vector<int>& v, int lo, int hi) {
    int pivot = v[hi], i = lo - 1;
    for (int j = lo; j < hi; j++) {
        comparisons++;
        if (v[j] <= pivot) swap(v[++i], v[j]);
    }
    swap(v[i + 1], v[hi]);
    return i + 1;
}

void quickSort(vector<int>& v, int lo, int hi) {
    if (lo < hi) {
        int p = partition(v, lo, hi);
        quickSort(v, lo, p - 1);
        quickSort(v, p + 1, hi);
    }
}

long long run(void (*sortFn)(vector<int>), vector<int> data) {
    comparisons = 0;
    sortFn(data);
    return comparisons;
}

long long runRef(void (*sortFn)(vector<int>&, int, int), vector<int> data) {
    comparisons = 0;
    sortFn(data, 0, (int)data.size() - 1);
    return comparisons;
}

int main() {
    mt19937 rng(42);
    vector<int> sizes = {100, 500, 1000, 2000, 5000};

    cout << "\n--- Comparison Counts on Random Input ---\n\n";
    cout << setw(7) << "n"
         << setw(14) << "Selection"
         << setw(14) << "Insertion"
         << setw(14) << "Merge"
         << setw(14) << "Quick"
         << "\n" << string(63, '-') << "\n";

    for (int n : sizes) {
        vector<int> data(n);
        iota(data.begin(), data.end(), 0);
        shuffle(data.begin(), data.end(), rng);

        long long sel = run(selectionSort, data);
        long long ins = run(insertionSort, data);

        long long ms_cmp = 0; comparisons = 0;
        { vector<int> tmp = data; mergeSort(tmp, 0, n-1); ms_cmp = comparisons; }

        long long qs_cmp = 0; comparisons = 0;
        { vector<int> tmp = data; quickSort(tmp, 0, n-1); qs_cmp = comparisons; }

        cout << setw(7) << n
             << setw(14) << sel
             << setw(14) << ins
             << setw(14) << ms_cmp
             << setw(14) << qs_cmp << "\n";
    }

    // Theoretical values for reference
    cout << "\n--- Theoretical Reference (approximate) ---\n\n";
    cout << setw(7) << "n"
         << setw(14) << "n(n-1)/2"
         << setw(14) << "n*log2(n)"
         << "\n" << string(35, '-') << "\n";
    for (int n : sizes) {
        long long quad = (long long)n * (n - 1) / 2;
        double   nlogn = n * log2(n);
        cout << setw(7) << n
             << setw(14) << quad
             << setw(14) << (long long)nlogn << "\n";
    }
    cout << "\n";
}
