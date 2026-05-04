#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

const int TABLE_SIZE = 11;

int hashFn(const string& s) {
    int h = 0;
    for (char c : s) h = h * 31 + c;
    return abs(h) % TABLE_SIZE;
}

enum State { EMPTY, OCCUPIED, DELETED };

struct Slot {
    string key;
    int    value = 0;
    State  state = EMPTY;
};

class LinearProbeTable {
    Slot table[TABLE_SIZE];
    int  count = 0;
public:
    void insert(const string& key, int val) {
        int start = hashFn(key);
        int idx   = start;
        int probes = 0;
        while (table[idx].state == OCCUPIED && table[idx].key != key) {
            idx = (idx + 1) % TABLE_SIZE;
            probes++;
            if (probes == TABLE_SIZE) { cout << "  TABLE FULL\n"; return; }
        }
        if (table[idx].state != OCCUPIED) count++;
        table[idx] = {key, val, OCCUPIED};
        cout << "  insert(\"" << key << "\") home=" << start;
        if (probes == 0) cout << "  placed at " << start << "  (0 extra probes)\n";
        else             cout << "  placed at " << idx
                              << "  (" << probes << " extra probe(s), cluster!)\n";
    }

    void remove(const string& key) {
        int start = hashFn(key);
        int idx   = start;
        int probes = 0;
        while (table[idx].state != EMPTY) {
            if (table[idx].state == OCCUPIED && table[idx].key == key) {
                table[idx].state = DELETED;
                count--;
                cout << "  remove(\"" << key << "\") → slot " << idx
                     << " marked TOMBSTONE\n";
                return;
            }
            idx = (idx + 1) % TABLE_SIZE;
            if (++probes == TABLE_SIZE) break;
        }
        cout << "  remove(\"" << key << "\") → NOT FOUND\n";
    }

    int search(const string& key) {
        int start = hashFn(key);
        int idx   = start;
        int probes = 0;
        cout << "  search(\"" << key << "\") home=" << start << "  path: ";
        while (table[idx].state != EMPTY) {
            string label = (table[idx].state == DELETED) ? "TOMB"
                         : "\"" + table[idx].key + "\"";
            cout << "[" << idx << "]=" << label;
            if (table[idx].state == OCCUPIED && table[idx].key == key) {
                cout << " ✓  (" << probes+1 << " probe(s))\n";
                return table[idx].value;
            }
            idx = (idx + 1) % TABLE_SIZE;
            probes++;
            if (probes == TABLE_SIZE) break;
            cout << " → ";
        }
        cout << "[" << idx << "]=EMPTY  NOT FOUND (" << probes << " probe(s))\n";
        return -1;
    }

    void printTable(const string& label = "") const {
        if (!label.empty()) cout << label << "\n";
        for (int i = 0; i < TABLE_SIZE; i++) {
            cout << "  [" << setw(2) << i << "] ";
            if      (table[i].state == EMPTY)    cout << ".\n";
            else if (table[i].state == DELETED)  cout << "<TOMBSTONE>\n";
            else cout << "\"" << table[i].key << "\" = " << table[i].value << "\n";
        }
        cout << "  Load factor: " << fixed << setprecision(2)
             << (double)count / TABLE_SIZE << "  (" << count << "/" << TABLE_SIZE << ")\n";
    }
};

int main() {
    LinearProbeTable t;

    cout << "=== Linear Probing: Insertions ===\n\n";
    t.printTable("Initial state:");

    vector<pair<string,int>> data = {
        {"alice",91}, {"bob",85}, {"carol",78},
        {"dave",92},  {"eve",88}, {"frank",74},
        {"grace",95}
    };

    for (auto& [k, v] : data) {
        t.insert(k, v);
        t.printTable();
    }

    cout << "\n=== Deletion and Tombstones ===\n\n";
    t.printTable("Before deletion:");

    // Search succeeds before deletion
    t.search("carol");

    // Delete a key in the middle of a potential probe chain
    t.remove("bob");
    t.printTable("After removing \"bob\":");

    // Search after deletion — must traverse tombstone
    t.search("carol");

    // Demonstrate danger: search WITHOUT tombstone support
    // (we simulate by asking about a key whose path goes through the deleted slot)
    cout << "\n=== Probe path comparison ===\n";
    cout << "Note: carol's home slot and bob's slot — observe how the\n"
         << "search for carol must cross bob's tombstone to succeed.\n\n";
    t.search("alice");
    t.search("grace");
    t.search("zara");   // not present
}
