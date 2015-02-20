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
#include <cassert>

using namespace std;

/*! a class for ballot information */

class Ballot {
public:

	//! a vector which holds votes
	/**
	 *	holds the indices of candidates in the order of which they were voted for
	 */
	vector<int> votes;

	//! current vote index
	/**
	 *	an int which points to the current candidate in votes this ballot is voting for
	 */
	int index;

	//! id of the ballot
	/**
	 *	an int which identifies this ballot in the vector containing all the ballots
	 */
	int id;

	/**
	 * default ballot constructor
	 */

	Ballot() : votes(), index(0), id(0){}

	/**
	 * constructor for taking a vector of int and an int for the id
	 * @param v votes in this ballot
	 * @param id id of this ballot
	 */

	Ballot(vector<int> v, int id) : votes(v), index(0), id(id){}

	/**
	 * get the candidate this ballot is voting for 
	 * @return the id of the candidate this ballot is voting for  
	 */

	int getVote();
};

/*! a class for candidate information */

class Candidate{
public:
	//! name of the candidate
	/**
	 *	string which contains the name of the candidate
	 */
	string name;

	//! vector of ballots
	/**
	 *	vector containing the IDs of ballots which voted for this candidate
	 */
	vector<int> ballots;

	//! a flag for if the candidate is running
	/**
	 *	a bool which is true if the candidate is still in the running, false if lost
	 */
	bool still_running;

	//! count of votes
	/**
	 *	an int which holds the number of ballots which voted for this candidate
	 */
	int votes;
	
	/**
	 * default candidate constructor
	 */

	Candidate() : name(""), ballots(), still_running(1), votes(0){}
	
	/**
	 * construct a Candidate object with name n
	 * @param n the name of the candidate
	 */
	Candidate(string n) : name(n), ballots(), still_running(1), votes(0){}
	
	/**
	 * value test constructor
	 * only used for testing
	 * @param n name of the candidate
	 * @param v number of votes
	 */

	Candidate(string n, int v) : name(n), ballots(), still_running(1), votes(v){} // for testing
	
	/**
	 * value test constructor
	 * only used for testing
	 * @param n name of the candidate
	 * @param v number of votes
	 * @param b the id of ballots voting for this candidate
	 */
	Candidate(string n, int v, vector<int> b) : name(n), ballots(b), still_running(1), votes(v) {} // for testing

	/**
	 * add ballot to list of ballots voting for this candidate
	 * @param id id of the ballot
	 */

	void addBallot(int id); 
};

// ------------
// voting_read
// ------------

/**
 * read one int from string s
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
 * solve the problem given from input stream r, outputs to outputstream w
 * @param r an istream
 * @param w an ostream
 */
void voting_solve (istream& r, ostream& w);

// -------------
// voting_readline
// -------------

/**
 * convert a ballot input into a vector<int>
 * @param s a const string
 * @return an int vector
 */
vector<int> voting_readline (const string& s);

// -------------
// count_votes
// -------------

/**
 * count votes for all candidates from ballots
 * @param candidates a vector of candidates
 * @param ballots a vector of ballots
 */
void count_votes(vector<Candidate>& candidates, vector<Ballot>& ballots);

// -------------
// check_votes
// -------------

/**
 * check terminating conditions: ties, majority winner
 * @param tied_candidates a vector of candidates that are tied
 * @param candidates a vector of candidates
 * @param finished a bool indiciating whether or not a terminating condition was reached
 * @param tying a bool indicating whether or not there is a tie
 * @param ballot_size number of votes per ballot
 * @param winner a vector of winning candidate indicies
 */
void check_votes(vector<int>& tied_candidates, vector<Candidate>& candidates,
	bool& finished, bool& tying, int ballot_size, vector<string>& winner);

// ------------
// update_state
// ------------

/**
 * reassign votes from ballots who voted for a losing candidate
 * @param losers a vector of ints that contain this iteration's losers
 * @param candidates vector of Candidate objects
 * @param ballots vector of Ballot objects
 */
void update_state(vector<int>& losers, vector<Candidate>& candidates, vector<Ballot>& ballots);

// -----------
// voting_eval
// -----------

/**
 * evaluates winner(s) given case
 * @param candidates a vector of Candidate object
 * @param ballots a vector of Ballot objects
 * return a vector<string> containing the results of the case
 */
vector<string> voting_eval(vector<Candidate>& candidates, vector<Ballot>& ballots);

// -----------
// find_start
// -----------

/**
 * find the starting index of the first candidate to be considered in find_losers
 * also adds candidates with zero votes to zero_votes vector
 * @param candidates a vector of candidates
 * @param zero_votes a vector of indicies to candidates with zero votes
 * @return the index of the first running candidate with non-zero votes
 */
int find_start(vector<Candidate>& candidates, vector<int>& zero_votes);

// -----------
// find_losers
// -----------

/**
 * find losing candidates for the current state of election
 * @param candidates a vector of candidates
 * @param zero_votes a vector of indicies to candidates with zero votes
 * @param losers a vector of losing candidates
 * @param start the index of the first running candidate with non-zero votes
 */
void find_losers(vector<Candidate>& candidates, vector<int>& zero_votes,
vector<int>& losers, int start);

#endif // Voting_h

