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

    void addEdge(int v, int w) {
        adj[v].push_back(w);
    }

    void parallelDFS(int startVertex) {
        vector<bool> visited(V, false);

        #pragma omp parallel
        {
            #pragma omp single
            dfsUtil(startVertex, visited);
        }
    }

    void dfsUtil(int v, vector<bool>& visited) {
        #pragma omp critical
        {
            if (visited[v]) return;
            visited[v] = true;
            cout << v << " ";
        }

        for (int i = 0; i < adj[v].size(); ++i) {
            int n = adj[v][i];
            #pragma omp task
            dfsUtil(n, visited);
        }

        #pragma omp taskwait
    }

    void parallelBFS(int startVertex) {
        vector<bool> visited(V, false);
        queue<int> q;

        visited[startVertex] = true;
        q.push(startVertex);

        while (!q.empty()) {
            int v;

            #pragma omp critical
            {
                v = q.front();
                q.pop();
                cout << v << " ";
            }

            vector<int> neighbors = adj[v];

            #pragma omp parallel for
            for (int i = 0; i < neighbors.size(); ++i) {
                int n = neighbors[i];

                bool doVisit = false;
                #pragma omp critical
                {
                    if (!visited[n]) {
                        visited[n] = true;
                        q.push(n);
                        doVisit = true;
                    }
                }
            }
        }
    }
};

int main() {
    Graph g(7);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 5);
    g.addEdge(2, 6);

    cout << "Depth-First Search (DFS): ";
    g.parallelDFS(0);
    cout << "\n";

    cout << "Breadth-First Search (BFS): ";
    g.parallelBFS(0);
    cout << "\n";

    return 0;
}
