#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <climits>
#include <chrono>
#include <iomanip>
#include <numeric>
#include <random>
#include <algorithm>
#include <array>
#include <string>
using namespace std;
using Clock = chrono::high_resolution_clock;

const int INF = INT_MAX / 2;
using WGraph = vector<vector<pair<int,int>>>;  // adj list (neighbor, weight)
using EdgeList = vector<tuple<int,int,int>>;   // (u, v, w)

// -------- BFS --------
void bfs(const vector<vector<int>>& adj, int src) {
    int V = adj.size();
    vector<bool> vis(V, false);
    queue<int> q;
    vis[src] = true; q.push(src);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) if (!vis[v]) { vis[v]=true; q.push(v); }
    }
}

// -------- DFS (iterative) --------
void dfs(const vector<vector<int>>& adj, int src) {
    int V = adj.size();
    vector<bool> vis(V, false);
    stack<int> s;
    s.push(src);
    while (!s.empty()) {
        int u = s.top(); s.pop();
        if (vis[u]) continue;
        vis[u] = true;
        for (int v : adj[u]) if (!vis[v]) s.push(v);
    }
}

// -------- Dijkstra --------
void dijkstra(const WGraph& adj, int src) {
    int V = adj.size();
    vector<int> dist(V, INF);
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq;
    dist[src] = 0; pq.push({0, src});
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;
        for (auto [v, w] : adj[u])
            if (dist[u]+w < dist[v]) { dist[v]=dist[u]+w; pq.push({dist[v],v}); }
    }
}

// -------- Bellman-Ford --------
void bellmanFord(int V, const EdgeList& edges, int src) {
    vector<int> dist(V, INF); dist[src] = 0;
    for (int i = 0; i < V-1; i++)
        for (auto [u, v, w] : edges)
            if (dist[u] < INF && dist[u]+w < dist[v]) dist[v] = dist[u]+w;
}

// -------- Topological sort (Kahn's algorithm) --------
void topoSort(const vector<vector<int>>& adj) {
    int V = adj.size();
    vector<int> indeg(V, 0);
    for (int u = 0; u < V; u++) for (int v : adj[u]) indeg[v]++;
    queue<int> q;
    for (int u = 0; u < V; u++) if (indeg[u]==0) q.push(u);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) if (--indeg[v]==0) q.push(v);
    }
}

// -------- Prim's MST --------
void prim(const WGraph& adj, int src) {
    int V = adj.size();
    vector<int> key(V, INF);
    vector<bool> inMST(V, false);
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq;
    key[src] = 0; pq.push({0, src});
    while (!pq.empty()) {
        auto [k, u] = pq.top(); pq.pop();
        if (inMST[u]) continue;
        inMST[u] = true;
        for (auto [v, w] : adj[u])
            if (!inMST[v] && w < key[v]) { key[v]=w; pq.push({key[v],v}); }
    }
}

// -------- Floyd-Warshall --------
void floydWarshall(vector<vector<int>> dist) {
    int V = dist.size();
    for (int k = 0; k < V; k++)
        for (int i = 0; i < V; i++)
            for (int j = 0; j < V; j++)
                if (dist[i][k] < INF && dist[k][j] < INF)
                    dist[i][j] = min(dist[i][j], dist[i][k]+dist[k][j]);
}

// -------- Graph generators --------
struct TestGraph {
    int V, E;
    vector<vector<int>>    unweighted;
    WGraph                 weighted;
    EdgeList               edgeList;
    vector<vector<int>>    matrix;
};

