#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

const int TABLE_SIZE = 11;   // prime

int hashFn(const string& s) {
    int h = 0;
    for (char c : s)
        h = h * 31 + c;
    return abs(h) % TABLE_SIZE;
}

int rawHash(const string& s) {
    int h = 0;
    for (char c : s)
        h = h * 31 + c;
    return abs(h);
}

int main() {
    vector<string> keys = {
        "alice", "bob", "carol", "dave", "eve",
        "frank", "grace", "heidi", "ivan", "judy"
    };

    cout << "TABLE_SIZE = " << TABLE_SIZE << "\n\n";
    cout << left
         << setw(10) << "Key"
         << setw(14) << "Raw hash"
         << setw(10) << "Slot"
         << "\n" << string(34, '-') << "\n";

    // Count collisions
    vector<int> slotCount(TABLE_SIZE, 0);
    for (const string& k : keys) {
        int raw  = rawHash(k);
        int slot = hashFn(k);
        slotCount[slot]++;
        cout << setw(10) << k
             << setw(14) << raw
             << setw(10) << slot << "\n";
    }

    cout << "\nSlot occupancy after all insertions:\n";
    cout << setw(8) << "Slot" << setw(8) << "Count" << "\n";
    cout << string(16, '-') << "\n";
    int collisions = 0;
    for (int i = 0; i < TABLE_SIZE; i++) {
        cout << setw(8) << i << setw(8) << slotCount[i];
        if (slotCount[i] > 1) { cout << "  ← collision"; collisions++; }
        cout << "\n";
    }
    cout << "\nSlots with collisions: " << collisions << "\n";
    cout << "Empty slots:           ";
    int empty = 0;
    for (int c : slotCount) if (c == 0) empty++;
    cout << empty << "\n";
    cout << "Load factor:           "
         << fixed << setprecision(2)
         << (double)keys.size() / TABLE_SIZE << "\n";
}
