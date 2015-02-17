// --------------------------
// projects/voting/voting.h
// Copyright (C) 2015
// Glenn P. Downing
// --------------------------

#ifndef Voting_h
#define Voting_h

// --------
// includes
// --------

#include <iostream> // istream, ostream
#include <string>   // string
#include <utility>  // pair
#include <vector>
#include "Candidate.h"

using namespace std;

// ------------
// voting_read
// ------------

/**
 * read two ints
 * @param s a string
 * @return a pair of ints, representing the beginning and end of a range, [i, j]
 */
int voting_readnum (const string& s);

// ------------
// voting_readcandidates
// ------------

/**
 * read in candidates from input stream 
 * @param r istream
 * @return a vector containing the names of the candidates
 */
vector<Candidate> voting_readcandidates(istream& r);

// ------------
// voting_readballots
// ------------

/**
 * read in ballots from input stream 
 * @param r istream
 * @return a vector containing the ballots
 */
vector<Ballot> voting_readballots(istream& r);


// -------------
// voting_solve
// -------------

/**
 * @param r an istream
 * @param w an ostream
 */
void voting_solve (istream& r, ostream& w);


// -------------
// voting_eval
// -------------

/**
 * @param candidates a vector of candidate names
 * @param ballots a 2d vector of ballots
 */
vector<string> voting_eval(vector<Candidate> candidates, vector<Ballot> ballots);


// -------------
// voting_readline
// -------------

/**
 * @param s a const string
 * @return an int vector
 */
vector<int> voting_readline (const string& s);

#endif // Voting_h
