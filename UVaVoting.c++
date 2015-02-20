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
#include <sstream>  // istringstream


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
		assert(index >= 0);
		assert(votes[index] >= 0);

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
		assert(votes >= 0);
		assert(id >= 0);

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

using namespace std;

int voting_readnum(const string& s) {
	istringstream sin(s);
	int i;
	sin >> i;
	return i;
}

void voting_readcandidates(istream& r, vector<Candidate>& candidates) {

	assert (candidates.size() == 0);
	
	// reading in number of candidates
	string s;
	getline(r, s);
	istringstream sin(s);
	int candidates_count;
	sin >> candidates_count;

	candidates.resize(candidates_count);

	// get string array of candidates
	int count = 0;
	while (count < candidates_count) {
		getline(r, s);
		candidates[count].name = s;			// not sure if copied by value
		count ++;
	}
}

void voting_readballots (istream& r, vector<Ballot>& ballots) {

	string s;
	istringstream sin(s);

	int i = 0;
	while (getline(r, s) && s != "") {
		Ballot b(voting_readline(s), i);
		ballots.push_back(b);
		i++;
	}
}

vector<int> voting_readline (const string& s) {
	istringstream sin(s);
	int i;

	vector<int> v;

	while (sin >> i) {
		v.push_back(i);
	}

	return v;
}

void count_votes(vector<Candidate>& candidates, vector<Ballot>& ballots){
	assert(candidates.size() > 0);

	for (Ballot b: ballots) {
		int cand = b.getVote(); 
		candidates[cand].addBallot(b.id);
	}
}

void check_votes(vector<int>& tied_candidates, vector<Candidate>& candidates, 
				bool& finished, bool& tying, int ballot_size, vector<string>& winner) {
	assert(tying == 1);
	assert(finished == 0);

	int votes_needed = ballot_size / 2 + 1;
	
	int tie_votes = 0;

	for(unsigned int i = 0; i < candidates.size(); i++){
		assert(finished == 0);

		if (candidates[i].still_running) {
			int current_votes = candidates[i].votes;

			if(current_votes >= votes_needed){ 								// majority winner
				winner.push_back(candidates[i].name);
				tying = 0;
				finished = 1;
				break;
			}
			else if (tying && (current_votes == 0 || tie_votes == 0 		// tying
					|| tie_votes == current_votes)) {
				if (tie_votes == 0)
					tie_votes = current_votes;
				if (current_votes != 0)
					tied_candidates.push_back(i);
			}
			else{
				tying = 0;
				tied_candidates.clear();
			}
		}
	}

	if(tying == 1){
		assert(finished == 0);
		finished = 1;
	}
}

int find_start(vector<Candidate>& candidates, vector<int>& zero_votes) {

	int start = 0;
	while (candidates[start].still_running == false || candidates[start].votes == 0) {
		if (candidates[start].still_running == true && candidates[start].votes == 0)
			zero_votes.push_back(start);
		start++;

		assert(start < (int) candidates.size());
	}

	assert(start >= 0);
	return start;
}

void find_losers(vector<Candidate>& candidates, vector<int>& zero_votes, vector<int>& losers, int start) {
	assert (start >= 0);
	assert (candidates.size() > 0);

	losers.push_back(start);
	int min = candidates[start].votes;

	for (unsigned int x = start + 1; x < candidates.size(); x++) {
		if (candidates[x].votes == 0) 
			zero_votes.push_back(x);
		else if (candidates[x].votes < min) {
			losers.clear();
			losers.push_back(x);
			min = candidates[x].votes;
		}
		else if (candidates[x].votes == min)
			losers.push_back(x);
	}

	for (int b: zero_votes)
		losers.push_back(b);
}

void update_state (vector<int>& losers, vector<Candidate>& candidates, vector<Ballot>& ballots) {
	assert (candidates.size() > 0);

	for(int a : losers) {
		candidates[a].still_running = false;
	}

	for(int x: losers) {
		for(int y: candidates[x].ballots) {
			while (candidates[ballots[y].getVote()].still_running == false) { 
				ballots[y].index++;
			} 

			candidates[ballots[y].getVote()].addBallot(y);
		}

		candidates[x].votes = 0;
		candidates[x].ballots.clear();
	}

	// for (unsigned int x = 0; x < candidates.size(); x++) {
	// 	candidates[x].votes = 0;
	// 	candidates[x].ballots.clear();
	// }
}

vector<string> voting_eval(vector<Candidate>& candidates, vector<Ballot>& ballots){
	assert (candidates.size() > 0);

	vector<int> ballot_index(ballots.size());

	bool finished = 0;
	vector<string> winner;

	count_votes(candidates, ballots);
	
	while(!finished){

		vector<int> tied_candidates;
		bool tying = 1;

		check_votes(tied_candidates, candidates, finished, tying, ballots.size(), winner);

		if (tying == 1) {
			for (unsigned int x = 0; x < tied_candidates.size(); x++)
					winner.push_back(candidates[tied_candidates[x]].name);
		}

		if (!finished) {													// increment ballot 
			vector<int> losers;	
			vector<int> zero_votes;											// when no winner

			int start = find_start(candidates, zero_votes);

			assert(start < (int) candidates.size());
			assert(start >= 0);

			find_losers(candidates, zero_votes, losers, start);

			update_state(losers, candidates, ballots);
		}
	}

	return winner;
}

void voting_solve(istream& r, ostream& w) {

	// getting number of cases, skips a line
	int cases;
	int count = 0;
	string s;
	getline(r, s);
	cases = voting_readnum(s);
	getline(r, s);

	// Note: Edge cases of 0 or negative numbers

	// iterating according to number of cases


	while (count < cases) {
		vector<Candidate> candidates;
		voting_readcandidates(r, candidates);
		vector<Ballot> ballots;
		voting_readballots(r, ballots);
		if(ballots.size() == 0){
			for(Candidate c : candidates){
				w << c.name << endl;
			}
		}
		else{
			vector<string> result = voting_eval(candidates, ballots); 
			for (unsigned int x = 0; x < result.size(); x++) {
				w << result[x] << endl;	
			}
		}
		if (count != cases -1)
			w << endl;
		count++;
	}
}



// -------------------------------
// projects/collatz/RunCollatz.c++
// Copyright (C) 2015
// Glenn P. Downing
// -------------------------------

// -------
// defines
// -------

#ifdef ONLINE_JUDGE
    #define NDEBUG
#endif

// --------
// includes
// --------

// ----
// main
// ----

int main () {
    using namespace std;
    voting_solve(cin, cout);
    return 0;}
