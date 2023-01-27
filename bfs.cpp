// O(m+n) - each edge and node gets visited and marked only once
#include <bits/stdc++.h>

using namespace std;


void bfs(int sourceNode, int numberOfNodes, const vector<vector<int>> &adjacencyList, int *parentOf, int *distanceFromSource) {
    bool isVisited[numberOfNodes];
    for (int i = 0; i < numberOfNodes; i++) {
        isVisited[i] = false;
    }

    queue<int> q;

    parentOf[sourceNode] = -1;
    distanceFromSource[sourceNode] = 0;
    isVisited[sourceNode] = true;
    q.push(sourceNode);

    int currentNode;
    while (not q.empty()) {
        currentNode = q.front();
        q.pop();

        // checking every node that connects the current node to it through an edge
        for (auto &adjacentNode: adjacencyList[currentNode])
            if (not isVisited[adjacentNode]) {
                // cout << currentNode + 1 << ' ' << adjacentNode + 1 << '\n';

                parentOf[adjacentNode] = currentNode;
                distanceFromSource[adjacentNode] = distanceFromSource[currentNode] + 1;
                isVisited[adjacentNode] = true;
                q.push(adjacentNode);
            }
    }
}


int main() {
    ifstream input("graf.in");

    int numberOfNodes, numberOfEdges, sourceNode;
    input >> numberOfNodes >> numberOfEdges;

    vector<vector<int>> adjacencyList(numberOfNodes, vector<int>());
    int firstNode, secondNode;
    for (int i = 0; i < numberOfEdges; i++) {
        input >> firstNode >> secondNode;
        firstNode--;
        secondNode--;

        adjacencyList[firstNode].push_back(secondNode);
        adjacencyList[secondNode].push_back(firstNode);
    }

    input >> sourceNode;
    sourceNode--;

    input.close();

    int parentOf[numberOfNodes];
    int distances[numberOfNodes];

    bfs(sourceNode, numberOfNodes, adjacencyList, parentOf, distances);

    // display edges in tree
    for (int node = 0; node < numberOfNodes; node++)
        if (node != sourceNode)
            cout << node + 1 << ' ' << parentOf[node] + 1;

    return 0;
}