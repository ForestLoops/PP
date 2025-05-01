#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>
using namespace std;

class Graph {
    int V;
    vector<vector<int>> adj;

public:
    Graph(int V) : V(V), adj(V) {}

    void addEdge(int u, int v) {
        if (u >= 0 && u < V && v >= 0 && v < V) {
            adj[u].push_back(v);
            adj[v].push_back(u);
        } else {
            cout << "Invalid edge!\n";
        }
    }

    void parallelBFS(int start) {
        if (start < 0 || start >= V) {
            cout << "Invalid start node!\n";
            return;
        }

        vector<bool> visited(V, false);
        queue<int> q;
        visited[start] = true;
        q.push(start);

        cout << "Parallel BFS Traversal: ";
        while (!q.empty()) {
            int size = q.size();
            vector<int> level, toAdd;

            for (int i = 0; i < size; ++i) {
                int node = q.front(); q.pop();
                cout << node << " ";
                level.push_back(node);
            }

            #pragma omp parallel for
            for (int i = 0; i < level.size(); ++i) {
                for (int neighbor : adj[level[i]]) {
                    if (!visited[neighbor]) {
                        #pragma omp critical
                        if (!visited[neighbor]) {
                            visited[neighbor] = true;
                            toAdd.push_back(neighbor);
                        }
                    }
                }
            }

            for (int node : toAdd) q.push(node);
        }
        cout << endl;
    }
};

int main() {
    int V, E, u, v, start;
    cout << "Enter vertices and edges: ";
    cin >> V >> E;

    Graph g(V);
    cout << "Enter " << E << " edges (u v):\n";
    while (E--) {
        cin >> u >> v;
        g.addEdge(u, v);
    }

    cout << "Enter start node: ";
    cin >> start;
    g.parallelBFS(start);
    return 0;
}