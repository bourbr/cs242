#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

// -------- Adjacency list --------
struct AdjList {
    int V;
    vector<vector<pair<int,int>>> adj;  // adj[u] = {(v, weight), ...}
    AdjList(int v) : V(v), adj(v) {}
    void addEdge(int u, int v, int w) {
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});   // undirected
    }
    void print() const {
        cout << "Adjacency List:\n";
        for (int u = 0; u < V; u++) {
            cout << "  " << u << ": ";
            for (auto [v, w] : adj[u]) cout << "(" << v << ",w=" << w << ") ";
            cout << "\n";
        }
    }
    // Each pair<int,int> = 8 bytes; vector metadata = 24 bytes per row
    size_t memBytes() const {
        size_t total = adj.size() * 24;          // vector headers
        for (auto& row : adj) total += row.size() * sizeof(pair<int,int>);
        return total;
    }
};

// -------- Adjacency matrix --------
struct AdjMatrix {
    int V;
    vector<vector<int>> mat;   // mat[u][v] = weight, 0 = no edge
    AdjMatrix(int v) : V(v), mat(v, vector<int>(v, 0)) {}
    void addEdge(int u, int v, int w) {
        mat[u][v] = w;
        mat[v][u] = w;   // undirected
    }
    void print() const {
        cout << "Adjacency Matrix:\n    ";
        for (int i = 0; i < V; i++) cout << setw(4) << i;
        cout << "\n    " << string(4*V, '-') << "\n";
        for (int u = 0; u < V; u++) {
            cout << setw(2) << u << " |";
            for (int v = 0; v < V; v++) cout << setw(4) << mat[u][v];
            cout << "\n";
        }
    }
    size_t memBytes() const { return (size_t)V * V * sizeof(int); }
};

// Build an identical graph in both representations
void buildGraph(AdjList& al, AdjMatrix& am,
                const vector<tuple<int,int,int>>& edges) {
    for (auto [u, v, w] : edges) {
        al.addEdge(u, v, w);
        am.addEdge(u, v, w);
    }
}

int main() {
    // Small example graph (6 vertices, 7 edges)
    int V = 6;
    vector<tuple<int,int,int>> edges = {
        {0,1,4}, {0,2,2}, {1,2,5}, {1,3,10},
        {2,4,3}, {3,5,7}, {4,5,1}
    };

    AdjList  al(V);
    AdjMatrix am(V);
    buildGraph(al, am, edges);

    al.print(); cout << "\n";
    am.print(); cout << "\n";

    // Memory comparison across graph sizes
    cout << "=== Memory Usage: Adjacency List vs Matrix ===\n\n";
    cout << setw(8)  << "V"
         << setw(10) << "E"
         << setw(22) << "List (bytes)"
         << setw(22) << "Matrix (bytes)"
         << setw(12) << "Ratio"
         << "\n" << string(74, '-') << "\n";

    // (V, E) pairs: sparse, medium, dense
    vector<pair<int,int>> configs = {
        {10,  12}, {10,  45},            // sparse vs dense, small
        {100, 150}, {100, 4950},         // sparse vs dense, medium
        {500, 600}, {500, 124750}        // sparse vs dense, large
    };

    for (auto [v, e] : configs) {
        AdjList  testAL(v);
        AdjMatrix testAM(v);
        // Distribute e edges evenly around a ring + random chords
        for (int i = 0; i < v; i++)
            testAL.addEdge(i, (i+1)%v, 1);
        // Fill up to e edges with stride-2 chords
        for (int added = v, s = 2; added < e && s < v; s++) {
            for (int i = 0; i < v && added < e; i++, added++)
                testAL.addEdge(i, (i+s)%v, 1);
        }
        // Mirror into matrix just for memory accounting
        for (int i = 0; i < v; i++)
            for (auto [nb, w] : testAL.adj[i])
                if (nb > i) testAM.addEdge(i, nb, w);

        size_t lm = testAL.memBytes();
        size_t mm = testAM.memBytes();
        cout << setw(8)  << v
             << setw(10) << e
             << setw(22) << lm
             << setw(22) << mm
             << setw(12) << fixed << setprecision(1)
             << (double)mm / lm << "x\n";
    }

    cout << "\nEdge lookup cost:\n";
    cout << "  Adjacency list:   O(degree(u))  — scan u's neighbor list\n";
    cout << "  Adjacency matrix: O(1)          — direct index mat[u][v]\n";
}
