# Lab 6: Observing Heaps and Heapsort

**CPTR 242 — Sequential and Parallel Data Structures and Algorithms**
Walla Walla University

---

## Overview

In this lab you will **run complete programs and observe their behavior**. There is no coding required. Your job is to read the output carefully, record what you see, and reason about what it means.

By the end you should be able to:

- Trace a max-heap through a sequence of insertions and deletions using array-index arithmetic
- Explain why a complete binary tree maps perfectly onto an array with no pointers
- Compare the memory footprint and runtime overhead of an array heap versus a linked-node heap
- Describe the two phases of heapsort and explain why the heapify phase runs in O(n), not O(n log n)
- Compare heapsort's time and space usage against mergesort and std::sort, and reason about when each is preferable

---

## Background: Priority and Shape

A **heap** is a complete binary tree satisfying the **heap property**: in a max-heap, every node's key is greater than or equal to its children's keys. Because the tree is always *complete* — every level full except possibly the last, filled left-to-right — it maps perfectly onto a flat array with no wasted space and no pointers.

For a node at 1-based index `i`:

| Relationship | Index formula |
|---|---|
| Left child   | `2i`     |
| Right child  | `2i + 1` |
| Parent       | `i / 2`  |

This arithmetic is the entire "structure" of the tree. Inserting a key appends it to the array and **sifts up** (swaps with its parent while the heap property is violated). Removing the maximum swaps the root with the last element, shrinks the array by one, and **sifts down** (swaps with the larger child while the heap property is violated). Both operations touch at most O(log n) nodes — the height of the tree.

The central question: **does representing the same logical structure as a linked tree instead of an array change correctness, and what does it cost?**

---

## Setup

All programs are self-contained. Each generates its own test data and prints results directly to the terminal.

**Compiler command for all programs:**
```bash
g++ -O0 -o program program.cpp && ./program
```

> `-O0` disables compiler optimizations so you observe true algorithm behavior.

---

## Model 1: The Array Heap — Sift-Up on Insertion

Before measuring anything, you need to see exactly what happens inside the array on every insertion. This program builds a max-heap by inserting keys one at a time and prints the full array and a swap trace after each insertion. It then performs several extractions with sift-down traces.

### Program 1 — `array_heap.cpp`

```cpp
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
```

### Observation Table 1a — Array State After Each Insertion

Run the program and fill in the array contents (h[1] through h[8]) after each insertion.

| After inserting | h[1] | h[2] | h[3] | h[4] | h[5] | h[6] | h[7] | h[8] |
|---|---|---|---|---|---|---|---|---|
| 10 | 10 | | | | | | | |
| 4  | 10 | 4 | | | | | | |
| 15 | 15 | 10 | 4 | | | | | |
| 7  | 15 | 10 | 7 | 4 | | | | |
| 20 | 20 | 15 | 10 | 7 | 4 | | | |
| 3  | 20 | 15 | 10 | 7 | 4 | 3 | | |
| 18 | 20 | 18 | 15 | 10 | 7 | 4 | 3 | |
| 9  | 20 | 18 | 15 | 10 | 9 | 7 | 4 | 3 |

### Observation Table 1b — Extraction Sequence

| Extraction # | Value returned | Number of sift-down swaps |
|---|---|---|
| 1st | 20 | 2|
| 2nd | 18 | 2|
| 3rd | 15 | 1|

---

### Critical Thinking Questions — Model 1

**Q1.** What value always sits at `h[1]` after every insertion, regardless of the order keys arrive? Why does the heap property guarantee this?

> Your answer: The max value of the heap, because the key which sits in h[1] has to reach the base case of the algorithm (so to speak), meaning there are no values above it violating the heap property. 

**Q2.** When 20 is inserted, it sifts all the way to the root. Trace the swaps by hand using the formula `parent = i / 2` starting from the index where 20 lands. Do the printed swaps match your calculation?

> Your answer: Starting from h[5], 5/2 = 2. h[2], 2/2 = 1. h[1]. So the key 20 uses the parent finding algorithm to find the parent node relative to its position and sift up if the heap property is violated.

**Q3.** `extractMax` moves the *last* element in the array to the root before sifting down. Why the last element specifically? What structural property of a complete binary tree makes this the correct choice?

