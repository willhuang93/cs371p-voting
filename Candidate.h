#ifndef Candidate_h
#define Candidate_h

#include <vector>
#include <string>
#include "Ballot.h"

using namespace std;

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
