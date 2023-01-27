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


// b) Shortest Common Supersequence
// This problem can be solved based on the longest common subsequence, since
// the shortest common supersequence of the same two words must contain the
// most amount of shared letters by the intertwined subsequences of said
// supersequence, to reduce its length as much as possible. Then, we can think
// of the unshared letters of both words as sitting between the letters of the
// longest common subsequence. Luckily, the index of the 'gap' in which each
// letter sits is maintained in the matrix used when computing the longest
// common subsequence itself.
//
// Complexity:  O(M*N)
// - computing the longest common subsequence => O(M*N)
// - constructing the shortest common supersequence based on
// the already computed matrix => O(M+N), since all the letters
// of the two words are used in the worst case, when said words
// don't contain any common letters
string shortestCommonSupersequence(string str1, string str2) {
    // computing the longest common subsequence which the two strings will 'share'

    // storing text lengths for further use
    int textLength1 = str1.length(), textLength2 = str2.length();
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
            if (str1[i] == str2[j])
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

    string shortestSupersequence;           // variable holding the to-be constructed supersequence
    /* starting from the bottom-right corner and
       building the sequence from last to first */
    int i = textLength1, j = textLength2;
    /* at least one word must contain unadded letters for the insertion process to continue;
       the sequence construction stalls when both word's letter have been traversed */
    while (i > 0 or j > 0) {
        /* all the letters in 'str1' have been added, which means that those
           left in 'str2' should be inserted one at a time until completion */
        if (i == 0)
            shortestSupersequence.insert(0, 1, str2[j - 1]), j--;
            /* all the letters in 'str2' have been added, which means that those
               left in 'str1' should be inserted one at a time until completion */
        else if (j == 0)
            shortestSupersequence.insert(0, 1, str1[i - 1]), i--;
            // both words still contain unadded letters
        else {
            // a letter in the longest common subsequence is encountered, which
            // means it must be inserted in the front of the constructed string
            // and both indices must be decremented, since the added letter is
            // shared between them
            if (str1[i - 1] == str2[j - 1]) {
                shortestSupersequence.insert(0, 1, str1[i - 1]);
                i--;
                j--;
            }
                // when the letters found at the current indices in the two words
                // differ, it must mean that one or both of them belong between
                // the previously found letter in the longest common subsequence
                // and the next letter in the longest common subsequence (before
                // the first letter and after the last letter respectively);
                // the greater value of the word prefixes ending in the currently
                // processed letters in the corresponding subsequence matrix used
                // when generating the longest common subsequence should be chosen,
                // since it indicates that said letter stands behind a more recent
                // letter belonging to the longest common subsequence;
                // the value stored in the matrix can be thought of as the index
                // of the gap between consecutive longest common subsequence letters
                // to which the currently processed letters belong;
                // when the values in the subsequence matrix are equal, it does not
                // matter which letter is inserted first, since they both sit inside
                // the same longest-common-subsequence 'gap'
            else if (subsequenceMatrix[i][j - 1] < subsequenceMatrix[i - 1][j])
                shortestSupersequence.insert(0, 1, str1[i - 1]), i--;
            else
                shortestSupersequence.insert(0, 1, str2[j - 1]), j--;
        }
    }

    return shortestSupersequence;
}


int main() {
    return 0;
}