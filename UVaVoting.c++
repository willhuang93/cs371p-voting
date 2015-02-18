#include <cassert>  // assert
#include <iostream> // endl, istream, ostream
#include <sstream>  // istringstream
#include <string>   // getline, string
#include <utility>  // make_pair, pair
#include <vector>

using namespace std;

#ifndef Ballot_h
#define Ballot_h

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

#endif // Ballot_h

#ifndef Candidate_h
#define Candidate_h

class Candidate{
public:
	string name;
	vector<int> ballots;
	bool still_running;
	int votes;

	Candidate() : name(""), ballots(), still_running(1), votes(0){}
	Candidate(string n) : name(n), ballots(), still_running(1), votes(0){}

	void addBallot(int id) {
		votes++;
		ballots.push_back(id);
	}
};

#endif // Candidate_h

#ifndef Voting_h
#define Voting_h



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
vector<string> voting_eval(vector<Candidate>& candidates, vector<Ballot>& ballots);


// -------------
// voting_readline
// -------------

/**
 * @param s a const string
 * @return an int vector
 */
vector<int> voting_readline (const string& s);

#endif // Voting_h



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

void count_votes(vector<Candidate>& candidates, vector<Ballot>& ballots){
	for (Ballot b: ballots) {
		int cand = b.getVote(); 
		candidates[cand].addBallot(b.id);
	}
}

void check_votes(vector<int>& tied_candidates, vector<Candidate>& candidates, 
				bool& finished, bool& tying, int ballot_size, vector<string>& winner) {

	int votes_needed = ballot_size / 2 + 1;
	
	int tie_votes = 0;
	for(unsigned int i = 0; i < candidates.size(); i++){
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
			else tying = 0;
		}
	}
}

int find_start(vector<Candidate>& candidates, vector<int>& zero_votes) {
	int start = 0;
	while (candidates[start].still_running == false || candidates[start].votes == 0) {
		if (candidates[start].still_running == true && candidates[start].votes == 0)
			zero_votes.push_back(start);
		start++;
	}
	return start;
}

void find_losers(vector<Candidate>& candidates, vector<int>& zero_votes, vector<int>& losers, int start) {
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

vector<string> voting_eval(vector<Candidate>& candidates, vector<Ballot>& ballots){
	
	vector<int> ballot_index(ballots.size());

	bool finished = 0;
	vector<string> winner;

	while(!finished){
		//cout << "hi" << endl;

		count_votes(candidates, ballots);

		vector<int> tied_candidates;
		bool tying = 1;

		check_votes(tied_candidates, candidates, finished, tying, ballots.size(), winner);

		if (tying == 1) {
			for (unsigned int x = 0; x < tied_candidates.size(); x++)
					winner.push_back(candidates[tied_candidates[x]].name);

			finished = 1;
		}

		if (!finished) {													// increment ballot 
			vector<int> losers;	
			vector<int> zero_votes;											// when no winner

			int start = find_start(candidates, zero_votes);

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

int main () {
    using namespace std;
    voting_solve(cin, cout);
    return 0;}


