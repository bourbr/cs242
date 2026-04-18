#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <numeric>
using namespace std;
using Clock = chrono::high_resolution_clock;
using us    = chrono::duration<double, micro>;

struct Node {
    int data; Node* next;
    Node(int v) : data(v), next(nullptr) {}
};

// --- Linked list operations ---
void ll_pushFront(Node*& head, int val) {
    Node* n = new Node(val);
    n->next = head; head = n;
}

int ll_getAt(Node* head, int index) {
    Node* c = head;
    for (int i = 0; i < index; i++) c = c->next;
    return c->data;
}

void ll_clear(Node*& head) {
    while (head) { Node* t = head->next; delete head; head = t; }
}

// --- Array-based list (std::vector) operations ---

template<typename Fn>
double timeUs(Fn fn, int reps = 200) {
    auto t0 = Clock::now();
    for (int i = 0; i < reps; i++) fn();
    return us(Clock::now() - t0).count() / reps;
}

int main() {
    vector<int> sizes = {1000, 5000, 10000, 50000, 100000};

    // ---- INSERT AT FRONT ----
    cout << "\n--- Insert at Front: " 
         << "Linked list pushFront vs vector insert(begin) (microseconds) ---\n\n";
    cout << setw(10) << "n"
         << setw(20) << "Linked pushFront"
         << setw(20) << "Vector insert front"
         << "\n" << string(50, '-') << "\n";

    for (int n : sizes) {
        // Linked list: push n elements to front
        double ll_time = timeUs([&]{
            Node* head = nullptr;
            for (int i = 0; i < n; i++) ll_pushFront(head, i);
            ll_clear(head);
        }, 20);

        // Vector: insert n elements at front (index 0)
        double vec_time = timeUs([&]{
            vector<int> v;
            v.reserve(n);
            for (int i = 0; i < n; i++) v.insert(v.begin(), i);
        }, 5);

        cout << setw(10) << n
             << setw(20) << fixed << setprecision(1) << ll_time
             << setw(20) << vec_time << "\n";
    }

    // ---- INSERT AT BACK ----
    cout << "\n--- Insert at Back: "
         << "Linked list (no tail) walk-to-end vs vector push_back (microseconds) ---\n\n";
    cout << setw(10) << "n"
         << setw(22) << "Linked (walk to back)"
         << setw(20) << "Vector push_back"
         << "\n" << string(52, '-') << "\n";

    for (int n : sizes) {
        // Linked list without tail pointer: walk to end each time
        double ll_back = timeUs([&]{
            Node* head = nullptr;
            for (int i = 0; i < n; i++) {
                Node* newN = new Node(i);
                if (!head) { head = newN; continue; }
                Node* c = head;
                while (c->next) c = c->next;  // walk to end — O(n)
                c->next = newN;
            }
            ll_clear(head);
        }, 5);

        double vec_back = timeUs([&]{
            vector<int> v;
            for (int i = 0; i < n; i++) v.push_back(i);
        }, 20);

        cout << setw(10) << n
             << setw(22) << fixed << setprecision(1) << ll_back
             << setw(20) << vec_back << "\n";
    }

    // ---- ACCESS BY INDEX ----
    cout << "\n--- Access by Index: "
         << "Linked list walk vs vector [] (nanoseconds, single access) ---\n\n";
    cout << setw(10) << "n"
         << setw(22) << "Linked getAt(n/2)"
         << setw(22) << "Vector v[n/2]"
         << "\n" << string(54, '-') << "\n";

    for (int n : sizes) {
        // Build both structures once
        Node* head = nullptr;
        vector<int> vec(n);
        for (int i = n-1; i >= 0; i--) ll_pushFront(head, i);
        iota(vec.begin(), vec.end(), 0);

        using ns = chrono::duration<double, nano>;
        int midIdx = n / 2;
        volatile int sink = 0;  // prevent optimization

        double ll_acc = timeUs([&]{
            sink = ll_getAt(head, midIdx);
        }, 5000);

        double vec_acc = timeUs([&]{
            sink = vec[midIdx];
        }, 500000);

        // Report in nanoseconds for readability
        cout << setw(10) << n
             << setw(22) << fixed << setprecision(3) << ll_acc * 1000
             << setw(22) << vec_acc * 1000 << "\n";

        ll_clear(head);
    }

    cout << "\n";
}
