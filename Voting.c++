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
	vector<Ballot> ballots(5);
	while (getline(r, s) && s != "") {
		// cout << s << endl;
		ballots[i].votes = voting_readline(s);
		ballots[i].id = i;

		// for (unsigned int x = 0; x < temp.size(); x++)
		// 	ballots[i].votes.push_back(temp[x]);

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


string voting_eval(vector<Candidate> candidates, vector<Ballot> ballots){
	
	vector<int> ballot_index(ballots.size());

	bool finished = 0;
	string winner;
	while(!finished){

		for (unsigned int x = 0; x < ballots.size(); x++) {
			int cand = ballots[x].getVote()-1; 
			candidates[cand].addBallot(ballots[x].id);
		}
		
		int votes_needed = ballots.size() / 2 + 1;
		// bool tying = 1;
		// int tie_votes = ballots.size() / candidates.size();
		for(unsigned int i = 0; i < candidates.size(); i++){
			int current_votes = candidates[i].votes;
			if(current_votes >= votes_needed){
				winner = candidates[i].name;
				finished = 1;
			}
			// else if(tying && current_votes != tie_votes){
			// 	tying = 0;
			// }
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
		string result = voting_eval(candidates, ballots);
		cout << result << endl;
		count++;
	}
}




