#include <iostream>
#include <stdexcept>
#include <string>
using namespace std;

// ── Linked-list queue ──────────────────────────────────────────────────
struct QNode { int data; QNode* next; QNode(int v):data(v),next(nullptr){} };

class LinkedQueue {
    QNode* head = nullptr;
    QNode* tail = nullptr;
    int    sz   = 0;
public:
    void enqueue(int v) {
        QNode* n = new QNode(v);
        if (!tail) { head=tail=n; }
        else       { tail->next=n; tail=n; }
        sz++;
    }
    int dequeue() {
        if (!head) throw underflow_error("empty");
        int v=head->data; QNode* t=head;
        head=head->next;
        if (!head) tail=nullptr;   // last element: reset tail too
        delete t; sz--;
        return v;
    }
    int  front() const { return head->data; }
    bool empty() const { return head==nullptr; }
    int  size()  const { return sz; }

    void print(const string& lbl) const {
        cout << lbl << "[front→] ";
        for (QNode* c=head; c; c=c->next) cout << c->data << " ";
        cout << "[←back]  (size=" << sz << ")\n";
    }
    ~LinkedQueue() { while(head){ QNode* t=head->next; delete head; head=t; } }
};

// ── Circular-array queue ───────────────────────────────────────────────
class ArrayQueue {
    static const int CAP = 8;
    int data[CAP];
    int front_idx = 0, back_idx = 0, cnt = 0;
public:
    void enqueue(int v) {
        if (cnt==CAP) throw overflow_error("full");
        data[back_idx] = v;
        back_idx = (back_idx+1) % CAP;
        cnt++;
    }
    int dequeue() {
        if (cnt==0) throw underflow_error("empty");
        int v = data[front_idx];
        front_idx = (front_idx+1) % CAP;
        cnt--;
        return v;
    }
    int  front() const { return data[front_idx]; }
    bool empty() const { return cnt==0; }
    int  size()  const { return cnt; }

    void print(const string& lbl) const {
        // Print raw array with front and back markers
        cout << lbl << "raw[";
        for (int i=0; i<CAP; i++) {
            if (cnt>0 && i==front_idx) cout << "F:";
            if (cnt>0 && i==(back_idx==0?CAP-1:back_idx-1)) cout << "B:";
            if (cnt==0) cout << ".";
            else {
                // Is this index logically in use?
                bool inUse = false;
                for (int j=0; j<cnt; j++)
                    if ((front_idx+j)%CAP == i) { inUse=true; break; }
                cout << (inUse ? to_string(data[i]) : ".");
            }
            if (i<CAP-1) cout << "|";
        }
        cout << "]  front=" << front_idx
             << " back=" << back_idx
             << " cnt=" << cnt << "\n";
    }
};

void doQOp(LinkedQueue& lq, ArrayQueue& aq, const string& op, int val=0) {
    cout << "\n── " << op;
    if (op=="enqueue") {
        cout << "(" << val << ")\n";
        lq.enqueue(val); aq.enqueue(val);
    } else if (op=="dequeue") {
        int lv=lq.dequeue(), av=aq.dequeue();
        cout << " → linked=" << lv << "  array=" << av << "\n";
    }
    lq.print("  Linked: ");
    aq.print("  Array:  ");
}

int main() {
    LinkedQueue lq; ArrayQueue aq;
    cout << "=== Queue Operation Trace ===\n";
    lq.print("  Linked: "); aq.print("  Array:  ");

    doQOp(lq, aq, "enqueue", 10);
    doQOp(lq, aq, "enqueue", 20);
    doQOp(lq, aq, "enqueue", 30);
    doQOp(lq, aq, "dequeue");
    doQOp(lq, aq, "enqueue", 40);
    doQOp(lq, aq, "dequeue");
    doQOp(lq, aq, "dequeue");
    doQOp(lq, aq, "enqueue", 50);
    doQOp(lq, aq, "enqueue", 60);
    doQOp(lq, aq, "enqueue", 70);
    doQOp(lq, aq, "dequeue");
    doQOp(lq, aq, "dequeue");
    doQOp(lq, aq, "dequeue");
    doQOp(lq, aq, "dequeue");

    cout << "\nFinal state:\n";
    lq.print("  Linked: "); aq.print("  Array:  ");
}
