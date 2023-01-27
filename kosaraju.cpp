// O(m + n) - two DFS traversals
#include <bits/stdc++.h>

using namespace std;


void firstDFS(int node, vector<int> *adjacencyList, bool *isVisited, stack<int> *stack) {
    isVisited[node] = true;

    for (auto adjacentNode: adjacencyList[node])
        if (not isVisited[adjacentNode])
            firstDFS(adjacentNode, adjacencyList, isVisited, stack);

    // pushing all encountered nodes to the stack, in the order of their time of completion
    stack->push(node);
}


void secondDFS(int node, vector<int> *adjacencyList, bool *isVisited, string *toBeWritten) {
    isVisited[node] = true;

    // appending each node belonging to the current SCC to its corresponding line
    toBeWritten->append(to_string(node + 1));
    toBeWritten->append(" ");

    for (auto adjacentNode: adjacencyList[node])
        if (not isVisited[adjacentNode])
            secondDFS(adjacentNode, adjacencyList, isVisited, toBeWritten);
}


// wrapper and driver function around the two DFS traversal functions
void writeSCCs(int numberOfNodes, vector<int> *adjacencyList, vector<int> *transposedAdjacencyList) {
    bool isVisited[numberOfNodes];  // needed to mark nodes as visited within both DFS traversals
    for (int node = 0; node < numberOfNodes; node++)
        isVisited[node] = false;

    stack<int> topologicalOrderingOfSCCs;   // used for storing the completion time of nodes in the first DFS traversal

    // first DFS, used to find the topological ordering of the underlying graph's SCCs
    for (int node = 0; node < numberOfNodes; node++)
        if (not isVisited[node])
            firstDFS(node, adjacencyList, isVisited, &topologicalOrderingOfSCCs);

    // marking all nodes as not visited, in preparation for the second DFS
    for (int node = 0; node < numberOfNodes; node++)
        isVisited[node] = false;

    string toBeWritten;     // used as a buffer for storing what will be written in the output file
    int numberOfSCCs = 0;

    // second DFS, used to find the SCCs (in reverse topological order of the transposed graph's SCCs);
    // the reverse topological ordering of the transposed graph's SCCs is the topological ordering of
    // the original graph's SCCs.
    // it is guaranteed that the top of our stack is one of the nodes in the first SCC within the original
    // graph's SCC topological-ordering, therefore it will be part of the last SCC in the SCC topological
    // ordering of the transposed graph.
    // this property holds for the remaining last and second-to-last SCCs after the preceding removal of
    // all nodes belonging to the current last SCC from the stack.
    while (not topologicalOrderingOfSCCs.empty()) {
        if (not isVisited[topologicalOrderingOfSCCs.top()]) {
            secondDFS(topologicalOrderingOfSCCs.top(), transposedAdjacencyList, isVisited, &toBeWritten);
            numberOfSCCs++;     // new SCC discovered
            toBeWritten.append("\n");
        }

        topologicalOrderingOfSCCs.pop();    // all visited nodes have to be popped, in order for said principle to hold
    }

    ofstream output;
    output.open("ctc.out");
    output << numberOfSCCs << '\n' << toBeWritten;
    output.close();
}


int main() {
    int numberOfNodes, numberOfEdges, outNode, inNode;
    ifstream input;

    input.open("ctc.in");

    input >> numberOfNodes >> numberOfEdges;
    vector<int> adjacencyList[numberOfNodes];
    vector<int> transposedAdjacencyList[numberOfNodes];     // needed to store the adjacency list of the transposed graph

    for (int i = 0; i < numberOfEdges; i++) {
        input >> outNode >> inNode;
        adjacencyList[outNode - 1].push_back(inNode - 1);
        transposedAdjacencyList[inNode - 1].push_back(outNode - 1);
    }

    input.close();

    writeSCCs(numberOfNodes, adjacencyList, transposedAdjacencyList);

    return 0;
}