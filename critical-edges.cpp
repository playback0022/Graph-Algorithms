// O(m+n) - DFS traversal
#include <bits/stdc++.h>
using namespace std;


void detectCriticalEdges(int currentNode, const vector<vector<int>> &adjacencyList, bool *isVisited, int *level, int *minimumLevel, int *parentOf) {
    isVisited[currentNode] = true;

    for (auto &adjacentNode: adjacencyList[currentNode]) {
        if (not isVisited[adjacentNode]) {
            parentOf[adjacentNode] = currentNode;
            level[adjacentNode] = level[currentNode] + 1;
            minimumLevel[adjacentNode] = level[currentNode] + 1;

            detectCriticalEdges(adjacentNode, adjacencyList, isVisited, level, minimumLevel, parentOf);

            // check if the subtree rooted with v is
            // connected to one of the ancestors of u
            if (minimumLevel[adjacentNode] < minimumLevel[currentNode])
                minimumLevel[currentNode] = minimumLevel[adjacentNode];

            // if the lowest node reachable from subtree
            // under v is below u in DFS tree, then u-v
            // is critical
            if (minimumLevel[adjacentNode] > level[currentNode])
                cout << currentNode + 1 << " " << adjacentNode + 1 << '\n';
        }
        // Update low value of u for parent function calls
        else if (adjacentNode != parentOf[currentNode]) {
            if (level[adjacentNode] < minimumLevel[currentNode])
                minimumLevel[currentNode] = level[adjacentNode];
        }
    }
}


int main() {
    ifstream input("graf.in");

    int numberOfNodes, numberOfEdges;
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

    input.close();

    bool isVisited[numberOfNodes];
    int level[numberOfNodes], minimumLevel[numberOfNodes], parentOf[numberOfNodes];

    // initialize parent and visited arrays
    for (int i = 0; i < numberOfNodes; i++) {
        parentOf[i] = -1;
        isVisited[i] = false;
    }

    // traverse all DFS trees
    for (int node = 0; node < numberOfNodes; node++)
        if (not isVisited[node]) {
            level[node] = 0;
            minimumLevel[node] = 0;

            detectCriticalEdges(node, adjacencyList, isVisited, level, minimumLevel, parentOf);
        }

//    // display edges in tree
//    for (int node = 0; node < numberOfNodes; node++)
//        if (parentOf[node] != -1)
//            cout << node + 1 << ' ' << parentOf[node] + 1;

    return 0;
}
