// Complexity:
// 1st version: using maximum distance function - O(n^2)
// - every call of the maximum distance function: O(n)
// - every potential node calling said function: O(n)
// 2nd version: using a priority queue - O(n + m*log(n)) => O(m*log(n))
// - every push and pop using the queue: O(log(n))
// - every edge processed: (roughly) O(m)
// - the nodes being traversed: O(n)
#include <bits/stdc++.h>
using namespace std;


//// minimum distance function
//int getNodeWithMinimumDistanceFromStart(int numberOfNodes, const int *distanceTo, const bool *isVisited) {
//    // initializing values
//    int node = -1;
//    int minimumDistance = MAXFLOAT;
//
//    // updating values based on those of the unvisited nodes
//    for (int i = 0; i < numberOfNodes; i++)
//        if (not isVisited[i] and distanceTo[i] < minimumDistance)
//            node = i, minimumDistance = distanceTo[i];
//
//    return node;
//}
//
//
//// Dijkstra's Shortest Path Algorithm - Minimum distance function version
//int dijkstra(int numberOfNodes, const vector<pair<int, int>> *adjacencyList, int startNode, int endNode) {
//    int parentOf[numberOfNodes];
//    int distanceTo[numberOfNodes];       // distance array
//    bool isVisited[numberOfNodes];
//
//    // initializing 'parentOf', 'distance' and 'isVisited' arrays
//    for (int i = 0; i < numberOfNodes; i++)
//        parentOf[i] = -1, distanceTo[i] = MAXFLOAT, isVisited[i] = false;
//
//    // closest node to the start node is the
//    // start node itself;
//    distanceTo[startNode] = 0;
//
//    int currentNode = -1;
//    // when the end node has been processed, the execution
//    // for the remaining reachable nodes can stop
//    while (currentNode != endNode) {
//        // no more reachable nodes from the start node could
//        // be found, which means that the end node is not
//        // reachable from the start node
//        if ((currentNode = getNodeWithMinimumDistanceFromStart(numberOfNodes, distanceTo, isVisited)) == -1) {
//            distanceTo[endNode] = 0;
//            break;
//        }
//
//        // marking node as visited to avoid future processing
//        isVisited[currentNode] = true;
//        // updating all adjacent nodes to the 'distance' from the start node to the
//        // current node plus the distance from the current node to the adjacent node
//        for (auto adjacentNode: adjacencyList[currentNode])
//            if (not isVisited[adjacentNode.first] and distanceTo[currentNode] + adjacentNode.second < distanceTo[adjacentNode.first]) {
//                distanceTo[adjacentNode.first] = distanceTo[currentNode] + adjacentNode.second;
//                // the parent of the adjacent node in the path from
//                // the start node must also be updated
//                parentOf[adjacentNode.first] = currentNode;
//            }
//    }
//
//    return distanceTo[endNode];
//}


// Dijkstra's Shortest Path Algorithm - Priority Queue version
int dijkstra(int numberOfNodes, const vector<pair<int, int>> *adjacencyList, int startNode, int endNode) {
    int parentOf[numberOfNodes];
    int distanceTo[numberOfNodes];       // distance array
    bool isVisited[numberOfNodes];

    // instead of the function returning the unvisited node with
    // the maximum 'distance' from the start node, which requires
    // n operations for every call, a priority queue is used to
    // retrieve said node for each operation, with an improved
    // complexity of log(n)
    priority_queue<pair<int, int>> priorityQueue;

    // initializing 'parentOf', 'distance' and 'isVisited' arrays
    for (int i = 0; i < numberOfNodes; i++)
        parentOf[i] = -1, distanceTo[i] = INT_MAX, isVisited[i] = false;

    // closest node to the start node is the
    // start node itself;
    // the probability to pass the start node
    // will be considered to be the certain event
    distanceTo[startNode] = 0;
    priorityQueue.push({0, startNode});

    // used for extracting current node with
    // maximum 'distance' from the start node
    int currentNode = -1;
    // 2 possible cases for which execution stalls:
    //  - there are no more reachable nodes from the start node
    //  - the end node has been processed, which means that execution
    // for the remaining reachable nodes can stop
    while (not priorityQueue.empty() and currentNode != endNode) {
        currentNode = priorityQueue.top().second;
        priorityQueue.pop();

        // since the priority queue does not support an operation
        // for updating values already pushed into it, whenever
        // an update of the 'distance' array occurs, a new pair
        // of values will be pushed in the queue;
        // even though multiple other distance values paired with
        // the same node may be inside the queue at the same time,
        // only the first occurrence will be processed (the true
        // maximum 'distance'), while ignoring the rest;
        if (not isVisited[currentNode]) {
            // marking node as visited to avoid future processing
            isVisited[currentNode] = true;

            // updating all adjacent nodes to the 'distance' from the start node to the
            // current node plus the distance from the current node to the adjacent node
            for (auto adjacentNode: adjacencyList[currentNode])
                if (not isVisited[adjacentNode.first] and distanceTo[currentNode] + adjacentNode.second <
                                                          distanceTo[adjacentNode.first]) {
                    // the parent of the adjacent node in the path from
                    // the start node must also be updated
                    parentOf[adjacentNode.first] = currentNode;
                    distanceTo[adjacentNode.first] = distanceTo[currentNode] + adjacentNode.second;
                    // the new pair of values gets pushed in the priority queue
                    priorityQueue.push({-distanceTo[adjacentNode.first], adjacentNode.first});
                }
        }
    }

    // there is no path from the start to the end node
    if (distanceTo[endNode] == INT_MAX)
        return 0;

    return distanceTo[endNode];
}


int main() {

    return 0;
}
