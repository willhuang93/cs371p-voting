#ifndef Ballot_h
#define Ballot_h

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
		return votes[index]; 
	}
};

#endif // Ballot_h