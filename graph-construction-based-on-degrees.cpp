//      The Edmonds-Karp max-flow algorithm can be used to determine the
// if a directed graph can be constructed based on each node's in-degree
// and out-degree.
//      Using the original nodes and their provided degrees, a flow
// network will be constructed, by copying each original node and adding
// an extra source and sink node. The edges between the originals and the
// copies will represent possible edges of the reconstructed graph and, as
// such, each will only have 1 as capacity to mark whether that particular
// edge was chosen by the Edmonds-Karp algorithm, which makes it part of the
// reconstructed graph. The original nodes are attached to the source, with
// their out-degrees as capacities for their corresponding edges, since each
// unit of flow taken to their copies represents an outgoing edge in the
// reconstructed graph. The sink will then be attached to the copied nodes,
// with the original nodes' in-degrees as capacities for their corresponding
// edges, since each unit of flow received from the original counterparts
// represents am incoming edge in the reconstructed graph.
//      Edmonds-Karp will run and determine the maximum flow which could be
// transported through the constructed network. Since each unit of flow represents
// an edge in the reconstructed graph, when the maximum flow differs from the
// sum of all nodes' in-degrees, a directed graph is impossible to reconstruct
// based on the provided values.
//      When a graph could be reconstructed, its edges are represented by the
// saturated nodes in the residual graph between original and copied nodes.
//
// Total time complexity - O(N*M^2)
// - Edmonds-Karp algorithm for determining the max-flow: O(N*M^2)
// - extra adjacency list traversal when printing the reconstructed edges: O(N+M)
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



// driver function, based on the Edmonds-Karp algorithm
bool generateDirectedGraphEdmondsKarp(const vector<pair<int, int>> *forwardAdjacencyList, int numberOfNodes, int numberOfEdgesToBeConstructed, int sourceNode, int sinkNode) {
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
            if (capacityAndFlow[parentNode][childNode].first - capacityAndFlow[parentNode][childNode].second <
                currentFlowUnits)
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

    // the max flow must be equal to the number of edges in the reconstructed graph
    if (maxFlow != numberOfEdgesToBeConstructed)
        return false;

    ofstream output("harta.out");

    output << maxFlow << '\n';

    // constructed edges' head ends are nodes in the original graph and
    // their tail ends are copies of the original graph's nodes;
    // an edge was constructed between two such nodes if and only if
    // its counterpart in the residual graph is saturated (i.e. its
    // flow is equal to 1)
    for (int outNode = 1; outNode <= numberOfNodes / 2 - 1; outNode++)
        for (auto inNode: forwardAdjacencyList[outNode])
            if (capacityAndFlow[outNode][inNode.first].second == 1)
                output << outNode << ' ' << inNode.first - (numberOfNodes / 2 - 1) << '\n';

    output.close();

    return true;
}



int main() {
    ifstream input("harta.in");

    int numberOfNodes;
    input >> numberOfNodes;

    // the network which will be built based on the provided in-degrees
    // and out-degrees will contain copies of each original node, an
    // extra source node and an extra sink node
    vector<pair<int, int>> adjacencyList[numberOfNodes * 2 + 2];

    int outDegree, inDegree, numberOfEdgesToBeConstructed = 0;
    for (int node = 1; node <= numberOfNodes; node++) {
        input >> outDegree >> inDegree;
        // the number of edges which will be constructed is the sum
        // of all node's in-degrees or out-degrees (property of
        // directed graphs)
        numberOfEdgesToBeConstructed += inDegree;

        // attaching original nodes to source;
        // since the original nodes will take units of flow to
        // their copied counterparts, the capacity of the edges
        // connecting them to the source must be the out-degree
        // of said nodes
        adjacencyList[0].emplace_back(node, outDegree);

        // an edge with a capacity of 1 exists between all the original nodes
        // and their copied counterparts (except their own copies)
        for (int adjacentNode = 1; adjacentNode <= numberOfNodes; adjacentNode++)
            if (adjacentNode != node)
                adjacencyList[node].emplace_back(adjacentNode + numberOfNodes, 1);

        // attaching sink to copied nodes;
        // since the copied nodes will receive units of flow from
        // their original counterparts, the capacity of the edges
        // connecting the sink to them must be the in-degree of
        // the original nodes
        adjacencyList[node + numberOfNodes].emplace_back(numberOfNodes * 2 + 1, inDegree);
    }

    input.close();

    if (not generateDirectedGraphEdmondsKarp(adjacencyList, numberOfNodes * 2 + 2, numberOfEdgesToBeConstructed, 0,
                                             numberOfNodes * 2 + 1)) {
        ofstream output("harta.out");
        output << "None";
        output.close();
    }

    return 0;
}