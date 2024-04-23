#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>

using namespace std;

// Structure to represent a graph
struct Graph {
    int V; // Number of vertices
    vector<vector<int>> adjList; // Adjacency list
};

// Function to add an edge to the graph
void addEdge(Graph &graph, int u, int v) {
    graph.adjList[u].push_back(v);
    graph.adjList[v].push_back(u); // For undirected graph
}

// Parallel Breadth First Search
void parallelBFS(Graph &graph, int source,int x) {
    vector<bool> visited(graph.V, false);
    queue<int> q;
    q.push(source);
    visited[source] = true;
    int count=0;
    while (!q.empty()) {
        #pragma omp parallel for
        for (int i = 0; i < q.size(); ++i) {
            int u = q.front();
            if(u==x){
                cout<<"found ";
                count=1;
            }
              
            q.pop();

            // Traverse all adjacent vertices of u
            for (int v : graph.adjList[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    q.push(v);
                }
            }
        }
    }
    if(count==0){
                cout<<"not found ";
            }
}

// Parallel Depth First Search
void parallelDFSUtil(Graph &graph, int u, vector<bool> &visited) {
    visited[u] = true;

    // Traverse all adjacent vertices of u
    #pragma omp parallel for
    for (int v : graph.adjList[u]) {
        if (!visited[v]) {
            parallelDFSUtil(graph, v, visited);
        }
    }
}

void parallelDFS(Graph &graph, int source) {
    vector<bool> visited(graph.V, false);
    parallelDFSUtil(graph, source, visited);
}

int main() {
    Graph graph;
    graph.V = 6;
    graph.adjList.resize(graph.V);

    addEdge(graph, 0, 1);
    addEdge(graph, 0, 2);
    addEdge(graph, 1, 3);
    addEdge(graph, 2, 4);
    addEdge(graph, 4, 5);

    cout << "Parallel BFS starting from vertex 0:" << endl;
    parallelBFS(graph, 0,1);

    cout << "Parallel DFS starting from vertex 0:" << endl;
    parallelDFS(graph, 0);

    return 0;
}
