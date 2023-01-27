#include <bits/stdc++.h>

using namespace std;


int closingTheCycleNode;

void convertToAdjacencyList(vector<vector<int>> &vectorOfPairs, vector<int> *adjacencyList) {
    for (auto pair: vectorOfPairs)
        adjacencyList[pair[1]].push_back(pair[0]);
}


bool containsCycleDFS(int node, vector<int> *adjacencyList, bool *isVisited, bool *isInStack, vector<int> &reversedCycle) {
    // whenever a new node is traversed,
    // it must be marked as visited,
    isVisited[node] = true;
    // and marked as such
    isInStack[node] = true;

    // then, all of its adjacent nodes will also be traversed
    for (auto adjacentNode: adjacencyList[node]) {
        // only unvisited nodes will be traversed further.
        // if a cycle was discovered at some point in the recursion chain, 'true' will cascade back,
        // otherwise the loop will keep on checking the rest of the adjacent nodes, since a function
        // call returning 'false' only means it encountered a dead end, not that there can't be any
        // other paths leading to a cycle.
        if (not isVisited[adjacentNode] &&
            containsCycleDFS(adjacentNode, adjacencyList, isVisited, isInStack, reversedCycle)) {
            // WHEN 'TRUE' CASCADES BACK, THE RECURSIVE CALL CHAIN WILL TRAVERSE
            // THE NODES IN THE CYCLE IN THE REVERSE ORDER OF THE PATH TAKEN
            reversedCycle.push_back(node);

            return true;
        }

            // if the node is visited and also in the stack, it must be the node which 'completes' or
            // 'closes' the cycle, otherwise it is part of an already visited acyclic DFS tree
        else if (isInStack[adjacentNode]) {
            // THIS IS WHERE THE NODE COMPLETING THE CYCLE IS ENCOUNTERED
            reversedCycle.push_back(adjacentNode);
            // pushing the node that led to it
            reversedCycle.push_back(node);
            closingTheCycleNode = adjacentNode;

            return true;
        }
    }

    // unmarking 'dead-end' paths
    isInStack[node] = false;
    return false;
}


// wrapper function used for traversing all DFS trees
vector<int> containsCycle(int numberOfNodes, vector<vector<int>> &adjacencyList) {
    // storing edges in a more convenient way (array of vectors)
    vector<int> adjacencyListModified[numberOfNodes];
    convertToAdjacencyList(adjacencyList, adjacencyListModified);

    bool isVisited[numberOfNodes]; // necessary to mark visited nodes
    bool isInStack[numberOfNodes]; // for checking if a node is in the stack

    for (int i = 0; i < numberOfNodes; i++) {
        isVisited[i] = false;
        isInStack[i] = false;
    }

    vector<int> reversedCycle;  // vector which will be returned
    // traversing all nodes, to analyse all DFS trees
    for (int i = 0; i < numberOfNodes; i++)
        // visited nodes are part of a traversed DFS tree, therefore only unvisited nodes will be considered;
        // acyclic DFS trees will be ignored and the loop will proceed in checking the remaining trees
        if (not isVisited[i] && containsCycleDFS(i, adjacencyListModified, isVisited, isInStack, reversedCycle)) {
            // for every DFS tree, the stack begins as empty and is continuously
            // popped from and pushed to during the traversal;
            // whenever a new node is visited, it is pushed to the stack along with each of its
            // adjacent nodes, recursively searching for a possible path which ends in a cycle.
            // all encountered 'dead-end' paths are then discarded, back to the first node which
            // still has possible paths to take (unvisited adjacent nodes);
            // since the function exits when a cycle is detected, it is guaranteed that the stack
            // only contains the uninterrupted path that leaded back to one of the nodes in the path.
            // the stack might also contain nodes which leaded to the detected cycle, but of which
            // they aren't a part of, which is where 'closingTheCycleNode' helps.
            // when a node already in the stack is encountered, it is stored in said variable, since
            // it completes the cycle.
            // upon returning, the vector is traversed and the first elements which are not equal to
            // the completing node will be discarded
            int j;
            for (j = 0; j < reversedCycle.size() && reversedCycle[j] != closingTheCycleNode; j++);

            vector<int> cycle;
            for (; j < reversedCycle.size(); j++)
                cycle.push_back(reversedCycle[j]);

            // since the nodes popped from the stack are in the reverse order of traversal, the vector
            // must be reversed in order to find the correct cycle direction;
            reverse(cycle.begin(), cycle.end());

            return cycle;
        }

    // no cycles were found in any of the DFS trees
    return {};
}


int main() {

    return 0;
}
