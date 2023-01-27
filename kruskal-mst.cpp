//      The idea behind my algorithm involves computing the Edit (Levenshtein)
// distance between every unordered pair of distinct words and treating it as
// a weighted. This way, every word can be seen as a node, which can be identified
// based on the index of the underlying word in the vector of words.
//      We can now view the problem as having to split a weighted, undirected
// simple graph into k minimum spanning connected components (the connected components
// have to be minimum-spanning in order to maximise the weight of the remaining
// edges connecting two different components - if any other edge was used in
// the structure of the components, the lower-weighted edge it replaced could
// become the new separation distance). Kruskal's algorithm is the perfect fit
// for this objective, since we can simply halt the execution of the algorithm
// when the nodes have been united into k components(trees), guaranteeing that
// any other k-split would have a total weight at most equal to that of the split
// computed by Kruskal's, which means that the separation distance will certainly
// be the same.
//      When the execution halts, the next weight of an edge connecting nodes in
// different components will be stored, as it represents the sought after
// separation distance. The special case of k=1 is also treated: the separation
// distance will be considered '-1', since there is no other class that the only
// class could be separated from.
//
// Notations:
// - n = number of words
// - m = maximum length of word
// - k = number of classes
//
// Intermediary operations:
// - computing the edit distance: n(n-1)/2 edges * m letters * m letters => O(n^2*m^2)
// - Kruskal's algorithm: n(n-1)/2 * log(n(n-1)/2) sorting + n(n-1)/2 * log(n) union and find operations => O(n^2*log(n))
// Therefore, the total complexity is:
// - O(n^2*log(n)), m <= sqrt(log(n))
// - O(n^2*m^2), m > sqrt(log(n))
#include <bits/stdc++.h>

using namespace std;



// recursive find function, which attaches each encountered node in
// the tree to the root of the tree, reducing the complexity to O(logn)
int pathCompressionFind(int node, int *isParentOf) {
    if (isParentOf[node] == -1)   // base case
        return node;

    // each node will get attached to the root node, which is cascaded
    // all the way up through the recursive call chain, by updating the
    // 'isParentOf' array
    isParentOf[node] = pathCompressionFind(isParentOf[node], isParentOf);
    return isParentOf[node];
}


bool areInTheSameSet(int firstNode, int secondNode, int *isParentOf) {
    firstNode = pathCompressionFind(firstNode, isParentOf);
    secondNode = pathCompressionFind(secondNode, isParentOf);

    // the root of both nodes' trees coincide,
    // which means they are part of the same tree
    if (firstNode == secondNode)
        return true;

    return false;
}


void uniteSets(int firstNode, int secondNode, int *isParentOf, int *heights) {
    // uniting trees doesn't make sense unless they are distinct
    if (areInTheSameSet(firstNode, secondNode, isParentOf))
        return;

    // determining the root of both nodes' trees
    firstNode = pathCompressionFind(firstNode, isParentOf);
    secondNode = pathCompressionFind(secondNode, isParentOf);

    // 'rank'-based union is preferred, since it is more efficient
    // to minimize the height of the resulting tree, in order to
    // reduce the length of the path the leaf nodes have to take
    // when a 'find' operation is called upon them;
    // therefore, the 'shorter' tree will be attached to the
    // 'longer' one, maintaining the overall height of the resulting
    // tree to that of the 'longer' initial one, which is optimal
    // (otherwise, the resulting tree would be 1 level 'longer' each time);
    if (heights[firstNode] < heights[secondNode])
        isParentOf[firstNode] = secondNode;
    else {
        isParentOf[secondNode] = firstNode;

        // when both trees have the same height, this optimization
        // doesn't work, because regardless of the chosen initial
        // tree, the resulting one will be 1 level 'longer' then
        // both initial trees, which means the choice is arbitrary.
        if (heights[secondNode] == heights[firstNode])
            heights[firstNode]++;   // the height of the root of the resulting tree must be updated
    }
}


int kruskalMST(int numberOfNodes, vector<vector<int>>& edgeList, int& numberOfEdgesUsed, int *parentOf) {
    int i, totalCost = 0;

    // height array needed for rank-based union of two trees(classes);
    int ranks[numberOfNodes];
    for (i = 0; i < numberOfNodes; i++)
        ranks[i] = 1;

    // sorting the vector in ascending order of the cost of the edges
    sort(edgeList.begin(), edgeList.end(),
         [](const vector<int> &firstDistance, const vector<int> &secondDistance) {
             return firstDistance[2] < secondDistance[2];
         });

    // iterating through the sorted edge vector
    for (i = 0; i < edgeList.size(); i++)
        // Kruskal's condition (only nodes in distinct trees will get their trees united)
        if (not areInTheSameSet(edgeList[i][0], edgeList[i][1], parentOf)) {
            uniteSets(edgeList[i][0], edgeList[i][1], parentOf, ranks);
            totalCost += edgeList[i][2];
            numberOfEdgesUsed++;
        }

    return totalCost;
}


int main() {
    ifstream input("apm.in");

    int numberOfNodes, numberOfEdges;
    input>>numberOfNodes>>numberOfEdges;

    vector<vector<int>> edgeList;
    int firstNode, secondNode, cost;
    for (int i=0; i< numberOfEdges; i++) {
        input>>firstNode>>secondNode>>cost;
        firstNode--; secondNode--;
        edgeList.push_back({firstNode, secondNode, cost});
    }

    input.close();

    ofstream output("apm.out");
    int numberOfEdgesUsed = 0;
    int parentOf[numberOfNodes];
    for(int i = 0; i < numberOfNodes; i++)
        parentOf[i] = -1;

    output << kruskalMST(numberOfNodes, edgeList, numberOfEdgesUsed, parentOf) << '\n' << numberOfEdgesUsed << '\n';

    for (int i = 1; i < numberOfNodes; i++) {
        output << i+1 << " " << parentOf[i]+1 << "\n";
    }

    output.close();

    return 0;
}