// O(m+n) - each edge and node gets visited and marked only once
#include <bits/stdc++.h>
using namespace std;


void dfs(int sourceNode, const vector<vector<int>> &adjacencyList, int *parentOf, int *distanceFromSource, bool *isVisited) {
    isVisited[sourceNode] = true;

    for (auto &adjacentNode: adjacencyList[sourceNode])
        if (not isVisited[adjacentNode]) {
            // cout << sourceNode + 1 << ' ' << adjacentNode + 1 << '\n';

            parentOf[adjacentNode] = sourceNode;
            distanceFromSource[adjacentNode] = distanceFromSource[sourceNode] + 1;
            dfs(adjacentNode, adjacencyList, parentOf, distanceFromSource, isVisited);
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
    bool isVisited[numberOfNodes];

    for (int i = 0; i < numberOfNodes; i++)
        isVisited[i] = false;

    dfs(sourceNode, adjacencyList, parentOf, distances,isVisited);

    // display edges in tree
    for (int node = 0; node < numberOfNodes; node++)
        if (node != sourceNode)
            cout << node + 1 << ' ' << parentOf[node] + 1;

    return 0;
}