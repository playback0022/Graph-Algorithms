//      Finding the maximum bipartite matching can be achieved
// by building a network based on the underlying bipartite graph.
//      A source and a sink node can be attached to the existing
// graph to satisfy the conditions of a flow network, in order to
// apply Ford-Fulkerson (Edmonds-Karp) upon the resulting network.
// The nodes in the left set are attached to the source and the
// sink will be attached to the nodes in the right set. We know
// that Ford-Fulkerson determines the maximum flow, which means
// we can use it to determine the maximum matching if we can derive
// a relationship between the capacity of our newly-generated flow
// network and the cardinality of said matching in the original graph.
//      The trick is assigning 1 as a capacity to all the edges
// in the flow network. This way, each node from the left set
// can send at most 1 unit of flow to a node in the right set,
// thus matching with it. Since each flow unit now represents
// a match between two nodes, Ford-Fulkerson will determine the
// maximum flow and, therefore, the maximum bipartite matching.
//      Finding the edges in the maximum bipartite matching is
// then achieved by traversing part of the residual graph after
// Edmonds-Karp's completion. The edges which are part of the
// final matching must be saturated and have a left-set node
// as their head and a right-set node as their tail.
//
// Total time complexity - O(N*M^2)
// - Edmonds-Karp algorithm for determining the max-flow: O(N*M^2)
// - extra adjacency list traversal when printing the matching edges: O(N+M)
#include <bits/stdc++.h>
using namespace std;



// slightly modified BFS - used to fin augmenting paths
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
void getMaxBipartiteMatching(const vector<pair<int, int>> *forwardAdjacencyList, int numberOfNodes, int numberOfNodesLeft,
                             int sourceNode, int sinkNode) {
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

    ofstream output("cuplaj.out");

    output << maxFlow << '\n';

    // the edges in the bipartite matching send flow from the nodes in the
    // left set to nodes in the right set;
    for (int nodeInLeft = 1; nodeInLeft <= numberOfNodesLeft; nodeInLeft++)
        for (auto nodeInRight: forwardAdjacencyList[nodeInLeft])
            // edge is part of the matching if and only if it is saturated
            // (i.e. it was used to send 1 unit of flow to the sink)
            if (capacityAndFlow[nodeInLeft][nodeInRight.first].second == 1)
                output << nodeInLeft << ' ' << nodeInRight.first - numberOfNodesLeft << '\n';

    output.close();
}



int main() {
    ifstream input("cuplaj.in");

    int numberOfNodesLeft, numberOfNodesRight, numberOfEdges;
    input >> numberOfNodesLeft >> numberOfNodesRight >> numberOfEdges;
    // the total number of nodes involves both the nodes in the left
    // set and those in the right set;
    // in order to apply the Fold-Fulkerson algorithm, two extra nodes
    // are added (a source node and a sink node)
    int numberOfNodes = numberOfNodesRight + numberOfNodesLeft + 2;

    // common adjacency list for both node sets
    vector<pair<int, int>> adjacencyList[numberOfNodes];

    int leftNode, rightNode;
    for (int i = 0; i < numberOfEdges; i++) {
        input >> leftNode >> rightNode;
        // right node indices are translated by the number of nodes in the
        // left set, in order to distinguish the two sets in the common graph
        rightNode = numberOfNodesLeft + rightNode;

        // all edges have the capacity equal to 1
        adjacencyList[leftNode].emplace_back(rightNode, 1);
    }

    input.close();

    // attaching source node to nodes in the left set
    for (int nodeInLeft = 1; nodeInLeft <= numberOfNodesLeft; nodeInLeft++)
        adjacencyList[0].emplace_back(nodeInLeft, 1);

    // attaching nodes in the right set to sink node
    for (int nodeInRight = numberOfNodesLeft + 1; nodeInRight <= numberOfNodes - 2; nodeInRight++)
        adjacencyList[nodeInRight].emplace_back(numberOfNodes - 1, 1);

    getMaxBipartiteMatching(adjacencyList, numberOfNodes, numberOfNodesLeft, 0, numberOfNodes - 1);

    return 0;
}