> Your answer: Possibly because the last element, or leaf, has no children; it is not the parent node of any other element, and so is the structurally simplest to re-assign. Using an element with children would require sifting for that element, and another sifting operation for an element nearby to deal with the gap left over.

**Q4.** The heap has 8 nodes, so its height is ⌊log₂ 8⌋ = 3. From Table 1b, did any extraction require more than 3 sift-down swaps? Explain why this bound holds.

> Your answer: No extraction required more than 3 sift-down operations, and this is due to the binary structure of the heap-array, each node having 2 children. This allows division by 2 to find the parent of any given node, which like binary search, imparts O(logn) efficiency.

---

## Model 2: Array Heap vs. Linked-Node Heap — Memory and Speed

An array heap requires no pointers. A linked-node heap stores explicit `left`, `right`, and `parent` pointers in every node. This program builds both heaps from identical insertions and reports the memory consumed by each representation at several values of n, then times n insertions for both.

### Program 2 — `heap_comparison.cpp`

```cpp
#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
using namespace std;
using Clock = chrono::high_resolution_clock;

// -------- Array heap --------
struct ArrayHeap {
    vector<int> h;
    ArrayHeap() { h.push_back(0); }
    void insert(int v) {
        h.push_back(v);
        int i = h.size() - 1;
        while (i > 1 && h[i/2] < h[i]) { swap(h[i], h[i/2]); i /= 2; }
    }
    size_t memBytes() const { return h.capacity() * sizeof(int); }
};

// -------- Linked-node heap --------
struct Node {
    int val;
    Node *left, *right, *parent;
    Node(int v) : val(v), left(nullptr), right(nullptr), parent(nullptr) {}
};

struct LinkedHeap {
    Node* root = nullptr;
    int   sz   = 0;

    // Navigate to 1-based index idx by reading its binary representation
    Node* nodeAt(int idx) {
        string bits;
        for (int x = idx; x > 1; x >>= 1) bits += ((x & 1) ? 'R' : 'L');
        Node* cur = root;
        for (int i = bits.size() - 1; i >= 0; i--)
            cur = (bits[i] == 'L') ? cur->left : cur->right;
        return cur;
    }

    void insert(int v) {
        sz++;
        Node* n = new Node(v);
        if (!root) { root = n; return; }
        Node* p = nodeAt(sz / 2);
        n->parent = p;
        (p->left ? p->right : p->left) = n;
        while (n->parent && n->parent->val < n->val) {
            swap(n->val, n->parent->val);
            n = n->parent;
        }
    }

    size_t memBytes() const { return (size_t)sz * sizeof(Node); }

    ~LinkedHeap() {
        if (!root) return;
        vector<Node*> q = {root};
        for (int i = 0; i < (int)q.size(); i++) {
            if (q[i]->left)  q.push_back(q[i]->left);
            if (q[i]->right) q.push_back(q[i]->right);
            delete q[i];
        }
    }
};

int main() {
    cout << "sizeof(int)  = " << sizeof(int)  << " bytes\n";
    cout << "sizeof(Node) = " << sizeof(Node)
         << " bytes  (val + 3 pointers)\n\n";

    vector<int> sizes = {1000, 10000, 100000, 500000};

    cout << "=== Memory Usage ===\n\n";
    cout << setw(10) << "n"
         << setw(22) << "Array heap (bytes)"
         << setw(22) << "Linked heap (bytes)"
         << setw(12) << "Ratio"
         << "\n" << string(66, '-') << "\n";

    for (int n : sizes) {
        ArrayHeap ah;  LinkedHeap lh;
        for (int i = 0; i < n; i++) {
            int v = (i * 2654435761u) >> 16;
            ah.insert(v);  lh.insert(v);
        }
        cout << setw(10) << n
             << setw(22) << ah.memBytes()
             << setw(22) << lh.memBytes()
             << setw(11) << fixed << setprecision(1)
             << (double)lh.memBytes() / ah.memBytes() << "x\n";
    }

    cout << "\n=== Insertion Time ===\n\n";
    cout << setw(10) << "n"
         << setw(22) << "Array heap (ms)"
         << setw(22) << "Linked heap (ms)"
         << "\n" << string(54, '-') << "\n";

    for (int n : sizes) {
        ArrayHeap ah;
        auto t0 = Clock::now();
        for (int i = 0; i < n; i++) ah.insert((i * 2654435761u) >> 16);
        double ams = chrono::duration<double, milli>(Clock::now() - t0).count();

        LinkedHeap lh;
        auto t1 = Clock::now();
        for (int i = 0; i < n; i++) lh.insert((i * 2654435761u) >> 16);
        double lms = chrono::duration<double, milli>(Clock::now() - t1).count();

        cout << setw(10) << n
             << setw(22) << fixed << setprecision(3) << ams
             << setw(22) << lms << "\n";
    }
}
```

