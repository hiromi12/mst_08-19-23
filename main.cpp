/**
 * 08-19-23
 * 
 * Given a weighted undirected connected graph with n vertices numbered from 0 to n - 1,
 * and an array edges where edges[i] = [ai, bi, weighti] represents a bidirectional and weighted edge between nodes ai and bi.
 * A minimum spanning tree (MST) is a subset of the graph's edges that connects all vertices without cycles and with the minimum possible total edge weight.
 * 
 * Find all the critical and pseudo-critical edges in the given graph's minimum spanning tree (MST).
 * An MST edge whose deletion from the graph would cause the MST weight to increase is called a critical edge.
 * On the other hand, a pseudo-critical edge is that which can appear in some MSTs but not all.
 * 
 * Note that you can return the indices of the edges in any order.
*/
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <climits>
using namespace std;

// Helper function to find parent of a node in disjoint-set
int findParent(vector<int>& parent, int node) {
    if (parent[node] != node) {
        parent[node] = findParent(parent, parent[node]);
    }
    return parent[node];
}

// Helper function to perform union of two sets
bool unionSets(vector<int>& parent, int node1, int node2) {
    int root1 = findParent(parent, node1);
    int root2 = findParent(parent, node2);
    if (root1 != root2) {
        parent[root1] = root2;
        return true;
    }
    return false;
}

// Helper function to calculate MST weight
int calculateMSTWeight(vector<vector<int>>& edges, vector<int>& selectedEdges) {
    int n = edges.size();
    vector<int> parent(n);
    iota(parent.begin(), parent.end(), 0);

    int weight = 0;
    for (int idx : selectedEdges) {
        if (unionSets(parent, edges[idx][0], edges[idx][1])) {
            weight += edges[idx][2];
        }
    }

    int root = findParent(parent, 0);
    for (int i = 1; i < n; ++i) {
        if (findParent(parent, i) != root) {
            return INT_MAX; // Disconnected graph
        }
    }
    return weight;
}

vector<vector<int>> findCriticalAndPseudoCriticalEdges(int n, vector<vector<int>>& edges) {
    vector<vector<int>> result(2); // For critical and pseudo-critical edges
    int m = edges.size();
    vector<int> indices(m);
    for (int i = 0; i < m; ++i) {
        indices[i] = i;
    }

    // Custom sort based on edge weights
    sort(indices.begin(), indices.end(), [&](int a, int b) {
        return edges[a][2] < edges[b][2];
    });

    // Helper function to find MST weight
    int mstWeight = calculateMSTWeight(edges, indices);

    for (int i = 0; i < m; ++i) {
        vector<int> selectedEdges;
        for (int j = 0; j < m; ++j) {
            if (i != j) {
                selectedEdges.push_back(indices[j]);
            }
        }
        if (calculateMSTWeight(edges, selectedEdges) > mstWeight) {
            result[0].push_back(indices[i]); // Critical edge
        } else {
            // Try including the edge in MST
            selectedEdges.push_back(indices[i]);
            if (calculateMSTWeight(edges, selectedEdges) == mstWeight) {
                result[1].push_back(indices[i]); // Pseudo-critical edge
            }
        }
    }
    return result;
}

int main() {
    int n = 5;
    vector<vector<int>> edges = {{0,1,1},{1,2,1},{2,3,2},{0,3,2},{0,4,3},{3,4,3},{1,4,6}};
    // int n = 4;
    // vector<vector<int>> edges = {{0,1,1},{1,2,1},{2,3,1},{0,3,1}};
    
    vector<vector<int>> output = findCriticalAndPseudoCriticalEdges(n, edges);

    cout << "Critical Edges: ";
    for (int edge : output[0]) {
        cout << edge << " ";
    }
    cout << endl;

    cout << "Pseudo-critical Edges: ";
    for (int edge : output[1]) {
        cout << edge << " ";
    }
    cout << endl;

    return 0;
}