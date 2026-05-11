#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

class MaxHeap {
    vector<int> h;   // 1-indexed; h[0] unused sentinel
public:
    MaxHeap() { h.push_back(0); }

    void insert(int key) {
        h.push_back(key);
        int i = h.size() - 1;
        cout << "insert(" << key << "):\n";
        while (i > 1 && h[i / 2] < h[i]) {
            cout << "  sift-up: swap h[" << i << "]=" << h[i]
                 << " with h[" << i/2 << "]=" << h[i/2] << "\n";
            swap(h[i], h[i / 2]);
            i /= 2;
        }
        printArray();
    }

    int extractMax() {
        int maxVal = h[1];
        cout << "extractMax() -> " << maxVal << "\n";
        cout << "  move h[" << h.size()-1 << "]=" << h.back() << " to root\n";
        h[1] = h.back();
        h.pop_back();
        int i = 1, n = h.size() - 1;
        while (true) {
            int largest = i;
            if (2*i   <= n && h[2*i]   > h[largest]) largest = 2*i;
            if (2*i+1 <= n && h[2*i+1] > h[largest]) largest = 2*i+1;
            if (largest == i) break;
            cout << "  sift-down: swap h[" << i << "]=" << h[i]
                 << " with h[" << largest << "]=" << h[largest] << "\n";
            swap(h[i], h[largest]);
            i = largest;
        }
        printArray();
        return maxVal;
    }

    void printArray() {
        cout << "  heap: [ ";
        for (int j = 1; j < (int)h.size(); j++) cout << setw(3) << h[j];
        cout << " ]\n\n";
    }
};

int main() {
    MaxHeap heap;
    for (int key : {10, 4, 15, 7, 20, 3, 18, 9})
        heap.insert(key);

    cout << "--- Extractions ---\n\n";
    for (int i = 0; i < 3; i++)
        heap.extractMax();
}
