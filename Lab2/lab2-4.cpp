#include <iostream>
#include <vector>
#include <iomanip>
#include <numeric>
#include <algorithm>
#include <random>
#include <string>
using namespace std;

// --- Traced Merge Sort ---
int mergeDepth = 0;

void tracedMerge(vector<int>& v, int lo, int mid, int hi) {
    string indent(mergeDepth * 2, ' ');
    vector<int> tmp; int i = lo, j = mid+1;
    while (i <= mid && j <= hi)
        tmp.push_back(v[i] <= v[j] ? v[i++] : v[j++]);
    while (i <= mid) tmp.push_back(v[i++]);
    while (j <= hi)  tmp.push_back(v[j++]);
    for (int k = 0; k < (int)tmp.size(); k++) v[lo+k] = tmp[k];
    cout << indent << "merge [" << lo << ".." << hi << "] → ";
    for (int k = lo; k <= hi; k++) cout << v[k] << " ";
    cout << "\n";
}

void tracedMergeSort(vector<int>& v, int lo, int hi) {
    string indent(mergeDepth * 2, ' ');
    if (lo >= hi) {
        cout << indent << "base [" << lo << "] = " << v[lo] << "\n";
        return;
    }
    cout << indent << "split [" << lo << ".." << hi << "]\n";
    int mid = lo + (hi-lo)/2;
    mergeDepth++;
    tracedMergeSort(v, lo, mid);
    tracedMergeSort(v, mid+1, hi);
    mergeDepth--;
    tracedMerge(v, lo, mid, hi);
}

// --- Traced Quicksort ---
int quickDepth = 0;

int tracedPartition(vector<int>& v, int lo, int hi) {
    string indent(quickDepth * 2, ' ');
    int pivot = v[hi], i = lo-1;
    cout << indent << "partition [" << lo << ".." << hi << "] pivot=" << pivot << "\n";
    for (int j = lo; j < hi; j++)
        if (v[j] <= pivot) swap(v[++i], v[j]);
    swap(v[i+1], v[hi]);
    int p = i+1;
    cout << indent << "  → pivot " << pivot << " placed at index " << p << " | left: [";
    for (int k = lo; k < p; k++) cout << v[k] << (k<p-1?",":"");
    cout << "] right: [";
    for (int k = p+1; k <= hi; k++) cout << v[k] << (k<hi?",":"");
    cout << "]\n";
    return p;
}

void tracedQuickSort(vector<int>& v, int lo, int hi) {
    if (lo < hi) {
        quickDepth++;
        int p = tracedPartition(v, lo, hi);
        tracedQuickSort(v, lo, p-1);
        tracedQuickSort(v, p+1, hi);
        quickDepth--;
    }
}

int main() {
    vector<int> data = {38, 27, 43, 3, 9, 82, 10};

    cout << "\n=== Merge Sort Trace (n=7) ===\n\n";
    { vector<int> v = data; tracedMergeSort(v, 0, (int)v.size()-1); }

    cout << "\n=== Quicksort Trace (n=7) ===\n\n";
    { quickDepth = 0; vector<int> v = data; tracedQuickSort(v, 0, (int)v.size()-1); }

    // Now show what happens to quicksort on sorted input
    cout << "\n=== Quicksort Trace on SORTED input (n=7, worst case) ===\n\n";
    { quickDepth = 0; vector<int> v = {3, 9, 10, 27, 38, 43, 82}; tracedQuickSort(v, 0, (int)v.size()-1); }

    cout << "\n";
}
