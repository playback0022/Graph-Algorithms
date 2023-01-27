// C++ program to find articulation points in an undirected graph
#include <bits/stdc++.h>
using namespace std;


void detectCriticalNodes(int currentNode, const vector<vector<int>> &adjacencyList, bool *isVisited, int *level, int *minimumLevel, int *parentOf, bool *isCritical) {
    int numberOfChildren = 0;   // used to verify root has at least 2 children

    isVisited[currentNode] = true;

    for (auto &adjacentNode: adjacencyList[currentNode]) {
        if (not isVisited[adjacentNode]) {
            numberOfChildren++;
            parentOf[adjacentNode] = currentNode;
            level[adjacentNode] = level[currentNode] + 1;
            minimumLevel[adjacentNode] = level[currentNode] + 1;

            detectCriticalNodes(adjacentNode, adjacencyList, isVisited, level, minimumLevel, parentOf, isCritical);

            // check if the subtree rooted with v is
            // connected to one of the ancestors of u
            if (minimumLevel[adjacentNode] < minimumLevel[currentNode])
                minimumLevel[currentNode] = minimumLevel[adjacentNode];

            // if the lowest node reachable from subtree
            // under v is below or at u in DFS tree, then
            // u is critical
            if (parentOf[currentNode] != -1 and minimumLevel[adjacentNode] >= level[currentNode])
                isCritical[currentNode] = true;
        }
            // Update low value of u for parent function calls
        else if (adjacentNode != parentOf[currentNode]) {
            if (level[adjacentNode] < minimumLevel[currentNode])
                minimumLevel[currentNode] = level[adjacentNode];
        }
    }

    if (parentOf[currentNode] == -1 and numberOfChildren > 1)
        isCritical[currentNode] = true;
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

    bool isVisited[numberOfNodes], isCritical[numberOfNodes];
    int level[numberOfNodes], minimumLevel[numberOfNodes], parentOf[numberOfNodes];

    // initialize parent and visited arrays
    for (int i = 0; i < numberOfNodes; i++) {
        parentOf[i] = -1;
        isVisited[i] = false;
        isCritical[i] = false;
    }

    // traverse all DFS trees
    for (int node = 0; node < numberOfNodes; node++)
        if (not isVisited[node]) {
            level[node] = 0;
            minimumLevel[node] = 0;

            detectCriticalNodes(node, adjacencyList, isVisited, level, minimumLevel, parentOf, isCritical);
        }

    for (int node = 0; node < numberOfNodes; node ++)
        if (isCritical[node])
            cout << node + 1 << ' ';

//    // display edges in tree
//    for (int node = 0; node < numberOfNodes; node++)
//        if (parentOf[node] != -1)
//            cout << node + 1 << ' ' << parentOf[node] + 1;

    return 0;

}
