// N = number of nodes
// M = number of operations
// Path compression approach - O(M*log(N))
// - find(): worst-case O(logN)
// - union(): 2 * find() => O(logN)
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


int main() {
    ifstream input;
    ofstream output;
    input.open("disjoint.in");
    output.open("disjoint.out");

    int numberOfSets, numberOfOperations, operationType, firstNode, secondNode;
    input >> numberOfSets >> numberOfOperations;

    int isParentOf[numberOfSets], heights[numberOfSets];
    for (int i = 0; i < numberOfSets; i++)
        isParentOf[i] = -1, heights[i] = 1;

    for (int i = 0; i < numberOfOperations; i++) {
        input >> operationType >> firstNode >> secondNode;
        firstNode--, secondNode--;

        if (operationType == 2) {
            if (areInTheSameSet(firstNode, secondNode, isParentOf))
                output << "DA\n";
            else
                output << "NU\n";
        }
        else uniteSets(firstNode, secondNode, isParentOf, heights);
    }

    output.close();
    input.close();

    return 0;
}