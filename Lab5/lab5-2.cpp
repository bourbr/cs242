#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

const int TABLE_SIZE = 7;

int hashFn(const string& s) {
    int h = 0;
    for (char c : s) h = h * 31 + c;
    return abs(h) % TABLE_SIZE;
}

struct Entry {
    string key; int value; Entry* next;
    Entry(const string& k, int v) : key(k), value(v), next(nullptr) {}
};

class ChainingTable {
    Entry* table[TABLE_SIZE];
    int    count = 0;
public:
    ChainingTable() { fill(table, table + TABLE_SIZE, nullptr); }

    void insert(const string& key, int val) {
        int idx = hashFn(key);
        // Check for existing key (update)
        for (Entry* e = table[idx]; e; e = e->next) {
            if (e->key == key) { e->value = val; return; }
        }
        Entry* n = new Entry(key, val);
        n->next = table[idx];
        table[idx] = n;
        count++;
    }

    int search(const string& key) {
        int idx = hashFn(key);
        int probes = 0;
        cout << "  search(\"" << key << "\") → slot " << idx << " → probes: ";
        for (Entry* e = table[idx]; e; e = e->next) {
            probes++;
            cout << "\"" << e->key << "\"";
            if (e->key == key) {
                cout << " ✓  (" << probes << " probe(s))\n";
                return e->value;
            }
            cout << " → ";
        }
        cout << "null  NOT FOUND (" << probes << " probe(s))\n";
        return -1;
    }

    void printTable(const string& label = "") const {
        if (!label.empty()) cout << label << "\n";
        for (int i = 0; i < TABLE_SIZE; i++) {
            cout << "  [" << i << "]: ";
            for (Entry* e = table[i]; e; e = e->next)
                cout << "(\"" << e->key << "\"," << e->value << ") → ";
            cout << "null\n";
        }
        cout << "  Load factor: " << fixed << setprecision(2)
             << (double)count / TABLE_SIZE << "  (" << count << "/" << TABLE_SIZE << ")\n";
    }

    ~ChainingTable() {
        for (int i = 0; i < TABLE_SIZE; i++)
            for (Entry* e = table[i]; e; ) {
                Entry* t = e->next; delete e; e = t;
            }
    }
};

int main() {
    ChainingTable t;

    vector<pair<string,int>> data = {
        {"alice",91}, {"bob",85}, {"carol",78},
        {"dave",92},  {"eve",88}, {"frank",74},
        {"grace",95}, {"heidi",81}
    };

    cout << "=== Chaining Hash Table: Insertions ===\n\n";
    t.printTable("Initial state:");

    for (auto& [k, v] : data) {
        cout << "\ninsert(\"" << k << "\", " << v
             << ")  →  slot " << hashFn(k) << "\n";
        t.insert(k, v);
        t.printTable();
    }

    cout << "\n=== Lookups ===\n";
    t.search("alice");
    t.search("carol");
    t.search("grace");
    t.search("zara");   // not present
}
