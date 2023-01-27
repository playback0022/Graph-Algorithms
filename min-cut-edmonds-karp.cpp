//      This implementation follows the Edmonds-Karp specification.
// A BFS search is used to find augmenting paths, guaranteeing that
// the shortest augmenting path is chosen at every step, the only
// notable caveats being that two extra edge traversals occur. Since
// at the very worst, all edges of the original graph are bidirectional,
// the BFS will traverse all edges one extra time. Furthermore, the
// augmenting path is reconstructed two times: once in order to compute
// the flow amount which can pe pushed through the path and once in order
// to update the flow values along the path. Taking all extra operations
// into consideration, this implementation maintains the original time
// complexity of the algorithm of O(N*M^2).
//
//      Determining the min-cut involves a DFS traversal with a complexity
// of O(N+M) and printing the underlying edges takes just as much time.
// Therefore, the total time complexity of algorithm for determining the
// min-cut remains O(N*M^2).
#include <bits/stdc++.h>
using namespace std;



// slightly modified BFS - used to find augmenting paths
bool BFS(const vector<pair<int, int>> *forwardAdjacencyList, const vector<pair<int, int>> *backwardAdjacencyList,
         const vector<vector<pair<int, int>>> &capacityAndFlow, int *parentOf, int numberOfNodes, int sourceNode,
         int sinkNode) {
    // declaring and initializing 'isVisited' array
    bool isVisited[numberOfNodes];
    for (int i = 0; i < numberOfNodes; i++)
        isVisited[i] = false;

    queue<int> workingQueue;
    // BFS begins from the source node
    workingQueue.push(sourceNode);
    isVisited[sourceNode] = true;


    // BFS stalls when no more nodes can be
    // accessed or when the sink node is reached
    while (not workingQueue.empty()) {
        int currentNode = workingQueue.front();
        workingQueue.pop();

        // traversing adjacent nodes reachable from forward edges
        for (auto adjacentNode: forwardAdjacencyList[currentNode])
            // only edges with non-null residual edges can be traversed
            if (capacityAndFlow[currentNode][adjacentNode.first].first -
                capacityAndFlow[currentNode][adjacentNode.first].second > 0) {
                // sink node reached
                if (adjacentNode.first == sinkNode) {
                    // marking the parent of the sink node accordingly
                    parentOf[sinkNode] = currentNode;
                    return true;
                }
                    // unvisited node reached
                else if (not isVisited[adjacentNode.first]) {
                    // pushed to the queue
                    workingQueue.push(adjacentNode.first);
                    // setting the parent node
                    parentOf[adjacentNode.first] = currentNode;
                    // marked as visited
                    isVisited[adjacentNode.first] = true;
                }
            }

        // traversing adjacent nodes reachable from backward edges;
        // the same exact operations occur
        for (auto adjacentNode: backwardAdjacencyList[currentNode])
            if (capacityAndFlow[currentNode][adjacentNode.first].first -
                capacityAndFlow[currentNode][adjacentNode.first].second > 0) {
                if (adjacentNode.first == sinkNode) {
                    parentOf[sinkNode] = currentNode;
                    return true;
                } else if (not isVisited[adjacentNode.first]) {
                    workingQueue.push(adjacentNode.first);
                    parentOf[adjacentNode.first] = currentNode;
                    isVisited[adjacentNode.first] = true;
                }
            }
    }

    // sink node could not be reached -> augmenting path couldn't be found
    return false;
}



// modified DFS; used to traverse the residual graph when no more augmenting paths exist;
// the goal is to mark all reachable nodes from the source node as such;
void DFS(const vector<pair<int, int>> *forwardAdjacencyList, const vector<pair<int, int>> *backwardAdjacencyList,
         const vector<vector<pair<int, int>>> &capacityAndFlow, int currentNode, bool *isVisited) {
    isVisited[currentNode] = true;  // mark node as visited

    // traversing all adjacent nodes through forwards edges
    for (auto adjacentNode: forwardAdjacencyList[currentNode])
        // only not visited edges with non-null residual capacities are taken into consideration
        if (not isVisited[adjacentNode.first] and capacityAndFlow[currentNode][adjacentNode.first].first -
                                                  capacityAndFlow[currentNode][adjacentNode.first].second > 0)
            DFS(forwardAdjacencyList, backwardAdjacencyList, capacityAndFlow, adjacentNode.first, isVisited);

    // traversing all adjacent nodes through forwards edges
    for (auto adjacentNode: backwardAdjacencyList[currentNode])
        // only not visited edges with non-null residual capacities are taken into consideration
        if (not isVisited[adjacentNode.first] and capacityAndFlow[currentNode][adjacentNode.first].first -
                                                  capacityAndFlow[currentNode][adjacentNode.first].second > 0)
            DFS(forwardAdjacencyList, backwardAdjacencyList, capacityAndFlow, adjacentNode.first, isVisited);
}



