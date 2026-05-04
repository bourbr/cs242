#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <iomanip>
#include <functional>
using namespace std;

// Prime table sizes for each load factor target
const vector<int> TABLE_SIZES = {101, 67, 43, 29, 17};
// Corresponding approximate load factors when filled with 50% of size keys

int h1(const string& s, int sz) {
    int h = 0; for (char c : s) h = h * 31 + c; return abs(h) % sz;
}
int h2(const string& s, int sz) {
    int h = 0; for (char c : s) h = h * 37 + c;
    return 1 + (abs(h) % (sz - 1));
}

// ---------- Chaining ----------
struct CEntry { string key; CEntry* next; };

double chainAvgProbes(const vector<string>& keys, int sz) {
    vector<CEntry*> table(sz, nullptr);
    for (auto& k : keys) {
        int idx = h1(k, sz);
        table[idx] = new CEntry{k, table[idx]};
    }
    long long total = 0;
    for (auto& k : keys) {
        int idx = h1(k, sz); int p = 0;
        for (CEntry* e = table[idx]; e; e = e->next) {
            p++;
            if (e->key == k) break;
        }
        total += p;
    }
    for (int i = 0; i < sz; i++)
        for (CEntry* e = table[i]; e; ) { CEntry* t = e->next; delete e; e = t; }
    return (double)total / keys.size();
}

// ---------- Linear Probing ----------
double linearAvgProbes(const vector<string>& keys, int sz) {
    vector<string> table(sz, "");
    vector<bool>   used(sz, false);
    for (auto& k : keys) {
        int idx = h1(k, sz);
        while (used[idx]) idx = (idx + 1) % sz;
        table[idx] = k; used[idx] = true;
    }
    long long total = 0;
    for (auto& k : keys) {
        int idx = h1(k, sz); int p = 0;
        while (used[idx] && table[idx] != k) { idx = (idx + 1) % sz; p++; }
        total += p + 1;
    }
    return (double)total / keys.size();
}

// ---------- Double Hashing ----------
double doubleAvgProbes(const vector<string>& keys, int sz) {
    vector<string> table(sz, "");
    vector<bool>   used(sz, false);
    for (auto& k : keys) {
        int idx  = h1(k, sz);
        int step = h2(k, sz);
        while (used[idx]) idx = (idx + step) % sz;
        table[idx] = k; used[idx] = true;
    }
    long long total = 0;
    for (auto& k : keys) {
        int idx  = h1(k, sz);
        int step = h2(k, sz);
        int p = 0;
        while (used[idx] && table[idx] != k) { idx = (idx + step) % sz; p++; }
        total += p + 1;
    }
    return (double)total / keys.size();
}

// Generate n unique synthetic keys
vector<string> makeKeys(int n) {
    vector<string> v;
    for (int i = 0; i < n; i++) {
        string s = "";
        int x = i;
        do { s += (char)('a' + x % 26); x /= 26; } while (x > 0);
        v.push_back(s);
    }
    return v;
}

int main() {
    // Test across several load factors using varying n/sz ratios
    vector<pair<int,int>> tests = {
        {10,  101},   // λ ≈ 0.10
        {25,  101},   // λ ≈ 0.25
        {50,  101},   // λ ≈ 0.50
        {70,  101},   // λ ≈ 0.69
        {90,  101},   // λ ≈ 0.89
    };

    cout << "=== Average Successful Probe Count by Strategy and Load Factor ===\n\n";
    cout << fixed << setprecision(3);
    cout << setw(12) << "Load (λ)"
         << setw(16) << "Chaining"
         << setw(16) << "Linear probe"
         << setw(16) << "Double hash"
         << "\n" << string(60, '-') << "\n";

    for (auto& [n, sz] : tests) {
        auto keys = makeKeys(n);
        double lam   = (double)n / sz;
        double chain = chainAvgProbes(keys, sz);
        double lin   = linearAvgProbes(keys, sz);
        double dbl   = doubleAvgProbes(keys, sz);
        cout << setw(12) << lam
             << setw(16) << chain
             << setw(16) << lin
             << setw(16) << dbl << "\n";
    }

    // Theoretical values for reference
    cout << "\n=== Theoretical Predictions (for comparison) ===\n\n";
    cout << setw(12) << "Load (λ)"
         << setw(20) << "Linear (theory)"
         << setw(20) << "Double (theory)"
         << "\n" << string(52, '-') << "\n";

    vector<double> lambdas = {0.10, 0.25, 0.50, 0.69, 0.89};
    for (double lam : lambdas) {
        double linTheory = 0.5 * (1.0 + 1.0 / (1.0 - lam));
        double dblTheory = (1.0 / lam) * log(1.0 / (1.0 - lam));
        cout << setw(12) << lam
             << setw(20) << linTheory
             << setw(20) << dblTheory << "\n";
    }
}

