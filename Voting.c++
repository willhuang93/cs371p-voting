#include <cassert>  // assert
#include <iostream> // endl, istream, ostream
#include <sstream>  // istringstream
#include <string>   // getline, string
#include <utility>  // make_pair, pair
#include <vector>

#include "Voting.h"

using namespace std;

int voting_readnum(const string& s) {
	istringstream sin(s);
	int i;
	sin >> i;
	return i;
}

vector<string> voting_readcandidates(istream& r) {
	
	// reading in number of candidates
	string s;
	getline(r, s);
	istringstream sin(s);
	int candidates_count;
	sin >> candidates_count;

	// get string array of candidates
	int count = 0;
	vector<string> candidates(candidates_count);
	while (count < candidates_count) {
		getline(r, s);
		candidates[count] = s;			// not sure if copied by value
		count ++;
	}

	for (int x = 0; x < candidates_count; x ++) {
		cout << candidates[x] << endl;
	}

	return candidates;
}

vector<vector<int> > voting_readballots (istream& r) {
	string s;
	istringstream sin(s);

	// getline(r, s);

	vector<vector<int> > ballots;
	while (getline(r, s) && s != "") {
		cout << s << endl;
		ballots.push_back(voting_readline(s));
	}

	for (unsigned int x = 0; x < ballots.size(); x++){
		for (unsigned int y = 0; y < ballots[x].size(); y++)
			cout << ballots[x][y];
		cout << endl;
	}

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

void voting_solve(istream& r, ostream& w) {

	// getting number of cases, skips a line
	//int cases;
	//int count = 0;
	string s;
	getline(r, s);
	//cases = voting_readnum(s);
	getline(r, s);

	// Note: Edge cases of 0 or negative numbers

	// iterating according to number of cases

	voting_readcandidates(r);
	voting_readballots(r);

	// while (count < cases - 1) {

	// }
}