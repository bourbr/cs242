#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <numeric>
using namespace std;
using Clock = chrono::high_resolution_clock;
using us    = chrono::duration<double, micro>;

struct Node { int data; Node* next; Node(int v):data(v),next(nullptr){} };

// Minimal linked stack/queue (no size tracking for speed)
struct LStack {
    Node* head=nullptr;
    void  push(int v){ Node* n=new Node(v); n->next=head; head=n; }
    int   pop()      { int v=head->data; Node* t=head; head=head->next; delete t; return v; }
};

struct LQueue {
    Node* head=nullptr; Node* tail=nullptr;
    void enqueue(int v){ Node* n=new Node(v); if(!tail){head=tail=n;}
                         else{tail->next=n; tail=n;} }
    int  dequeue()     { int v=head->data; Node* t=head; head=head->next;
                         if(!head)tail=nullptr; delete t; return v; }
};

// Array stack using std::vector (dynamic — no overflow)
struct VStack {
    vector<int> data;
    void push(int v) { data.push_back(v); }
    int  pop()       { int v=data.back(); data.pop_back(); return v; }
};

// Circular array queue (fixed cap — large enough for test)
struct AQueue {
    static const int CAP=5000001;
    vector<int> data;
    int front=0, back=0, cnt=0;
    AQueue(): data(CAP) {}
    void enqueue(int v){ data[back]=v; back=(back+1)%CAP; cnt++; }
    int  dequeue()     { int v=data[front]; front=(front+1)%CAP; cnt--; return v; }
};

template<typename Fn>
double timeUs(Fn fn, int reps=3) {
    auto t0=Clock::now();
    for(int i=0;i<reps;i++) fn();
    return us(Clock::now()-t0).count()/reps;
}

int main() {
    vector<int> sizes = {100000, 500000, 1000000, 5000000};

    cout << "\n--- Push n items then pop all: Linked Stack vs Vector Stack (microseconds) ---\n\n";
    cout << setw(10) << "n"
         << setw(22) << "Linked push+pop"
         << setw(22) << "Vector push+pop"
         << setw(16) << "Ratio"
         << "\n" << string(70,'-') << "\n";

    for (int n : sizes) {
        double lt = timeUs([&]{
            LStack s;
            for(int i=0;i<n;i++) s.push(i);
            for(int i=0;i<n;i++) s.pop();
        });
        double vt = timeUs([&]{
            VStack s;
            s.data.reserve(n);
            for(int i=0;i<n;i++) s.push(i);
            for(int i=0;i<n;i++) s.pop();
        });
        cout << setw(10) << n
             << setw(22) << fixed << setprecision(0) << lt
             << setw(22) << vt
             << setw(15) << setprecision(2) << (lt/vt) << "x\n";
    }

    cout << "\n--- Enqueue n items then dequeue all: Linked Queue vs Array Queue (microseconds) ---\n\n";
    cout << setw(10) << "n"
         << setw(24) << "Linked enq+deq"
         << setw(24) << "Array enq+deq"
         << setw(16) << "Ratio"
         << "\n" << string(74,'-') << "\n";

    for (int n : sizes) {
        double lt = timeUs([&]{
            LQueue q;
            for(int i=0;i<n;i++) q.enqueue(i);
            for(int i=0;i<n;i++) q.dequeue();
        });
        double at = timeUs([&]{
            AQueue q;
            for(int i=0;i<n;i++) q.enqueue(i);
            for(int i=0;i<n;i++) q.dequeue();
        });
        cout << setw(10) << n
             << setw(24) << fixed << setprecision(0) << lt
             << setw(24) << at
             << setw(15) << setprecision(2) << (lt/at) << "x\n";
    }

    cout << "\n";
}