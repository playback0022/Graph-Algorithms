// O(m+n) - each edge and node gets visited and marked only once
#include <bits/stdc++.h>
using namespace std;


bool getBipartitions(int numberOfNodes, const vector<vector<int>> &adjacencyList, vector<vector<int>> &partitions) {
    // used to 'color' or mark each node as part of one of the two partitions
    int belongsTo[numberOfNodes];
    for (int i = 0; i < numberOfNodes; i++)
        belongsTo[i] = -1;

    queue<int> q;  // used for BFS

    // iterating through all possible nodes, to ensure all connected components where searched through.
    // after each iteration, all nodes belonging to the same component as the chosen root node 'node'
    // will have been marked accordingly, therefore the first next unmarked node will be part of
    // another graph component and a new search will begin.
    int currentNode;
    for (int node = 0; node < numberOfNodes; node++)
        // 'node' is not marked, therefore it is part of a new graph component
        if (belongsTo[node] == -1) {
            q.push(node);       // pushing 'node' in the queue
            belongsTo[node] = 1;   // and marking it accordingly

            // BFS begins
            while (not q.empty()) {
                currentNode = q.front();
                q.pop();

                // checking every node that connects the current node to it through an edge
                for (auto &adjacentNode: adjacencyList[currentNode])
                    // both nodes belong to the same partition, which means the given graph is not bipartite
                    if (belongsTo[currentNode] == belongsTo[adjacentNode])
                        return false;
                        // node is not marked
                    else if (belongsTo[adjacentNode] == -1) {
                        // based on the partition to which the 'currentNode' belongs,
                        // the child 'node' is assigned the other partition
                        if (belongsTo[currentNode] == 1)
                            belongsTo[adjacentNode] = 0;
                        else
                            belongsTo[adjacentNode] = 1;

                        q.push(adjacentNode);
                    }
            }
        }

    // pushing the first partition
    partitions.emplace_back(vector<int>());
    // pushing the second partition
    partitions.emplace_back(vector<int>());

    for (int node = 0; node < numberOfNodes; node++)
        if (belongsTo[node] == 0)
            partitions[0].push_back(node);
        else
            partitions[1].push_back(node);

    return true;
}


int main() {
    return 0;
}