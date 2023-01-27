//      The general idea behind Hierholzer's algorithm is cycle fusion.
// In regards with cycle finding, an initial cycle is traversed and,
// along the way, other cycles connected to the initial cycle through
// various single nodes already in the initial cycle might be encountered,
// at which moment they are fused into the 'main' one. The algorithm goes
// quite similarly for eulerian paths, the notable difference being that
// instead of an initial 'main' cycle, the main path is traversed. When
// various cycles are discovered, they are fused with or 'glued' to this
// main path.
//      This approach can be achieved by using a post-order DFS traversal,
// which inserts nodes into the cycle/path only once all of their adjacent
// nodes' accessible edges have been traversed, which guarantees that no
// edges were omitted. When an edge is traversed, it must be marked as
// visited to avoid crossing it again.
//
// Complexity: All edges are traversed exactly three times (once for computing
// the node degrees, once when validating the theorem conditions and once
// during the DFS) -> O(M)
#include <bits/stdc++.h>
using namespace std;



// post-order traversal and visited edge 'removal'
void HierholzerDFS(int currentNode, vector<vector<int>> &path, unordered_map<int, vector<int>> &adjacencyList, unordered_map<int, int> &outDegrees) {
    // loop stalls when no more unvisited out-going
    // edges from the current node can be found;
    // it is most convenient to traverse the edges
    // of a node in reverse order, by using its out-degree and
    // simply decrementing it whenever the current edge is processed
    while (outDegrees[currentNode]) {
        // the storing the adjacent node at the head end of the next unvisited edge
        int adjacentNode = adjacencyList[currentNode][outDegrees[currentNode] - 1];
        // marking said edge as visited
        outDegrees[currentNode]--;
        // recursively call the function upon the adjacent node
        HierholzerDFS(adjacentNode, path, adjacencyList, outDegrees);
        // post-order traversal;
        // the current node and its chosen adjacent node are appended only once the recursive call
        // finishes executing (i.e. all the chosen adjacent node's out-going paths have been visited),
        // in order to maintain the edge relationships
        path.push_back({currentNode, adjacentNode});
    }
}



// driver function -> general eulerian path/cycle finding algorithm
// when the algorithm is expected to determine a cycle and not
// only a path, the corresponding parameter should be provided
// !!! RETURNS A LIST OF EDGES, NOT THE LIST OF NODES !!!
vector<vector<int>> HierholzerGetPath(vector<vector<int>> &pairs, bool cycle = false) {
    // needed for DFS traversal
    unordered_map<int, vector<int>> adjacencyList;
    // needed to determine the start node and, later on,
    // to mark out-going edges as visited
    unordered_map<int, int> inDegrees;
    unordered_map<int, int> outDegrees;
    // some nodes might not have null in-degrees or out-degrees,
    // which makes it necessary to store a list of all the nodes
    // in the graph, in order to be able to iterate through it
    // during the in-out-degree eulerian graph validation
    set<int> nodes;

    int numberOfEdges = pairs.size();
    // unordered map optimization
    adjacencyList.reserve(numberOfEdges);
    inDegrees.reserve(numberOfEdges);
    outDegrees.reserve(numberOfEdges);

    // initializing unordered maps
    for (auto &pair: pairs) {
        adjacencyList[pair[0]].push_back(pair[1]);
        outDegrees[pair[0]]++;
        inDegrees[pair[1]]++;

        nodes.insert(pair[0]);
        nodes.insert(pair[1]);
    }

    // eulerian graph theorem validation
    int startNode = -1, finishNode = -1;
    // iterating through all the nodes
    for (auto &node: nodes)
        // existence of a single start node (in-degree(startNode) = out-degree(startNode) - 1)
        if (inDegrees[node] == outDegrees[node] - 1) {
            // when a cycle is set to be determined, the existence of any
            // node with unequal in-degrees and out-degrees is forbidden
            if (cycle)
                return {};

            // making sure there is at most one node with this property
            if (startNode == -1)
                startNode = node;
            else
                return {};
        }
        else if (outDegrees[node] == inDegrees[node] - 1) {
            // when a cycle is set to be determined, the existence of any
            // node with unequal in-degrees and out-degrees is forbidden
            if (cycle)
                return {};

            // making sure there is at most one node with this property
            if (finishNode == -1)
                finishNode = node;
            else
                return {};
        }
            // the only other allowed possibility is for the current node to be
            // inside the eulerian path (and implicitly inside the eulerian cycle)
        else if (outDegrees[node] != inDegrees[node])
            return {};

    // all nodes have equal in and out degrees
    if (startNode == -1 and finishNode == -1) {
        // making sure not to choose an isolated node
        for (auto &node: outDegrees)
            // the first node with a non-null out-degree is chosen
            // (it must be part of a connected component)
            if (node.second) {
                startNode = node.first;
                break;
            }
    }
        // both starting and finish nodes must exist at the same time
        // (the nodes with the special in-degrees and out-degrees)
    else if (startNode == -1 or finishNode == -1)
        return {};


    // initializing and determining the eulerian path
    vector<vector<int>> eulerianPath;
    HierholzerDFS(startNode, eulerianPath, adjacencyList, outDegrees);

    // the provided graph contains multiple connected
    // components, which means it cannot be eulerian
    if (eulerianPath.size() != pairs.size())
        return {};

    // since the push_back() method is used in the DFS traversal, the resulting vector must be reversed
    reverse(eulerianPath.begin(), eulerianPath.end());
    return eulerianPath;
}



int main() {
    return 0;
}