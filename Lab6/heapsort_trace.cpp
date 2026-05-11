#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

// 0-indexed; children of i are 2i+1 and 2i+2
void siftDown(vector<int>& a, int root, int sz) {
    while (true) {
        int largest = root, l = 2*root+1, r = 2*root+2;
        if (l < sz && a[l] > a[largest]) largest = l;
        if (r < sz && a[r] > a[largest]) largest = r;
        if (largest == root) break;
        swap(a[root], a[largest]);
        root = largest;
    }
}

void printArr(const vector<int>& a, int boundary = -1) {
    cout << "[ ";
    for (int i = 0; i < (int)a.size(); i++) {
        if (i == boundary) cout << "| ";   // unsorted | sorted
        cout << setw(3) << a[i];
    }
    cout << " ]\n";
}

int main() {
    vector<int> a = {5, 3, 8, 1, 9, 2, 7, 4, 6};
    int n = a.size();

    cout << "Input:  "; printArr(a); cout << "\n";

    cout << "=== Phase 1: Heapify (bottom-up sift-down) ===\n\n";
    for (int i = n/2 - 1; i >= 0; i--) {
        siftDown(a, i, n);
        cout << "siftDown from index " << i << ":  "; printArr(a);
    }
    cout << "\nHeap built.  Root = " << a[0] << " (maximum).\n\n";

    cout << "=== Phase 2: Sort-down ===\n\n";
    for (int end = n-1; end > 0; end--) {
        cout << "swap a[0]=" << a[0] << " with a[" << end << "]=" << a[end] << ":  ";
        swap(a[0], a[end]);
        siftDown(a, 0, end);
        printArr(a, end);
    }

    cout << "\nSorted: "; printArr(a);
}
