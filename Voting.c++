#include <cassert>  // assert
#include <iostream> // endl, istream, ostream
#include <sstream>  // istringstream
#include <string>   // getline, string
#include <utility>  // make_pair, pair
#include <vector>
#include "Candidate.h"
#include "Ballot.h"

#include "Voting.h"

using namespace std;

int voting_readnum(const string& s) {
	istringstream sin(s);
	int i;
	sin >> i;
	return i;
}

vector<Candidate> voting_readcandidates(istream& r) {
	
	// reading in number of candidates
	string s;
	getline(r, s);
	istringstream sin(s);
	int candidates_count;
	sin >> candidates_count;

	// get string array of candidates
	int count = 0;
	vector<Candidate> candidates(candidates_count);
	while (count < candidates_count) {
		getline(r, s);
		candidates[count].name = s;			// not sure if copied by value
		count ++;
	}

	// for (int x = 0; x < candidates_count; x ++) {
	// 	cout << candidates[x].name << endl;
	// }

	return candidates;
}

vector<Ballot> voting_readballots (istream& r) {
	string s;
	istringstream sin(s);

	int i = 0;
	// getline(r, s);
	vector<Ballot> ballots;
	while (getline(r, s) && s != "") {
		Ballot b(voting_readline(s), i);
		ballots.push_back(b);
		// ballots[i].votes = voting_readline(s);
		// ballots[i].id = i;
		// ballots[i].index = 0;
		i++;
	}

	// for (unsigned int x = 0; x < ballots.size(); x++){
	// 	for (unsigned int y = 0; y < ballots[x].votes.size(); y++)
	// 		cout << ballots[x].votes[y];
	// 	cout << endl;
	// }

	return ballots;
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

// 		vector<int> votes(candidates.size());
// 		vector<vector<int> > candidate_votes(candidates.size(), vector<int>());
// 		for(unsigned int i = 0; i < ballots.size(); i++){
// 			int current_ballot_index = ballot_index[i];
// 			int cand = ballots[i][current_ballot_index] - 1;
// 			candidate_votes[cand].push_back(i);
// 			votes[cand]++;
// 			cout << i << " voting for: " << candidates[cand] << endl;
// 		}
// 		// check termination/update indices


vector<string> voting_eval(vector<Candidate> candidates, vector<Ballot> ballots){
	
	vector<int> ballot_index(ballots.size());

	bool finished = 0;
	vector<string> winner;

	while(!finished){
		//cout << "hi" << endl;

		for (unsigned int x = 0; x < ballots.size(); x++) {
			int cand = ballots[x].getVote(); 
			candidates[cand].addBallot(ballots[x].id);
		}

		// for (Candidate c: candidates)
		// 	cout << c.name << ": " << c.votes << endl;

		int votes_needed = ballots.size() / 2 + 1;
		bool tying = 1;
		int tie_votes = 0;

		vector<int> tied_candidates;

		//cout << "tie votes: " << tie_votes << endl;
		for(unsigned int i = 0; i < candidates.size(); i++){
			if (candidates[i].still_running) {
				int current_votes = candidates[i].votes;

				//cout << i << " current votes: " << current_votes << endl;
				if(current_votes >= votes_needed){ 								// majority winner
					// cout << "single winner: " << candidates[i].name << endl;
					
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
				else tying = 0;
			}
		}

		if (tying == 1) {
			for (unsigned int x = 0; x < tied_candidates.size(); x++)
					winner.push_back(candidates[tied_candidates[x]].name);

			finished = 1;
		}

		if (!finished) {													// increment ballot 
			vector<int> losers;	
			vector<int> zero_votes;											// when no winner
			int start = 0;

			while (candidates[start].still_running == false || candidates[start].votes == 0) {
				if (candidates[start].still_running == true && candidates[start].votes == 0)
					zero_votes.push_back(start);
				start++;
			}

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
			// might not work, increment ballot indices
			// because candidate ballot vector does not change
			for(int a : losers) {
				candidates[a].still_running = false;
			}

			for(int x: losers) {
				//cout << "candidate loser " << x << endl;
				for(int y: candidates[x].ballots) {
					do { ballots[y].index++;
						//cout << "decrementing ballots " << y << " " << ballots[y].index << endl;
					} while (candidates[ballots[y].getVote()].still_running == false);
				}
			}

			for (unsigned int x = 0; x < candidates.size(); x++) {
				candidates[x].votes = 0;
				candidates[x].ballots.clear();
			}
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
		vector<Candidate> candidates = voting_readcandidates(r);
		vector<Ballot> ballots = voting_readballots(r);
		vector<string> result = voting_eval(candidates, ballots);
		
		for (unsigned int x = 0; x < result.size(); x++) {
			cout << result[x] << endl;	
		}
		if (count != cases -1)
			cout << endl;
		count++;
	}
}