### Observation Table 2a — Memory Usage

| n | Array heap (bytes) | Linked heap (bytes) | Ratio (linked / array) |
------------------------------------------------------------------
      1000                  4264                 32000        7.5x
     10000                 48552                320000        6.6x
    100000                553020               3200000        5.8x
    500000               2799652              16000000        5.7x

### Observation Table 2b — Insertion Time

|n              | Array heap (ms) | Linked heap (ms) |
------------------------------------------------------
      1000                 0.145                 0.326
     10000                 1.216                 4.565
    100000                 5.830                46.859
    500000                25.788               297.752

---

### Critical Thinking Questions — Model 2

**Q5.** The program prints `sizeof(int)` and `sizeof(Node)` at startup. Using those two numbers, compute the *theoretical* ratio of linked-to-array memory and compare it to the ratios you measured in Table 2a. Does the ratio stay constant as n grows? What explains any discrepancy at small n?

> Your answer: The ratio is 32/4 which is 8 times the memory used by the Linked Heap versus the Array Heap. The ratio narrows as the input n increases. The array structure has larger container overhead which makes memory use less efficient looking at smaller n, while the linked list has per-node memory use overhead. At larger values of n however, the array memory use is 'amortized', while the linked list memory use continues to grow linearly.

**Q6.** The linked heap's `nodeAt(idx)` navigates from the root to the target node by following pointers level by level. The array heap computes the parent in one step with `i / 2`. Both are O(log n) in theory, but with very different constants. Explain in terms of CPU cache behavior why pointer chasing is slower than index arithmetic, referencing what you observed about linked lists vs. arrays in Lab 4.

> Your answer: Pointer chasing almost always results in cache-misses. This means each operation checks the cache and then has to find the element in memory via the pointer. Arrays made up of the same data type can be stored in cache memory contiguously, and quickly retrieved, in the case of a heap with on O(1) arithmetic operation. The difference between the two is like having your next assignment in your desk drawer, versus checking the drawer and finding a note explaining you have to go to your basement to get the assignment.

**Q7.** Given what Tables 2a and 2b show, propose at least one realistic scenario where you would choose the linked-node heap over the array heap despite its overhead.

> Your answer: So, one naive reason would be the linked-list structure's capacity for constant or sporadic growth without need for resizing. A more subtle reason would relate to data structures in which tracking of elements continuously through their life-cycle (so to speak) is required or desireable. As a node is an object in memory, it remains 'itself' wherever it is moved in the data structure. With the array structure, swapping and especially copying and resizing move elements around in memory and so break this stability.

---

## Model 3: Heapsort — Heapify and Sort-Down

Heapsort sorts an array in two phases. **Phase 1 (heapify):** rearrange the whole array into a max-heap in O(n) time by applying sift-down from the bottom of the tree upward. **Phase 2 (sort-down):** repeatedly swap the root (the current maximum) to the end of the unsorted region and sift the new root down. This program prints the full array after every step of both phases so you can trace the sort exactly.

### Program 3 — `heapsort_trace.cpp`

```cpp
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
```

### Observation Table 3a — Array After Each Heapify Step

| siftDown from index | Array contents after step |
|---|---|
| (initial) | 5, 3, 8, 1, 9, 2, 7, 4, 6 |
siftDown from index 3:  [   5  3  8  6  9  2  7  4  1 ]
siftDown from index 2:  [   5  3  8  6  9  2  7  4  1 ]
siftDown from index 1:  [   5  9  8  6  3  2  7  4  1 ]
siftDown from index 0:  [   9  6  8  5  3  2  7  4  1 ]

