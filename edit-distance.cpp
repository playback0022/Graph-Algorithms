// Complexity: O(M*N), where M and N are the lengths of each word
#include <bits/stdc++.h>

using namespace std;


// Levenshtein Distance between two words - Optimized for memory
int getEditDistance(const string &firstWord, const string &secondWord) {
    // a memoization matrix of two rows and 'length of firstWord + 1'
    // columns will be used, in the place of the matrix using 'length
    // of secondWord + 1' rows, since at any given point during the
    // execution, only two consecutive rows are required for the
    // current computation (the current and previous rows, respectively);
    // using modulo arithmetic, the two rows will switch places based on
    // the parity of the index used in the traversal of the 'secondWord'
    // (even - row #0 is the 'current' row and row #1 is the 'previous'
    // one, odd - the opposite)
    int rows = secondWord.length() + 1, columns = firstWord.length() + 1;
    int memoizationTable[2][columns];

    // the Levenshtein distance between all the prefixes of 'firstWord'
    // to all the prefixes of 'secondWord' can be reduced to two base
    // cases and two general cases:
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < columns; j++) {
            // first base case - going from all the prefixes of 'firstWord'
            // to the empty string requires 'j' deletions (where 'j' is the
            // length of the prefix)
            if (i == 0)
                memoizationTable[0][j] = j;

                // second base case - going from the empty string to all the
                // prefixes of 'secondWord' requires 'i' additions (where 'i'
                // is the length of the prefix)
            else if (j == 0)
                memoizationTable[i % 2][0] = i;

                // when the letters match, the distance is the same as that which
                // took 'firstWord[(i - 1) % 2]' to 'secondWord[j - 1]';
                // 'j-1' and 'i-1' necessary, to compensate for row and column '0' - the empty string
            else if (firstWord[j-1] == secondWord[i-1])
                memoizationTable[i%2][j] = memoizationTable[(i - 1) % 2][j - 1];

                // letters do not match
            else
                // the edit distance is, therefore, 1 operation (deletion, insertion, replacement)
                // + the minimum distance to the previous cases:
                memoizationTable[i%2][j] = 1 + min(memoizationTable[(i - 1) % 2][j - 1],    // replacement
                                                   min(memoizationTable[i % 2][j - 1],     // insertion
                                                       memoizationTable[(i - 1) % 2][j])); // deletion
        }

    // the minimum edit distance will be found on the
    // '(length of secondWord) % 2' row and 'length of firstWord' column
    return memoizationTable[(rows - 1) % 2][columns - 1];
}


//// Levenshtein Distance between two words - Unoptimized for memory
//int getEditDistance(const string &firstWord, const string &secondWord) {
//    // a memoization matrix of 'length of secondWord + 1' rows and
//    // 'length of firstWord + 1' columns will be used, in order to
//    // memorize intermediary results that would be computed multiple
//    // times in the naive approach of using an unaltered recursive
//    // function
//    int rows = secondWord.length() + 1, columns = firstWord.length() + 1;
//    int memoizationTable[rows][columns];
//
//    // the Levenshtein distance between all the prefixes of 'firstWord'
//    // to all the prefixes of 'secondWord' can be reduced to two base
//    // cases and two general cases:
//    for (int i = 0; i < rows; i++)
//        for (int j = 0; j < columns; j++) {
//            // first base case - going from all the prefixes of 'firstWord'
//            // to the empty string requires 'j' deletions (where 'j' is the
//            // length of the prefix)
//            if (i == 0)
//                memoizationTable[0][j] = j;
//
//                // second base case - going from the empty string to all the
//                // prefixes of 'secondWord' requires 'i' additions (where 'i'
//                // is the length of the prefix)
//            else if (j == 0)
//                memoizationTable[i][0] = i;
//
//                // when the letters match, the distance is the same as that which
//                // took 'firstWord[i - 1]' to 'secondWord[j - 1]';
//                // 'j-1' and 'i-1' necessary, to compensate for row and column '0' - the empty string
//            else if (firstWord[j-1] == secondWord[i-1])
//                memoizationTable[i][j] = memoizationTable[i - 1][j - 1];
//
//                // letters do not match
//            else
//                // the edit distance is, therefore, 1 operation (deletion, insertion, replacement)
//                // + the minimum distance to the previous cases:
//                memoizationTable[i][j] = 1 + min(memoizationTable[i - 1][j - 1],    // replacement
//                                                 min(memoizationTable[i][j - 1],    // insertion
//                                                     memoizationTable[i - 1][j]));  // deletion
//        }
//
//    // the minimum edit distance will be found on the
//    // 'length of secondWord' row and 'length of firstWord' column
//    // (distance between the whole firstWord to the whole secondWord)
//    return memoizationTable[rows - 1][columns - 1];
//}


int main() {
    string first = "examen", second = "restanta";
    cout<<getEditDistance(first, second);
    return 0;
}