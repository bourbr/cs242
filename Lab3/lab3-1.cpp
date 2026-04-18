#include <iostream>
#include <iomanip>
using namespace std;

struct Node {
    int   data;
    Node* next;
    Node(int v) : data(v), next(nullptr) {}
};

void printList(Node* head, const string& label) {
    cout << label << ": ";
    for (Node* c = head; c; c = c->next)
        cout << "[" << c->data << "@" << c << "]";
    if (head) cout << " → null";
    cout << "\n";
}

void printAddresses(Node* head) {
    cout << "  Addresses: ";
    for (Node* c = head; c; c = c->next)
        cout << c << "(→" << c->next << ") ";
    cout << "\n";
}

// Insert newNode after prev. Pass nullptr for prev to insert at front.
void insertAfter(Node*& head, Node* prev, int val) {
    Node* n = new Node(val);
    cout << "\n  → Inserting " << val;
    if (!prev) {
        cout << " at FRONT\n";
        cout << "    Before: head=" << head << "\n";
        n->next = head;
        head = n;
        cout << "    After:  head=" << head
             << "  new->next=" << n->next << "\n";
    } else {
        cout << " after node [" << prev->data << "@" << prev << "]\n";
        cout << "    Before: prev->next=" << prev->next << "\n";
        n->next = prev->next;   // step 1
        prev->next = n;         // step 2
        cout << "    After:  new->next=" << n->next
             << "  prev->next=" << prev->next << "\n";
    }
}

void removeAfter(Node*& head, Node* prev) {
    if (!prev) {
        // Remove head
        Node* gone = head;
        cout << "\n  → Removing HEAD node [" << head->data
             << "@" << head << "]\n";
        cout << "    Before: head=" << head << "\n";
        head = head->next;
        delete gone;
        cout << "    After:  head=" << head << "\n";
    } else {
        Node* gone = prev->next;
        cout << "\n  → Removing [" << gone->data << "@" << gone
             << "] (after [" << prev->data << "])\n";
        cout << "    Before: prev->next=" << prev->next << "\n";
        prev->next = gone->next;
        delete gone;
        cout << "    After:  prev->next=" << prev->next << "\n";
    }
}

int main() {
    Node* head = nullptr;

    cout << "=== Building a singly-linked list ===\n";
    printList(head, "Initial");

    // Insert 30 at front
    insertAfter(head, nullptr, 30);
    printList(head, "After insert 30 at front");

    // Insert 10 at front
    insertAfter(head, nullptr, 10);
    printList(head, "After insert 10 at front");

    // Insert 20 after 10 (head)
    insertAfter(head, head, 20);
    printList(head, "After insert 20 after 10");

    // Insert 40 after 30 (last node)
    Node* node30 = head->next->next;
    insertAfter(head, node30, 40);
    printList(head, "After insert 40 after 30");
    printAddresses(head);

    cout << "\n=== Removing nodes ===\n";
    printList(head, "Before removes");

    // Remove 20 (second node, after head)
    removeAfter(head, head);
    printList(head, "After removing 20 (after head)");

    // Remove head (10)
    removeAfter(head, nullptr);
    printList(head, "After removing head (10)");

    printAddresses(head);

    // Clean up
    while (head) { Node* t = head->next; delete head; head = t; }
    cout << "\n(All nodes freed.)\n";
}