// driver function for the Edmonds-Karp algorithm
void
printMinCutWithMaxFlowEdmondsKarp(const vector<pair<int, int>> *forwardAdjacencyList, int numberOfNodes, int sourceNode,
                                  int sinkNode) {
    // matrix which holds both the capacity and the currently running flow of an edge from node i to node j
    vector<vector<pair<int, int>>> capacityAndFlow(numberOfNodes,
                                                   vector<pair<int, int>>(numberOfNodes, pair<int, int>()));
    // since there can be a bidirectional edge between two nodes in the original graph, it's easiest
    // to maintain a separate adjacency list for backwards edges, not to overwrite the original
    // bidirectional edges with backward edges, if there are any;
    vector<pair<int, int>> backwardAdjacencyList[numberOfNodes];

    // initializing the backward adjacency list and the capacity-flow matrix
    for (int node = 0; node < numberOfNodes; node++)
        for (auto adjacentNode: forwardAdjacencyList[node]) {
            // storing backward edge
            backwardAdjacencyList[adjacentNode.first].emplace_back(node, adjacentNode.second);

            // all pairs in the matrix are initially 0;
            // even though the forward and backwards edges are stored in distinct
            // adjacency lists, both the capacity and flow will be shared between
            // edges with the same direction, but different types (one forward, one
            // backward), since it is the same as keeping them separate;
            // in order to achieve this, their capacities and flows will be summed
            // up from the very beginning;
            // from now on, whether a forward or backward edge with the same direction
            // connecting the same two nodes will be chosen is indifferent;

            // adding the capacity of the forward edge
            capacityAndFlow[node][adjacentNode.first].first += adjacentNode.second;
            // the flow of the forward edge is initially 0, therefore it was omitted

            // adding the capacity of the backward edge
            capacityAndFlow[adjacentNode.first][node].first += adjacentNode.second;
            // adding the flow of the backward edge;
            // since the residual capacity is the total capacity - the current flow,
            // the backward edge is essentially saturated, so it can't be traversed
            // anymore;
            capacityAndFlow[adjacentNode.first][node].second += adjacentNode.second;
        }


    int parentOf[numberOfNodes], maxFlow = 0;
    // algorithm stalls when no more augmenting paths exist
    while (BFS(forwardAdjacencyList, backwardAdjacencyList, capacityAndFlow, parentOf, numberOfNodes, sourceNode, sinkNode)) {
        // initializing current flow through the found path
        int currentFlowUnits = INT_MAX;

        // first path reconstruction based on 'parentOf' array;
        // computing the minimum flow units which can be pushed
        // through the augmenting path
        for (int childNode = sinkNode; childNode != sourceNode; childNode = parentOf[childNode]) {
            int parentNode = parentOf[childNode];

            // residual capacity = total capacity - current flow
            if (capacityAndFlow[parentNode][childNode].first - capacityAndFlow[parentNode][childNode].second < currentFlowUnits)
                currentFlowUnits = capacityAndFlow[parentNode][childNode].first - capacityAndFlow[parentNode][childNode].second;
        }

        // second path reconstruction -> updating the flow of every edge of the augmenting path
        for (int childNode = sinkNode; childNode != sourceNode; childNode = parentOf[childNode]) {
            int parentNode = parentOf[childNode];

            // residual capacity of forward edge + residual capacity of backward edge = total capacity;
            // whenever the flow of one of the edges is modified, the other must be updated accordingly;
            // adding flow units to the chosen edge
            capacityAndFlow[parentNode][childNode].second += currentFlowUnits;
            // removing the same amount of flow units from the opposite edge
            capacityAndFlow[childNode][parentNode].second -= currentFlowUnits;
        }

        maxFlow += currentFlowUnits;
    }


    // the trick to finding a set of edges that represent a minimum cut in the original graph
    // is traversing the residual graph at the final step of the Ford-Fulkerson algorithm, when
    // no augmenting paths exist anymore;
    // using a DFS traversal of the residual graph from the source node, all encountered nodes
    // reachable through unsaturated edges are marked;
    // the adjacent nodes to the marked nodes are connected through forward edges that must be
    // saturated and their sum must be exactly the value of the max flow, which was previously
    // determined, since they represent the bottleneck of the network (i.e. if the sum was
    // smaller than the max-flow, there must have been other augmenting paths from the source
    // to the sink);
    // since all these edges are fully saturated, their flow is equal to their capacity, which
    // means that the sum of their weights in the original graph represents the max flow of the
    // network;
    // by the min-cut-max-flow theorem, this makes said set of edges the sought after min-cut;

    bool isVisited[numberOfNodes];          // used for marking reachable nodes from source
    for (int i = 0; i < numberOfNodes; i++)
        isVisited[i] = false;

    // DFS traversal with which reachable nodes from the source in the residual graph are marked
    DFS(forwardAdjacencyList, backwardAdjacencyList, capacityAndFlow, sourceNode, isVisited);
    // printing all edges from the marked set to the unmarked set
    for (int node = 0; node < numberOfNodes; node++)
        if (isVisited[node])
            for (auto adjacentNode: forwardAdjacencyList[node])
                if (not isVisited[adjacentNode.first])
                    cout << node << " -> " << adjacentNode.first << '\n';
}



int main() {
    ifstream input("maxflow.in");

    int numberOfNodes, numberOfEdges;
    input >> numberOfNodes >> numberOfEdges;

    vector<pair<int, int>> adjacencyList[numberOfNodes];
    int parentNode, childNode, capacity;
    for (int i = 0; i < numberOfEdges; i++) {
        input >> parentNode >> childNode >> capacity;
        parentNode--;
        childNode--;
        adjacencyList[parentNode].emplace_back(childNode, capacity);
    }

    input.close();

    printMinCutWithMaxFlowEdmondsKarp(adjacencyList, numberOfNodes, 0, numberOfNodes - 1);

    return 0;
}