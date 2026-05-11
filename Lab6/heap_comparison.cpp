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
