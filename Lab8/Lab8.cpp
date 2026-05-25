#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <climits>
#include <cmath>
#include <set>
#include <algorithm>
using namespace std;

// ── Timing helpers ────────────────────────────────────────────────────────────

using HRC = chrono::high_resolution_clock;
using TP  = HRC::time_point;
TP   snap()              { return HRC::now(); }
long us(TP a, TP b)      { return chrono::duration_cast<chrono::microseconds>(b-a).count(); }

// ── Integer BST ───────────────────────────────────────────────────────────────

struct BSTNode {
    int val;
    BSTNode *left = nullptr, *right = nullptr;
    BSTNode(int v) : val(v) {}
};

BSTNode* bst_insert(BSTNode* n, int key) {
    if (!n) return new BSTNode(key);
    if (key < n->val) n->left  = bst_insert(n->left,  key);
    else if (key > n->val) n->right = bst_insert(n->right, key);
    return n;
}

bool bst_search(BSTNode* n, int key) {
    if (!n) return false;
    if (key == n->val) return true;
    return key < n->val ? bst_search(n->left, key) : bst_search(n->right, key);
}

int bst_height(BSTNode* n) {
    if (!n) return -1;
    return 1 + max(bst_height(n->left), bst_height(n->right));
}

void bst_inorder(BSTNode* n, vector<int>& out) {
    if (!n) return;
    bst_inorder(n->left, out);
    out.push_back(n->val);
    bst_inorder(n->right, out);
}

BSTNode* bst_minNode(BSTNode* n) {
    while (n->left) n = n->left;
    return n;
}

BSTNode* bst_remove(BSTNode* n, int key) {
    if (!n) return nullptr;
    if      (key < n->val) n->left  = bst_remove(n->left,  key);
    else if (key > n->val) n->right = bst_remove(n->right, key);
    else {
        // Case 1: leaf
        if (!n->left && !n->right) { delete n; return nullptr; }
        // Case 2: one child
        if (!n->left)  { BSTNode* t = n->right; delete n; return t; }
        if (!n->right) { BSTNode* t = n->left;  delete n; return t; }
        // Case 3: two children — replace with in-order successor
        BSTNode* s = bst_minNode(n->right);
        n->val = s->val;
        n->right = bst_remove(n->right, s->val);
    }
    return n;
}

void bst_destroy(BSTNode* n) {
    if (!n) return;
    bst_destroy(n->left); bst_destroy(n->right); delete n;
}

// ── String BST ────────────────────────────────────────────────────────────────

struct StrNode {
    string val;
    StrNode *left = nullptr, *right = nullptr;
    StrNode(string v) : val(move(v)) {}
};

StrNode* str_insert(StrNode* n, const string& key) {
    if (!n) return new StrNode(key);
    if (key < n->val) n->left  = str_insert(n->left,  key);
    else if (key > n->val) n->right = str_insert(n->right, key);
    return n;
}

bool str_search(StrNode* n, const string& key) {
    if (!n) return false;
    if (key == n->val) return true;
    return key < n->val ? str_search(n->left, key) : str_search(n->right, key);
}

int str_height(StrNode* n) {
    if (!n) return -1;
    return 1 + max(str_height(n->left), str_height(n->right));
}

void str_destroy(StrNode* n) {
    if (!n) return;
    str_destroy(n->left); str_destroy(n->right); delete n;
}

// ── Trie ─────────────────────────────────────────────────────────────────────

struct TrieNode {
    TrieNode* ch[26] = {};
    bool isEnd = false;
};

void trie_insert(TrieNode* root, const string& word) {
    TrieNode* cur = root;
    for (char c : word) {
        if (!cur->ch[c-'a']) cur->ch[c-'a'] = new TrieNode();
        cur = cur->ch[c-'a'];
    }
    cur->isEnd = true;
}

bool trie_search(TrieNode* root, const string& word) {
    TrieNode* cur = root;
    for (char c : word) {
        if (!cur->ch[c-'a']) return false;
        cur = cur->ch[c-'a'];
    }
    return cur->isEnd;
}

bool trie_hasPrefix(TrieNode* root, const string& prefix) {
    TrieNode* cur = root;
    for (char c : prefix) {
        if (!cur->ch[c-'a']) return false;
        cur = cur->ch[c-'a'];
    }
    return true;
}

// ── Word generator ────────────────────────────────────────────────────────────

vector<string> makeWords(int n, unsigned seed = 42) {
    srand(seed);
    const string alpha = "abcdefghijklmnopqrstuvwxyz";
    set<string> seen;
    vector<string> words;
    words.reserve(n);
    while ((int)words.size() < n) {
        int len = 4 + rand() % 7;
        string w;
        for (int j = 0; j < len; j++) w += alpha[rand() % 26];
        if (seen.insert(w).second) words.push_back(w);
    }
    return words;
}

