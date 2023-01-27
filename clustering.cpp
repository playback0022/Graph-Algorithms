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


int computeRelationshipCostBetweenElements() {
    return 0;
}


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


// modified Kruskal's MST function
int splitIntoClasses(const vector<string> &words, int *wordClasses, int desiredNumberOfClasses) {
    if (desiredNumberOfClasses == 1) {
        wordClasses[0] = -1;
        for (int i = 1; i < words.size(); i++)
            wordClasses[i] = 0;

        return -1;
    }

    int numberOfWords = words.size(), i, j;
    // since the edit distance between two words can be perceived as
    // the weight of the edge connecting the two words' corresponding
    // nodes, a vector of vectors of length three is needed to store
    // the resulting graph's edges in a format best-suited for
    // Kruskal's algorithm
    vector<vector<int>> wordsAndDistances;

    // the edit distance between two identical words is 0
    // and the edit distance between wordI and wordJ is the
    // same as that between wordJ and wordI
    for (i = 0; i < numberOfWords; i++)
        for (j = i + 1; j < numberOfWords; j++)
            wordsAndDistances.push_back({i, j, computeRelationshipCostBetweenElements(words[i], words[j])});

    // height array needed for rank-based union of two trees(classes);
    // Kruskal's will stop when 'desiredNumberOfClasses' is reached, which
    // means a counter is necessary to keep track of the number of trees(classes)
    int ranks[numberOfWords], currentNumberOfClasses = numberOfWords;
    for (i = 0; i < numberOfWords; i++) {
        ranks[i] = 1;
    }

    // classical Kruskal - sorting the vector in ascending order of the edit distances
    sort(wordsAndDistances.begin(), wordsAndDistances.end(),
         [](const vector<int> &firstDistance, const vector<int> &secondDistance) {
             return firstDistance[2] < secondDistance[2];
         });

    // iterating through the sorted edge vector
    for (i = 0; i < wordsAndDistances.size(); i++)
        // Kruskal's condition (only nodes in distinct trees will get their trees united)
        if (not areInTheSameSet(wordsAndDistances[i][0], wordsAndDistances[i][1], wordClasses)) {
            // Kruskal's halts when the 'desiredNumberOfClasses' is achieved
            if (currentNumberOfClasses == desiredNumberOfClasses)
                break;

            uniteSets(wordsAndDistances[i][0], wordsAndDistances[i][1], wordClasses, ranks);
            currentNumberOfClasses--;   // two trees(classes) were merged
        }

    // in order to find the separation distance between the 'k' classes,
    // we have to iterate through the rest of edge vector until an edge
    // connecting nodes in different trees is found - its weight is the
    // desired separation distance
    for (; i < wordsAndDistances.size(); i++)
        if (not areInTheSameSet(wordsAndDistances[i][0], wordsAndDistances[i][1], wordClasses)) {
            i = wordsAndDistances[i][2];
            break;
        }

    // in order to guarantee the tree structure outlined bellow, the find
    // function must be called upon every node in every tree (else, it is
    // possible that some nodes were only processed by 'find' once, after
    // which some union operations occurred based on nodes on different
    // branches, which left our initial nodes not directly attached to
    // the new root)
    for (j = 0; j < numberOfWords; j++)
        pathCompressionFind(j, wordClasses);

    return i;
}


int main() {
    // a vector will be used to store all
    // the words contained in the source file
    vector<string> words;
    // used for reading from file and storing in the vector
    string word;
    // the 'k' desired number of classes
    int numberOfClasses;

    ifstream input("cuvinte.in");
    while (input >> word)
        words.push_back(word);
    input.close();

    cout << "Enter the number of classes: ";
    cin >> numberOfClasses;


    // it is impossible to split the words
    // in more classes than there are words
    // or in less than one class
    if (numberOfClasses > words.size() or numberOfClasses<1) {
        cout << "Number of classes must be between one and the total number of words!\n";
        return -1;
    }


    // array used to store the parent-child relationship between the word-containing nodes
    int wordClasses[words.size()];
    for (int i = 0; i < words.size(); i++)
        wordClasses[i] = -1;    // at first, each node is isolated

    // calling the function will split the words into 'k' trees and
    // return the minimum separation distance between two classes
    int separationDistance = splitIntoClasses(words, wordClasses, numberOfClasses);
    // printing the classes to the screen;
    // after the function call, all trees will be 2-levels high,
    // with one root node to which the rest of the nodes in its
    // class are attached:
    // - wordClasses[rootOfClass_I] = -1
    // - wordClasses[node] = rootOfClass_I, for 'node' in Class_I
    for (int i = 0; i < words.size(); i++)
        if (wordClasses[i] == -1) {
            cout << words[i];
            for (int j = 0; j < words.size(); j++)
                if (wordClasses[j] == i)
                    cout << ' ' << words[j];

            cout << '\n';
        }

    cout << separationDistance;

    return 0;
}