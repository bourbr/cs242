#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <numeric>
#include <algorithm>
#include <random>
using namespace std;

using Clock = chrono::high_resolution_clock;
using us    = chrono::duration<double, micro>;

struct Node {
    int data;
    Node* next;
    Node(int v) : data(v), next(nullptr) {}
};

void clearList(Node*& h) {
    while (h) {
        Node* t = h->next;
        delete h;
        h = t;
    }
}

// Build a RANDOMIZED (scattered) linked list
Node* buildRandomList(int n) {
    vector<Node*> nodes;
    nodes.reserve(n);

    // allocate nodes
    for (int i = 0; i < n; i++)
        nodes.push_back(new Node(i));

    // shuffle pointers to destroy locality
    random_device rd;
    mt19937 g(rd());
    shuffle(nodes.begin(), nodes.end(), g);

    // link them
    for (int i = 0; i < n - 1; i++)
        nodes[i]->next = nodes[i + 1];
    nodes[n - 1]->next = nullptr;

    return nodes[0]; // head
}

template<typename Fn>
double timeUs(Fn fn, int reps = 1000) {
    // warm-up (important!)
    for (int i = 0; i < 100; i++) fn();

    auto t0 = Clock::now();
    for (int i = 0; i < reps; i++) fn();
    return us(Clock::now() - t0).count() / reps;
}

int main() {
    cout << "\n=== Cache Locality Experiment: Vector vs Linked List ===\n\n";

    cout << setw(12) << "n"
         << setw(20) << "Vector (us)"
         << setw(22) << "Linked List (us)"
         << setw(14) << "Slowdown"
         << "\n" << string(70, '-') << "\n";

    vector<int> sizes = {100000, 500000, 1000000};

    for (int n : sizes) {
        // contiguous array
        vector<int> v(n);
        iota(v.begin(), v.end(), 0);

        // scattered linked list
        Node* ll = buildRandomList(n);

        volatile long long sink = 0;

        double vec_t = timeUs([&] {
            long long s = 0;
            for (int x : v) s += x;
            sink = s;
        }, 500);

        double ll_t = timeUs([&] {
            long long s = 0;
            for (Node* c = ll; c; c = c->next)
                s += c->data;
            sink = s;
        }, 500);

        cout << setw(12) << n
             << setw(20) << fixed << setprecision(2) << vec_t
             << setw(22) << ll_t
             << setw(13) << setprecision(1) << (ll_t / vec_t) << "x\n";

        clearList(ll);
    }

    cout << "\n";
}