// ── Minimax: stone-taking game ────────────────────────────────────────────────
// Two players alternate. Each turn: take 1 or 2 stones.
// The player who takes the last stone wins.
// Returns +1 if current player wins with perfect play, -1 if they lose.

int minimax(int stones, bool isMax) {
    if (stones == 0) return isMax ? -1 : 1;
    int best = isMax ? INT_MIN : INT_MAX;
    for (int take : {1, 2}) {
        if (take > stones) continue;
        int score = minimax(stones - take, !isMax);
        best = isMax ? max(best, score) : min(best, score);
    }
    return best;
}

// ─────────────────────────────────────────────────────────────────────────────

int main() {

    // ── Part A: Insertion order and height ───────────────────────────────────
    cout << "=== Part A: BST Height by Insertion Order ===\n";

    vector<int> sorted_keys   = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    vector<int> shuffled_keys = {8,4,12,2,6,10,14,1,3,5,7,9,11,13,15};

    BSTNode* sorted_bst   = nullptr;
    BSTNode* shuffled_bst = nullptr;
    for (int k : sorted_keys)   sorted_bst   = bst_insert(sorted_bst,   k);
    for (int k : shuffled_keys) shuffled_bst = bst_insert(shuffled_bst, k);

    cout << "Sorted insertion   — height: " << bst_height(sorted_bst) << "\n";
    cout << "Shuffled insertion — height: " << bst_height(shuffled_bst) << "\n";

    vector<int> out;
    bst_inorder(shuffled_bst, out);
    cout << "In-order output:  ";
    for (int x : out) cout << x << " ";
    cout << "\n";

    bst_destroy(sorted_bst);
    bst_destroy(shuffled_bst);

    // ── Part B: Trie prefix search ───────────────────────────────────────────
    cout << "\n=== Part B: Trie Prefix Search ===\n";

    TrieNode* trie = new TrieNode();
    for (string w : {"apple","application","apply","apt",
                     "banana","band","bandana","bat",
                     "cat","card","care","car","carpet"})
        trie_insert(trie, w);

    for (string q : {"apple","app","car","carpet","bat","xyz","ap"}) {
        cout << "  \"" << q << "\"  search=" << trie_search(trie, q)
             << "  hasPrefix=" << trie_hasPrefix(trie, q) << "\n";
    }

    // ── Part C: BST removal — all three cases ────────────────────────────────
    cout << "\n=== Part C: BST Removal ===\n";

    BSTNode* rt = nullptr;
    for (int v : {50,30,70,20,40,60,80,35}) rt = bst_insert(rt, v);

    auto printTree = [&](const string& label) {
        out.clear(); bst_inorder(rt, out);
        cout << "  " << label;
        for (int x : out) cout << x << " ";
        cout << "  (height=" << bst_height(rt) << ")\n";
    };

    printTree("Initial:               ");
    rt = bst_remove(rt, 20);  printTree("After remove(20) [leaf]:       ");
    rt = bst_remove(rt, 30);  printTree("After remove(30) [one child]:  ");
    rt = bst_remove(rt, 50);  printTree("After remove(50) [two children]:");

    bst_destroy(rt);

    // ── Part D: Empirical comparison — Trie vs. string BST ───────────────────
    cout << "\n=== Part D: Trie vs. BST — 50,000 words ===\n";

    const int N    = 50000;
    const int REPS = 10;
    auto words = makeWords(N);

    TrieNode* big_trie = new TrieNode();
    StrNode*  str_root = nullptr;
    for (auto& w : words) {
        trie_insert(big_trie, w);
        str_root = str_insert(str_root, w);
    }

    cout << "BST height:    " << str_height(str_root) << "\n";
    cout << "log2(" << N << ") ≈ " << (int)log2(N)
         << "  (balanced ideal)\n\n";

    volatile bool sink = false;

    auto t1 = snap();
    for (int r = 0; r < REPS; r++)
        for (auto& w : words) sink = trie_search(big_trie, w);
    auto t2 = snap();
    for (int r = 0; r < REPS; r++)
        for (auto& w : words) sink = str_search(str_root, w);
    auto t3 = snap();

    cout << "Exact search (" << N*REPS << " lookups):\n";
    cout << "  Trie: " << us(t1,t2) << " μs\n";
    cout << "  BST:  " << us(t2,t3) << " μs\n\n";

    str_destroy(str_root);

    return 0;
}
