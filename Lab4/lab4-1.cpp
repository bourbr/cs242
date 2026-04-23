#include <iostream>
#include <stdexcept>
#include <string>
using namespace std;

// ── Linked-list stack ──────────────────────────────────────────────────
struct Node { int data; Node* next; Node(int v):data(v),next(nullptr){} };

class LinkedStack {
    Node* head = nullptr;
    int   sz   = 0;
public:
    void push(int v) { Node* n=new Node(v); n->next=head; head=n; sz++; }
    int  pop()  {
        if (!head) throw underflow_error("empty");
        int v=head->data; Node* t=head; head=head->next; delete t; sz--;
        return v;
    }
    int  peek() const { if(!head) throw underflow_error("empty"); return head->data; }
    bool empty() const { return head==nullptr; }
    int  size()  const { return sz; }

    void print(const string& label) const {
        cout << label << " [top→] ";
        for (Node* c=head; c; c=c->next) cout << c->data << " ";
        cout << "(size=" << sz << ")\n";
    }
    ~LinkedStack() { while(head){ Node* t=head->next; delete head; head=t; } }
};

// ── Array-based stack ──────────────────────────────────────────────────
class ArrayStack {
    static const int CAP = 16;
    int  data[CAP];
    int  top = -1;
public:
    void push(int v) {
        if (top==CAP-1) throw overflow_error("full");
        data[++top]=v;
    }
    int  pop()  {
        if (top==-1) throw underflow_error("empty");
        return data[top--];
    }
    int  peek() const { return data[top]; }
    bool empty() const { return top==-1; }
    int  size()  const { return top+1; }

    void print(const string& label) const {
        cout << label << " [top→] ";
        for (int i=top; i>=0; i--) cout << data[i] << " ";
        cout << "(size=" << size() << ")\n";
    }
};

void doOp(LinkedStack& ls, ArrayStack& as, const string& op, int val=0) {
    cout << "\n── " << op;
    if (op=="push") { cout << "(" << val << ")\n"; ls.push(val); as.push(val); }
    else if (op=="pop") {
        int lv=ls.pop(), av=as.pop();
        cout << " → linked=" << lv << "  array=" << av << "\n";
    }
    else if (op=="peek") {
        cout << " → linked=" << ls.peek() << "  array=" << as.peek() << "\n";
    }
    ls.print("  Linked: ");
    as.print("  Array:  ");
}

int main() {
    LinkedStack ls;
    ArrayStack  as;

    cout << "=== Stack Operation Trace ===\n";
    ls.print("  Linked: "); as.print("  Array:  ");

    doOp(ls, as, "push", 10);
    doOp(ls, as, "push", 20);
    doOp(ls, as, "push", 30);
    doOp(ls, as, "peek");
    doOp(ls, as, "pop");
    doOp(ls, as, "push", 40);
    doOp(ls, as, "push", 50);
    doOp(ls, as, "pop");
    doOp(ls, as, "pop");
    doOp(ls, as, "pop");

    cout << "\nFinal state:\n";
    ls.print("  Linked: "); as.print("  Array:  ");
    cout << "  Both empty? " << (ls.empty() && as.empty() ? "yes" : "no") << "\n";
}
