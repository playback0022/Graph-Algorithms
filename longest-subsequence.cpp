#include <bits/stdc++.h>
using namespace std;

// The intuitive explanation of the recursive equation can be
// found in the comments bellow.
//
// Complexity: Matrix traversal => O(M*N)
int longestCommonSubsequence(string text1, string text2) {
    // storing text lengths for further use
    int textLength1 = text1.length(), textLength2 = text2.length();
    /* matrix used for storing subproblem solutions in iterative approach;
       it contains an extra row and an extra column, which represent the
       empty string of each string */
    int subsequenceMatrix[textLength1 + 1][textLength2 + 1];

    /* initializing matrix with the base case -> 'text1' reduced to
       the empty string;
       the longest common subsequence of the empty string and any
       substring of 'text2' is the empty string itself;
       based on this observation, the first column of the matrix
       is filled with 0s */
    for (int i = 0; i <= textLength1; i++)
        subsequenceMatrix[i][0] = 0;

    /* initializing matrix with the base case -> 'text2' reduced to
       the empty string;
       the longest common subsequence of the empty string and any
       substring of 'text1' is the empty string itself;
       based on this observation, the first row of the matrix is
       also filled with 0s */
    for (int i = 0; i <= textLength2; i++)
        subsequenceMatrix[0][i] = 0;

    /* according to the recursive equation, the longest subsequence
       of every prefix-combination of the two words will be computed,
       starting from the base case outline above */
    for (int i = 0; i < textLength1; i++)
        for (int j = 0; j < textLength2; j++)
            /* the last letters of the currently visited word prefixes
               coincide, which means that their longest subsequence is
               that of 'the word prefixes without two said ending
               letters' + 1 (the two ending letters being identical) */
            if (text1[i] == text2[j])
                subsequenceMatrix[i + 1][j + 1] = 1 + subsequenceMatrix[i][j];
                /* when the ending letters do not coincide, the longest subsequence
                   of the current word prefixes is the maximum of the values held
                   by the two prefixes with each ending letter removed at a time;
                   this makes sense intuitively, as the currently longest must be
                   somehow maintained from past prefixes, given that the current
                   endings don't match;
                   it can be viewed as selectively choosing which ending letter to
                   view as being 'appended' to the one of the prefixes, without
                   changing the value of the previously computed value; */
            else
                subsequenceMatrix[i + 1][j + 1] = max(subsequenceMatrix[i + 1][j], subsequenceMatrix[i][j + 1]);

    // the sought after value corresponds to the longest subsequence
    // of the prefixes which represent the two words themselves
    return subsequenceMatrix[textLength1][textLength2];
}

int main() {
    return 0;
}