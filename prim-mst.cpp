#include <bits/stdc++.h>
using namespace std;


int primMST(int numberOfNodes, vector<vector<pair<int, int>>> &adjacencyList, int& numberOfEdgesUsed, vector<int> &parentOf) {
    // stores currently smallest distance from a visited node to an unvisited one;
    // useful to filter out potentially useless queue insertions
    // and to keep track of the correct parent of each node
    vector<int> distanceTo(numberOfNodes, INT_MAX);
    vector<bool> isVisited(numberOfNodes, false);
    // min heap, in order to reduce the time complexity of the search for the next node
    priority_queue<pair<int, int>> q;
    // minimum cost of spanning tree
    int totalCost = 0;

    // pushing the first node
    distanceTo[0] = 0;
    q.push({0, 0});

    while (!q.empty()) {
        // some nodes might be pushed multiple times (they might be part
        // of multiple edges, but only one of them is of minimum cost)
        if (not isVisited[q.top().second]) {
            int u = q.top().second;
            totalCost -= q.top().first;
            numberOfEdgesUsed++;
            q.pop();
            isVisited[u] = true;

            // attempting to update its neighbours' distances
            for (auto neighbour: adjacencyList[u]) {
                int v = neighbour.first;
                int weight = neighbour.second;

                // don't push nodes in the queue if they have already been pushed with a smaller distance
                if (!isVisited[v] && weight < distanceTo[v]) {
                    distanceTo[v] = weight;
                    parentOf[v] = u;
                    q.push({-weight, v});
                }
            }
        }
        // pop visited node
        else
            q.pop();
    }

    return totalCost;
}


int main() {
    ifstream input("apm.in");

    int numberOfNodes, numberOfEdges;
    input>>numberOfNodes>>numberOfEdges;

    vector<vector<pair<int,int>>> adjacencyList(numberOfNodes, vector<pair<int,int>>());
    int firstNode, secondNode, cost;
    for (int i=0; i< numberOfEdges; i++) {
        input>>firstNode>>secondNode>>cost;
        firstNode--; secondNode--;
        adjacencyList[firstNode].emplace_back(secondNode, cost);
        adjacencyList[secondNode].emplace_back(firstNode, cost);
    }

    input.close();

    ofstream output("apm.out");
    int numberOfEdgesUsed = -1;
    vector<int> parentOf(numberOfNodes, -1);

    output<<primMST(numberOfNodes, adjacencyList, numberOfEdgesUsed, parentOf)<<'\n'<<numberOfEdgesUsed<<'\n';

    for (int i = 1; i < numberOfNodes; i++) {
        output << i+1 << " " << parentOf[i]+1 << "\n";
    }

    output.close();

    return 0;
}