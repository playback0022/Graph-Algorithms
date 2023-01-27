// O(k(m+n)) - k DFS traversals, where k is the number of control points
#include <bits/stdc++.h>

using namespace std;


// modified BFS function
void bfs(int controlPoint, int numberOfNodes, vector<int> *adjacencyList, const bool *isControlPoint,
         int *distance) {
    int currentNode;
    queue<int> workingQueue;
    bool isVisited[numberOfNodes];

    for (int node = 0; node < numberOfNodes; node++)
        isVisited[node] = false;

    // beginning of BFS
    workingQueue.push(controlPoint);
    isVisited[controlPoint] = true;

    while (not workingQueue.empty()) {
        currentNode = workingQueue.front();
        workingQueue.pop();

        // traversing all adjacent nodes to the current node
        for (auto adjacentNode: adjacencyList[currentNode]) {
            // an adjacent node is added to the queue only if it meets the following criteria:
            //  - it hasn't been visited before (else an infinite loop would occur)
            //  - it isn't a control point (all adjacent nodes to other control points
            // are closer to them and their distance either was or will be computed
            // during their corresponding control point BFS traversal)
            //  - its distance wasn't computed relative to any other control point yet
            //  - its distance relative to another control point was computed, and it is
            // longer than the distance to the current control point (it is pointless to
            // traverse the adjacent node's adjacent nodes, since all of their distances
            // will be shorter relative to the current control point)
            if (not isVisited[adjacentNode] and
                not isControlPoint[adjacentNode] and (distance[adjacentNode] != -1 and
                                                      distance[currentNode] + 1 < distance[adjacentNode] or
                                                      distance[adjacentNode] == -1)) {

                workingQueue.push(adjacentNode);
                // marking adjacent node as 'visited' in advance to prevent other nodes
                // which are also adjacent to it from adding it repeatedly to the queue;
                isVisited[adjacentNode] = true;

                // the distance from the root control point to the adjacent node is
                // the incremented distance from the current node to it, since only
                // one more edge was crossed to reach it;
                distance[adjacentNode] = distance[currentNode] + 1;
            }
        }
    }
}


int main() {
    int numberOfNodes, numberOfEdges, firstNode, secondNode;
    ifstream input;

    // storing graph data
    input.open("graf.in");

    input >> numberOfNodes >> numberOfEdges;

    vector<int> adjacencyList[numberOfNodes];
    bool isControlPoint[numberOfNodes];
    int shortestDistanceToControlPoint[numberOfNodes];

    // initializing arrays
    for (int node = 0; node < numberOfNodes; node++) {
        isControlPoint[node] = false;
        // '-1' marks the distance from 'node' to a control point as not yet known
        shortestDistanceToControlPoint[node] = -1;
    }

    // reading and storing edge data
    for (int edge = 0; edge < numberOfEdges; edge++) {
        input >> firstNode >> secondNode;
        adjacencyList[firstNode - 1].push_back(secondNode - 1);
        adjacencyList[secondNode - 1].push_back(firstNode - 1);
    }

    // reading and storing control point data
    while (input >> firstNode) {
        isControlPoint[firstNode - 1] = true;
        shortestDistanceToControlPoint[firstNode - 1] = 0;
    }

    input.close();


    // a (partial) BFS will be performed starting from every control point
    for (int node = 0; node < numberOfNodes; node++) {
        if (isControlPoint[node])
            bfs(node, numberOfNodes, adjacencyList, isControlPoint, shortestDistanceToControlPoint);
    }


    // writing result to file
    ofstream output;
    output.open("graf.out");

    for (int node = 0; node < numberOfNodes; node++)
        output << shortestDistanceToControlPoint[node] << ' ';

    output.close();

    return 0;
}