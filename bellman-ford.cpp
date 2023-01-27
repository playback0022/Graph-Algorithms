#include <bits/stdc++.h>

using namespace std;


// THIS VARIANT REQUIRES THE EDGES TO BE STORED IN AN EDGE LIST
// This it the standard version of the Bellman-Ford algorithm, with the
// slight improvement of bringing the main loop to a stop whenever no
// update to any node's distance has occurred during the current iteration.
// RETURN VALUE:
// - if (nullptr) then a cycle was detected
// - the 'distanceTo' was updated successfully and contains the correct result
void *standardBellmanFord(int numberOfNodes, const vector<vector<int>> &edgeList, int sourceNode, int *distanceTo) {
    // initializing array
    for (int i = 0; i < numberOfNodes; i++)
        distanceTo[i] = INT_MAX;

    // the source node is updated
    distanceTo[sourceNode] = 0;
    bool updatesOccurred = true;

    int i;
    // main loop => any node's final distance must be obtained by at most 'numberOfNodes' - 1 updates
    for (i = 0; i < numberOfNodes - 1 and updatesOccurred; i++) {
        updatesOccurred = false;

        for (const auto &edge: edgeList)
            if (distanceTo[edge[0]] + edge[2] < distanceTo[edge[1]]) {
                distanceTo[edge[1]] = distanceTo[edge[0]] + edge[2];

                // an update on a distance has occurred
                if (not updatesOccurred)
                    updatesOccurred = true;
            }
    }

    // only the graphs which reached the final iteration might contain negative cycles;
    // since the updates would never stop if a negative cycle exists, it is certain that
    // none can be found in the graph if no more updates occurred at an earlier iteration
    if (i == numberOfNodes - 1)
        for (const auto &edge: edgeList)
            // another update can occur => negative cycle
            if (distanceTo[edge[0]] + edge[2] < distanceTo[edge[1]])
                return nullptr;

    return distanceTo;
}


//// THIS VARIANT REQUIRES THE EDGES TO BE STORED IN AN ADJACENCY LIST
//// Variant which uses a priority queue to keep track of unprocessed nodes with modified distances.
//// In the original algorithm, if a node's distance wasn't modified during the previous iteration, then
//// its adjacent nodes' distances will certainly not be updated either, making the processing of said node redundant.
//// RETURN VALUE:
//// - if (nullptr) then a cycle was detected
//// - the 'distanceTo' was updated successfully and contains the correct result
//void *optimisedBellmanFord(int numberOfNodes, const vector<vector<pair<int, int>>> &adjacencyList, int sourceNode, int *distanceTo) {
//    // necessary to keep track of unprocessed modified nodes
//    queue<int> modifiedNodes;
//    // a node's distance can only be updated in as many as 'numberOfNodes' iteration;
//    // any more than that and it is clear that that particular node is part of a cycle;
//    int timesUpdated[numberOfNodes];
//
//    // initializing arrays
//    for (int i = 0; i < numberOfNodes; i++) {
//        distanceTo[i] = INT_MAX;
//        timesUpdated[i] = 0;
//    }
//
//    // pushing the source node and performing the necessary alterations in the arrays
//    distanceTo[sourceNode] = 0;
//    timesUpdated[sourceNode] = 1;
//    modifiedNodes.push(sourceNode);
//
//    int currentNode;
//    // algorithm stalls when either there no more nodes' distances have been modified
//    // or when a node with more than 'numberOfNodes' updates to its distance has been
//    // found (and, therefore, a cycle was detected)
//    while (not modifiedNodes.empty()) {
//        currentNode = modifiedNodes.front();
//        modifiedNodes.pop();
//
//        // updating adjacent nodes' distances
//        for (const auto &adjacentNode: adjacencyList[currentNode])
//            // it might happen that a node had been pushed to the queue multiple times
//            // and, between the processing of its first instance and its remaining instances,
//            // its distance remained unmodified; it is therefore necessary to verify the condition
//            // bellow applies, not to traverse its adjacent nodes unnecessarily and falsely mark it as updated
//            if (distanceTo[currentNode] + adjacentNode.second < distanceTo[adjacentNode.first]) {
//                // cycle detected
//                if (timesUpdated[adjacentNode.first] > numberOfNodes - 1)
//                    return nullptr;
//
//                // updating distance
//                distanceTo[adjacentNode.first] = distanceTo[currentNode] + adjacentNode.second;
//                // marking the new update
//                timesUpdated[adjacentNode.first]++;
//                // pushing adjacent node to the queue for processing
//                modifiedNodes.push(adjacentNode.first);
//            }
//    }
//
//    return (void *) distanceTo;
//}


int main() {
    int numberOfNodes, numberOfEdges, firstNode, secondNode, cost;

    ifstream input("bellmanford.in");

    input >> numberOfNodes >> numberOfEdges;
    vector<vector<int>> edgeList;

    for (int i = 0; i < numberOfEdges; i++) {
        input >> firstNode >> secondNode >> cost;
        firstNode--;
        secondNode--;
        edgeList.push_back({firstNode, secondNode, cost});
    }

    input.close();

    ofstream output("bellmanford.out");

    int distanceTo[numberOfNodes];

    if (standardBellmanFord(numberOfNodes, edgeList, 0, distanceTo) == nullptr)
        output << "Ciclu negativ!";
    else
        for (int i = 1; i < numberOfNodes; i++)
            output << distanceTo[i] << ' ';

    output.close();

    return 0;
}
