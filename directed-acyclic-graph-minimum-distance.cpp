#include <bits/stdc++.h>
using namespace std;


// Kahn's algorithm for topological sorting
void getTopologicalOrder(int numberOfNodes, vector<pair<int, int>> *adjacencyList, int *topologicalOrder) {
    // since at every step of the algorithm the nodes
    // with a null in-degree are chosen, an array that
    // keeps track of all the nodes' in-degrees throughout
    // the execution of the algorithm is necessary
    int inDegrees[numberOfNodes];

    // initializing the array
    for (int i = 0; i < numberOfNodes; i++)
        inDegrees[i] = 0;

    // computation of the initial in-degree of every node;
    // traversing the adjacency list, the in-degree of every
    // adjacent node to the current is incremented
    for (int i = 0; i < numberOfNodes; i++)
        for (auto node: adjacencyList[i])
            inDegrees[node.first]++;

    // the queue in which nodes with null
    // in-degrees will be pushed at every step
    queue<int> queueForNextInSorting;

    // pushing all nodes with initial null in-degrees in the queue
    for (int i = 0; i < numberOfNodes; i++)
        if (inDegrees[i] == 0) {
            queueForNextInSorting.push(i);
        }

    // i - incremented index used to store nodes in 'topologicalOrder' array
    // count - incremented whenever a new node with a null in-degree is popped from the queue
    int i = 0, count = 0, currentNode;
    // in case of a DAG (directed acyclic graph), all nodes will get
    // pushed in and popped from the queue exactly once;
    // if a cycle is encountered, the loop will halt prior to the
    // processing of all the nodes;
    while (not queueForNextInSorting.empty()) {
        currentNode = queueForNextInSorting.front();
        // next node in the topological order has been found
        topologicalOrder[i++] = currentNode;
        // popping 'currentNode' from the queue
        queueForNextInSorting.pop();

        // updating the in-degree of all the nodes adjacent to the current node
        for (auto adjacentNode: adjacencyList[currentNode]) {
            inDegrees[adjacentNode.first]--;
            // whenever a node reaches a null in-degree following the removal
            // of all of its incoming nodes, it is pushed in the queue
            if (inDegrees[adjacentNode.first] == 0)
                queueForNextInSorting.push(adjacentNode.first);
        }

        count++;
    }

    // a cycle was encountered => marking first position as -1
    if (count < numberOfNodes)
        topologicalOrder[0] = -1;
}


int getMinimumDistance(int numberOfNodes, int sourceNode, const vector<pair<int, int>> *adjacencyList, const int *topologicalOrder, int *minimumDistanceTo, int *parentOf) {
    for (int i = 0; i < numberOfNodes; i++)
        minimumDistanceTo[i] = INT_MAX, parentOf[i] = 0;

    minimumDistanceTo[sourceNode] = 0;

    int i = 0;
    // ignore nodes before the sourceNode in the topological order
    for (; i<numberOfNodes && topologicalOrder[i] != sourceNode; i++);

    int currentNode;
    // traverse the nodes in the topological order
    for (; i < numberOfNodes; i++) {
        currentNode = topologicalOrder[i];
        // only reachable nodes are processed further
        if (minimumDistanceTo[currentNode] != INT_MIN)
            // traversing all of its adjacent nodes and updating
            // their minimum distance wherever necessary
            for (auto adjacentNode: adjacencyList[currentNode])
                // a smaller distance to an adjacent node was found
                if (minimumDistanceTo[currentNode] + adjacentNode.second < minimumDistanceTo[adjacentNode.first])
                    minimumDistanceTo[adjacentNode.first] =
                            minimumDistanceTo[currentNode] + adjacentNode.second, parentOf[adjacentNode.first] = currentNode;
    }
}


int main() {
    int numberOfNodes, numberOfEdges, firstNode, secondNode, cost, startNode;

    ifstream input("easygraph.in");
    ofstream output("easygraph.out");

    input >> startNode >> numberOfNodes >> numberOfEdges;

    vector<pair<int, int>> adjacencyList[numberOfNodes];
    for (int j = 0; j < numberOfEdges; j++) {
        input >> firstNode >> secondNode >> cost;
        firstNode--;
        secondNode--;
        adjacencyList[firstNode].emplace_back(secondNode, cost);
    }

    int topologicalOrder[numberOfNodes];
    getTopologicalOrder(numberOfNodes, adjacencyList, topologicalOrder);

    int minimumDistanceTo[numberOfNodes], parentOf[numberOfNodes];
    output << getMinimumDistance(numberOfNodes, startNode, adjacencyList, topologicalOrder, minimumDistanceTo,
                                 parentOf);

    output.close();
    input.close();

    return 0;
}