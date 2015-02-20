#include <cassert>  // assert
#include <iostream> // endl, istream, ostream
#include <sstream>  // istringstream
#include <string>   // getline, string
#include <utility>  // make_pair, pair
#include <vector>

#include "Voting.h"

using namespace std;

// class access functions

// Candidate class helper
void Candidate::addBallot(int id) {
		assert(votes >= 0);
		assert(id >= 0);

		votes++;
		ballots.push_back(id);
}

// Ballot helper 
int Ballot::getVote() {
		assert(index >= 0);
		assert(votes[index] >= 0);

		return votes[index] - 1; 
}

int voting_readnum(const string& s) {
	istringstream sin(s);
	int i;
	sin >> i;
	return i;
}

// Ballot 

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

			// if a candidate has more than half the votes, they are the majority winner
			if(current_votes >= votes_needed){
				winner.push_back(candidates[i].name);
				tying = 0;
				finished = 1;
				break;
			}
			// if there's currently a tie and a candidate has 0 or tie_votes votes
			// continue the tying process
			else if (tying && (current_votes == 0 || tie_votes == 0
					|| tie_votes == current_votes)) {
				if (tie_votes == 0)
					tie_votes = current_votes;
				if (current_votes != 0)
					tied_candidates.push_back(i);
			}
			// otherwise, there is no tie
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
	// cycle through candidates until a running candidate with at least one vote is found
	while (candidates[start].still_running == false || candidates[start].votes == 0) {
		// if a running candidate has zero votes, it goes into the zero_votes vector
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
		if (candidates[x].votes == 0) // candidates with zero votes lose
			zero_votes.push_back(x);
		else if (candidates[x].votes < min) { // reset losers vector if candidate with less votes found
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
			// find the next valid candidate on the ballot
			while (candidates[ballots[y].getVote()].still_running == false) { 
				ballots[y].index++;
			} 

			// move vote to next valid candidate
			candidates[ballots[y].getVote()].addBallot(y);
		}

		candidates[x].votes = 0;
		candidates[x].ballots.clear();
	}
}

vector<string> voting_eval(vector<Candidate>& candidates, vector<Ballot>& ballots){
	assert (candidates.size() > 0);

	vector<int> ballot_index(ballots.size());

	bool finished = 0;
	vector<string> winner;

	// cycle through all ballots to count the initial votes
	count_votes(candidates, ballots);
	
	while(!finished){

		vector<int> tied_candidates;
		bool tying = 1;

		check_votes(tied_candidates, candidates, finished, tying, ballots.size(), winner);

		// if there is a tie, put all candidates in the winner vector
		if (tying == 1) {
			for (unsigned int x = 0; x < tied_candidates.size(); x++)
					winner.push_back(candidates[tied_candidates[x]].name);
		}


		if (!finished) {													// increment ballot 
			vector<int> losers;	
			vector<int> zero_votes;											// when no winner

			// find the first valid candidate, candidates with zero votes
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
		// read and process input
		vector<Candidate> candidates;
		vector<Ballot> ballots;
		voting_readcandidates(r, candidates);
		voting_readballots(r, ballots);

		// print all candidates if 0 ballots
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



