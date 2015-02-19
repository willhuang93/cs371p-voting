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

using namespace std;

class Ballot {
public:
	vector<int> votes;
	int index;
	int id;

	Ballot() : votes(), index(0), id(0){}
	Ballot(vector<int> v, int id) : votes(v), index(0), id(id){}
	Ballot(vector<int> v) : votes(v), index(0), id(0){}

	int getVote()
	{
		return votes[index] - 1; 
	}
};

class Candidate{
public:
	string name;
	vector<int> ballots;
	bool still_running;
	int votes;

	Candidate() : name(""), ballots(), still_running(1), votes(0){}
	Candidate(string n) : name(n), ballots(), still_running(1), votes(0){}
	Candidate(string n, int v) : name(n), ballots(), still_running(1), votes(v){} // for testing
	Candidate(string n, int v, vector<int> b) : name(n), ballots(b), still_running(1), votes(v) {} // for testing


	void addBallot(int id) {
		votes++;
		ballots.push_back(id);
	}
};

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
 * @param candidates a vector to be filled with candidates
 */
void voting_readcandidates(istream& r, vector<Candidate>& candidates);

// ------------
// voting_readballots
// ------------

/**
 * read in ballots from input stream 
 * @param r istream
 * @param candidates a vector to be filled with ballots
 */
void voting_readballots(istream& r, vector<Ballot>& ballots);


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
vector<string> voting_eval(vector<Candidate>& candidates, vector<Ballot>& ballots);


// -------------
// voting_readline
// -------------

/**
 * @param s a const string
 * @return an int vector
 */
vector<int> voting_readline (const string& s);

// -------------
// count_votes
// -------------

/**
 * @param candidates a vector of candidates
 * @param ballots a vector of ballots
 */
void count_votes(vector<Candidate>& candidates, vector<Ballot>& ballots);

// -------------
// check_votes
// -------------

/**
 * @param tied_candidates a vector of candidates that are tied
 * @param candidates a vector of candidates
 * @param finished a bool indiciating whether or not a terminating condition was reached
 * @param tying a bool indicating whether or not there is a tie
 * @param ballot_size number of votes per ballot
 * @param winner a vector of winning candidate indicies
 */
void check_votes(vector<int>& tied_candidates, vector<Candidate>& candidates,
	bool& finished, bool& tying, int ballot_size, vector<string>& winner);

/**
 *
 * @param losers a vector of ints that contain this iteration's losers
 * @param candidates vector of Candidate objects
 * @param ballots vector of Ballot objects
 */
void update_state(vector<int>& losers, vector<Candidate>& candidates, vector<Ballot>& ballots);

/**
 * @param candidates a vector of Candidate object
 * @param ballots a vector of Ballot objects
 * return a vector<string> containing the results of the case
 */
vector<string> voting_eval(vector<Candidate>& candidates, vector<Ballot>& ballots);

/**
 * @param candidates a vector of candidates
 * @param zero_votes a vector of indicies to candidates with zero votes
 * @return the index of the first running candidate with non-zero votes
 */
int find_start(vector<Candidate>& candidates, vector<int>& zero_votes);

/**
 * @param candidates a vector of candidates
 * @param zero_votes a vector of indicies to candidates with zero votes
 * @param losers a vector of losing candidates
 * @param start the index of the first running candidate with non-zero votes
 */
void find_losers(vector<Candidate>& candidates, vector<int>& zero_votes,
vector<int>& losers, int start);

#endif // Voting_h