### Observation Table 3b — Sort-Down Phase

Record the value moved into the sorted region and the remaining unsorted size after each step.

| Step | Value placed in sorted region | Unsorted size remaining |
|---|---|---|
| 1 | 9 | 8|
| 2 | 8 9 | 7|
| 3 | 7 8 9 | 6|
| 4 | 6 7 8 9 | 5|
| 5 | 5 6 7 8 9 | 4|
| 6 | 4 5 6 7 8 9 | 3|
| 7 | 3 4 5 6 7 8 9 | 2|
| 8 | 2 3 4 5 6 7 8 9 | 1|

---

### Critical Thinking Questions — Model 3

**Q8.** The heapify phase starts at index `n/2 - 1` and works *downward* to 0. Why not start at `n - 1`? What kind of nodes occupy indices `n/2` through `n - 1`, and why do they need no sifting?

> Your answer: Those elements are leaf nodes and have no children, so they can't sift down or swap down with any other elements. 

**Q9.** After Phase 1, the maximum is at index 0. Phase 2 swaps it to the last position, temporarily placing a small value at the root and breaking the heap property. Why is it still correct to call `siftDown(a, 0, end)` with the sorted tail excluded? What guarantee does sift-down restore?

> Your answer: Siftdown will move the element downward until it is larger than the element beneath it, restoring the heap property.

---

## Model 4: Heapsort vs. Mergesort vs. std::sort — Time and Space

Heapsort's O(n log n) guarantee comes with a practical cost: non-sequential memory access causes frequent cache misses. Mergesort accesses memory sequentially but requires O(n) auxiliary space. `std::sort` uses a hybrid strategy that attempts to get the best of both worlds. This program benchmarks all three on random, already-sorted, and reverse-sorted inputs, and separately reports the auxiliary memory each algorithm requires.

### Program 4 — `sort_comparison.cpp`

```cpp
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
```

### Observation Table 4a — Sorting Time (ms)

**n = 10,000**

| Input order | heapsort (ms) | mergesort (ms) | std::sort (ms) |
------------------------------------------------------------------
      random              6.56              8.09              3.88
      sorted              5.43              5.08              0.71
     reverse              6.30             12.23              0.95

**n = 100,000**

| Input order | heapsort (ms) | mergesort (ms) | std::sort (ms) |
------------------------------------------------------------------
      random             90.75             79.22             36.71
      sorted             71.19             61.42             10.74
     reverse             61.75             63.20             12.06

**n = 500,000**

| Input order | heapsort (ms) | mergesort (ms) | std::sort (ms) |
------------------------------------------------------------------
      random            481.36            344.12            245.19
      sorted            305.36            241.11             51.32
     reverse            279.15            239.57             61.83

### Observation Table 4b — Auxiliary Memory at n = 500,000

| Algorithm | Space complexity | Auxiliary bytes |
|---|---|---|
  heapsort:     O(1)             4 bytes
  mergesort:    O(n)       2000000 bytes (1953 KB)
| std::sort | O(log n) | *(stack, not measured)* |

---

### Critical Thinking Questions — Model 4

**Q10.** Across all three input orders and all three values of n, which algorithm is consistently fastest? Which is consistently slowest? Does input order affect runtime meaningfully? Explain why or why not this would be the case, referencing heapsort's algorithm nature.

> Your answer: std::sort is fastest, while heapsort is slowest, being least able to take advantage of sorted inputs, building a heap algorithmically moving elements and sifting down regardless of the order of the input elements. std::sort, having access to insertion sort, benefits from pre-sorted inputs.

**Q11.** `std::sort` uses **introsort**: it starts with quicksort but falls back to heapsort when recursion depth exceeds 2·⌊log₂ n⌋, preventing O(n²) worst-case behavior. Based on Table 4a, does `std::sort` appear to behave more like heapsort or something significantly faster? What does that tell you about how often the heapsort fallback is actually triggered in practice?

> Your answer: It suggests that recourse to the heapsort algorithm is unusual, but also that in the event this is necessary, it's the optimal approach for the situation, unlike say when heapsort performs its general algorithm on sorted data.

---