TestGraph makeGraph(int V, int E, mt19937& rng) {
    TestGraph g;
    g.V = V; g.E = E;
    g.unweighted.assign(V, {});
    g.weighted.assign(V, {});
    g.matrix.assign(V, vector<int>(V, INF));
    for (int i = 0; i < V; i++) g.matrix[i][i] = 0;

    // Guarantee connectivity via a spanning chain
    for (int i = 0; i < V-1; i++) {
        int w = 1 + rng() % 20;
        g.unweighted[i].push_back(i+1);
        g.weighted[i].push_back({i+1, w});
        g.edgeList.push_back({i, i+1, w});
        g.matrix[i][i+1] = w;
    }
    // Fill remaining edges randomly
    uniform_int_distribution<int> vdist(0, V-1);
    int added = V-1;
    while (added < E) {
        int u = vdist(rng), v = vdist(rng);
        if (u == v) continue;
        int w = 1 + rng() % 20;
        g.unweighted[u].push_back(v);
        g.weighted[u].push_back({v, w});
        g.edgeList.push_back({u, v, w});
        if (g.matrix[u][v] == INF) g.matrix[u][v] = w;
        added++;
    }
    return g;
}

double bench(const string& algo, TestGraph& g) {
    auto t0 = Clock::now();
    if      (algo == "bfs")    bfs(g.unweighted, 0);
    else if (algo == "dfs")    dfs(g.unweighted, 0);
    else if (algo == "dijkstra") dijkstra(g.weighted, 0);
    else if (algo == "bellman")  bellmanFord(g.V, g.edgeList, 0);
    else if (algo == "topo")   topoSort(g.unweighted);
    else if (algo == "prim")   prim(g.weighted, 0);
    else if (algo == "floyd")  floydWarshall(g.matrix);
    return chrono::duration<double, milli>(Clock::now() - t0).count();
}

int main() {
    mt19937 rng(42);

    // (V, E) configs: sparse, medium, dense
    vector<tuple<int,int,string>> configs = {
        {500,  600,   "sparse (E≈V)"},
        {500,  5000,  "medium (E≈10V)"},
        {500,  50000, "dense  (E≈100V)"}
    };

    vector<string> algos = {
        "bfs","dfs","dijkstra","bellman","topo","prim","floyd"
    };
    vector<string> labels = {
        "BFS","DFS","Dijkstra","Bellman-Ford","Topo Sort","Prim MST","Floyd-Warshall"
    };

    for (auto& [V, E, desc] : configs) {
        TestGraph g = makeGraph(V, E, rng);
        cout << "=== " << desc << " (V=" << V << ", E=" << E << ") ===\n\n";
        cout << setw(18) << "Algorithm"
             << setw(14) << "Time (ms)"
             << "\n" << string(32, '-') << "\n";
        for (int i = 0; i < (int)algos.size(); i++) {
            // Floyd-Warshall only on small dense to keep runtime sane
            if (algos[i] == "floyd" && E > 10000) {
                cout << setw(18) << labels[i]
                     << setw(14) << "(skipped — O(V^3))" << "\n";
                continue;
            }
            double ms = bench(algos[i], g);
            cout << setw(18) << labels[i]
                 << setw(14) << fixed << setprecision(3) << ms << "\n";
        }
        cout << "\n";
    }

    // Complexity reference table
    cout << "=== Complexity Reference ===\n\n";
    cout << setw(18) << "Algorithm"
         << setw(18) << "Time"
         << setw(18) << "Space (extra)"
         << setw(16) << "Graph type"
         << setw(20) << "Handles neg. edges?"
         << "\n" << string(90, '-') << "\n";

    vector<array<string,5>> info = {
        {{"BFS",           "O(V+E)",       "O(V)",      "unweighted",   "N/A"}},
        {{"DFS",           "O(V+E)",       "O(V)",      "any",          "N/A"}},
        {{"Dijkstra",      "O((V+E)logV)", "O(V)",      "non-neg wts",  "No"}},
        {{"Bellman-Ford",  "O(VE)",        "O(V)",      "any weights",  "Yes"}},
        {{"Topo Sort",     "O(V+E)",       "O(V)",      "DAG only",     "N/A"}},
        {{"Prim MST",      "O((V+E)logV)", "O(V)",      "undirected",   "No"}},
        {{"Floyd-Warshall","O(V^3)",       "O(V^2)",    "any weights",  "Yes"}},
    };

    for (auto& row : info)
        cout << setw(18) << row[0] << setw(18) << row[1]
             << setw(18) << row[2] << setw(16) << row[3]
             << setw(20) << row[4] << "\n";
}
