#include <iostream>
#include <vector>
#include <limits>
#include <omp.h>
using namespace std;

#define INF numeric_limits<int>::max()

int minDistance(const vector<int>& dist, const vector<bool>& sptSet, int V) {
    int min = INF, idx = -1;
    #pragma omp parallel for
    for (int v = 0; v < V; ++v) {
        if (!sptSet[v]) {
            #pragma omp critical
            if (dist[v] < min) {
                min = dist[v];
                idx = v;
            }
        }
    }
    return idx;
}

void dijkstra(const vector<vector<int>>& graph, int src, int V) {
    vector<int> dist(V, INF);
    vector<bool> sptSet(V, false);
    dist[src] = 0;

    for (int i = 0; i < V - 1; ++i) {
        int u = minDistance(dist, sptSet, V);
        if (u == -1) break;
        sptSet[u] = true;

        #pragma omp parallel for
        for (int v = 0; v < V; ++v) {
            if (!sptSet[v] && graph[u][v] && dist[u] != INF &&
                dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }

    cout << "\nVertex \t Distance from Source\n";
    for (int i = 0; i < V; ++i)
        cout << i << " \t " << (dist[i] == INF ? -1 : dist[i]) << "\n";
}

int main() {
    int V, E, u, v, w, src;
    cout << "Enter vertices and edges: ";
    cin >> V >> E;

    vector<vector<int>> graph(V, vector<int>(V, 0));
    cout << "Enter " << E << " edges (u v w):\n";
    while (E--) {
        cin >> u >> v >> w;
        graph[u][v] = graph[v][u] = w;
    }

    cout << "Enter source vertex: ";
    cin >> src;

    dijkstra(graph, src, V);
    return 0;
}