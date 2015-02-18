#include <iostream> // cout, endl
#include <sstream>  // istringtstream, ostringstream
#include <string>   // string
#include <utility>  // pair

#include "gtest/gtest.h"

#include "Voting.h"

using namespace std;

// -------
// readnum
// -------

TEST(Voting, readnum_1) {
	string s("0\n");
	int i = voting_readnum(s);
	ASSERT_EQ(0, i);
}

TEST(Voting, readnum_2) {
	string s("4\n");
	int i = voting_readnum(s);
	ASSERT_EQ(4, i);
}

TEST(Voting, readnum_3) {
	string s("205\n");
	int i = voting_readnum(s);
	ASSERT_EQ(205, i);
}

// --------------
// readcandidates
// --------------

TEST(Voting, readcandidates_1) {
	istringstream r("1\nJoe\n");
	vector<Candidate> c = voting_readcandidates(r);
	vector<Candidate> e = {Candidate("Joe")};
	for(unsigned int i = 0; i < c.size(); i++){
		ASSERT_EQ(c[i].name, e[i].name);
	}
}

TEST(Voting, readcandidates_2) {
	istringstream r("4\nJoe\nBob\nSue\nFred\n");
	vector<Candidate> c = voting_readcandidates(r);
	vector<Candidate> e = {Candidate("Joe"),Candidate("Bob"),
						   Candidate("Sue"),Candidate("Fred")};
	for(unsigned int i = 0; i < c.size(); i++){
		ASSERT_EQ(c[i].name, e[i].name);
	}
}

TEST(Voting, readcandidates_3) {
	istringstream r("4\nJOEJOEJOEJOEJOEJOE\nBOBBOBOBOB\nFred Fredson\nAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");
	vector<Candidate> c = voting_readcandidates(r);
	vector<Candidate> e = {Candidate("JOEJOEJOEJOEJOEJOE"),Candidate("BOBBOBOBOB"),
						   Candidate("Fred Fredson"),Candidate("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA")};
	for(unsigned int i = 0; i < c.size(); i++){
		ASSERT_EQ(c[i].name, e[i].name);
	}
}

// -----------
// readballots
// -----------

TEST(Voting, readballots_1) {
	istringstream r("1 2 3\n");
	vector<Ballot> c = voting_readballots(r);
	vector<Ballot> e = {Ballot(vector<int>({1, 2, 3}), 0)};
	for(unsigned int i = 0; i < c.size(); i++){
		for(unsigned int j = 0; j < c[i].votes.size(); j++){
			ASSERT_EQ(c[i].votes[j], e[i].votes[j]);
		}
		ASSERT_EQ(c[i].id, e[i].id);
	}
}

TEST(Voting, readballots_2) {
	istringstream r("1 2 3\n3 2 1\n");
	vector<Ballot> c = voting_readballots(r);
	vector<Ballot> e = {Ballot(vector<int>({1, 2, 3}), 0), Ballot(vector<int>({3, 2, 1}), 1)};
	for(unsigned int i = 0; i < c.size(); i++){
		for(unsigned int j = 0; j < c[i].votes.size(); j++){
			ASSERT_EQ(c[i].votes[j], e[i].votes[j]);
		}
		ASSERT_EQ(c[i].id, e[i].id);
	}
}

TEST(Voting, readballots_3) {
	istringstream r("1 2 3 4 5 6\n4 5 6 3 2 1\n6 5 4 3 2 1\n");
	vector<Ballot> c = voting_readballots(r);
	vector<Ballot> e = {Ballot(vector<int>({1, 2, 3, 4, 5, 6}), 0), 
						Ballot(vector<int>({4, 5, 6, 3, 2, 1}), 1),
						Ballot(vector<int>({6, 5, 4, 3, 2, 1}), 2)};
	for(unsigned int i = 0; i < c.size(); i++){
		for(unsigned int j = 0; j < c[i].votes.size(); j++){
			ASSERT_EQ(c[i].votes[j], e[i].votes[j]);
		}
		ASSERT_EQ(c[i].id, e[i].id);
	}
}


// --------
// readline
// --------

TEST(Voting, readline_1){
	string s("1 2 3\n");
	vector<int> c = voting_readline(s);
	vector<int> e = {1, 2, 3};
	ASSERT_EQ(c.size(), e.size());
	for(unsigned int i = 0; i < c.size(); i++){
		ASSERT_EQ(c[i], e[i]);
	}
}

TEST(Voting, readline_2){
	string s("1\n");
	vector<int> c = voting_readline(s);
	vector<int> e = {1};
	ASSERT_EQ(c.size(), e.size());
	for(unsigned int i = 0; i < c.size(); i++){
		ASSERT_EQ(c[i], e[i]);
	}
}

TEST(Voting, readline_3){
	string s("1 2 3 4 5 6 7 8\n");
	vector<int> c = voting_readline(s);
	vector<int> e = {1, 2, 3, 4, 5, 6, 7, 8};
	ASSERT_EQ(c.size(), e.size());
	for(unsigned int i = 0; i < c.size(); i++){
		ASSERT_EQ(c[i], e[i]);
	}
}

// -----------
// count_votes
// -----------

TEST(Voting, count_votes_1){
	vector<Candidate> c = {Candidate("Joe")};
	vector<Ballot> b = {Ballot({1}, 0)};
	vector<int> e = {1};
	count_votes(c, b);

	for(unsigned int i = 0; i < c.size(); i++){
		ASSERT_EQ(c[i].votes, e[i]);
	}
}

TEST(Voting, count_votes_2){
	vector<Candidate> c = {Candidate("Joe"), Candidate("Sue")};
	vector<Ballot> b = {Ballot({1, 2}, 0), Ballot({1, 2}, 1)};
	vector<int> e = {2, 0};
	count_votes(c, b);

	for(unsigned int i = 0; i < c.size(); i++){
		ASSERT_EQ(c[i].votes, e[i]);
	}
}

TEST(Voting, count_votes_3){
	vector<Candidate> c = {Candidate("Joe"), Candidate("Sue"), Candidate("Bob")};
	vector<Ballot> b = {Ballot({1, 2, 3}, 0), Ballot({1, 2, 3}, 1), Ballot({3, 2, 1}, 2), Ballot({2, 1, 3}, 3)};
	vector<int> e = {2, 1, 1};
	count_votes(c, b);

	for(unsigned int i = 0; i < c.size(); i++){
		ASSERT_EQ(c[i].votes, e[i]);
	}
}

// -----------
// check_votes
// -----------

TEST(Voting, check_votes_1){
	vector<Candidate> c = {Candidate("Joe", 3), Candidate("Sue", 0)};
	vector<string> e = {"Joe"};
	vector<int> etc;
	vector<int> tc;
	vector<string> w;
	bool f = 0;
	bool t = 1;
	int bs = 3;
	check_votes(tc, c, f, t, bs, w);
	ASSERT_EQ(f, 1);
	ASSERT_EQ(t, 0);
	ASSERT_EQ(w.size(), e.size());
	for(unsigned int i = 0; i < w.size(); i++){
		ASSERT_EQ(w[i], e[i]);
	}
	ASSERT_EQ(tc.size(), etc.size());
	for(unsigned int i = 0; i < tc.size(); i++){
		ASSERT_EQ(tc[i], etc[i]);
	}
}

TEST(Voting, check_votes_2){
	vector<Candidate> c = {Candidate("Joe", 3), Candidate("Sue", 3), Candidate("Bob", 3)};
	vector<string> e;
	vector<int> etc = {0, 1, 2};
	vector<int> tc;
	vector<string> w;
	bool f = 0;
	bool t = 1;
	int bs = 9;
	check_votes(tc, c, f, t, bs, w);
	ASSERT_EQ(f, 1);
	ASSERT_EQ(t, 1);
	ASSERT_EQ(w.size(), e.size());
	for(unsigned int i = 0; i < w.size(); i++){
		ASSERT_EQ(w[i], e[i]);
	}
	ASSERT_EQ(tc.size(), etc.size());
	for(unsigned int i = 0; i < tc.size(); i++){
		ASSERT_EQ(tc[i], etc[i]);
	}
}

TEST(Voting, check_votes_3){
	vector<Candidate> c = {Candidate("Joe", 2), Candidate("Sue", 1), Candidate("Bob", 1)};
	vector<string> e;
	vector<int> etc;
	vector<int> tc;
	vector<string> w;
	bool f = 0;
	bool t = 1;
	int bs = 5;
	check_votes(tc, c, f, t, bs, w);
	ASSERT_EQ(f, 0);
	ASSERT_EQ(t, 0);
	ASSERT_EQ(w.size(), e.size());
	for(unsigned int i = 0; i < w.size(); i++){
		ASSERT_EQ(w[i], e[i]);
	}
	ASSERT_EQ(tc.size(), etc.size());
	for(unsigned int i = 0; i < tc.size(); i++){
		ASSERT_EQ(tc[i], etc[i]);
	}
}


// -----
// solve
// -----

TEST(Voting, solve_1) {
	istringstream r("1\n\n2\nJoe\nBob\n1 2\n2 1\n");
	ostringstream w;
	voting_solve(r, w);
	ASSERT_EQ("Joe\nBob\n", w.str());
}

TEST(Voting, solve_2) {
	istringstream r("2\n\n2\nJoe\nBob\n1 2\n2 1\n\n3\nJoe\nBob\nSue\n1 2 3\n1 2 3\n2 1 3\n2 1 3\n3 2 1\n");
	ostringstream w;
	voting_solve(r, w);
	ASSERT_EQ("Joe\nBob\n\nBob\n", w.str());
}

TEST(Voting, solve_3) {
	istringstream r("1\n\n3\nJoe\nBob\nSue\n1 2 3\n2 1 3\n3 1 2\n");
	ostringstream w;
	voting_solve(r, w);
	ASSERT_EQ("Joe\nBob\nSue\n", w.str());
}

// -----------
// voting_eval
// -----------

TEST(Voting, eval1) {
	vector<Candidate> c = {Candidate("Joe"), Candidate("Bob"), Candidate("Sue")};
	vector<Ballot> b = {Ballot({1,2,3}, 0), Ballot({1,2,3}, 1), Ballot({3,2,1}, 2)};
	vector<string> ans = {"Joe"};
	vector<string> res = voting_eval(c, b);
	ASSERT_EQ(ans, res);
}

TEST(Voting, eval2) {
	vector<Candidate> c = {Candidate("Joe"), Candidate("Bob")};
	vector<Ballot> b = {Ballot({1, 2}, 0), Ballot({2, 1}, 1)};
	vector<string> ans = {"Joe", "Bob"};
	vector<string> res = voting_eval(c, b);
	ASSERT_EQ(ans, res);
}

TEST(Voting, eval3) {
	vector<Candidate> c = {Candidate("Joe"), Candidate("Bob"), Candidate("Sue")};
	vector<Ballot> b = {Ballot({1,2,3}, 0), Ballot({1,2,3}, 1), Ballot({1,2,3}, 2),
						Ballot({3, 1, 2}, 3), Ballot({2, 1, 3}, 4), Ballot({3, 1, 2}, 5) };
	vector<string> ans = {"Joe"};
	vector<string> res = voting_eval(c, b);
	ASSERT_EQ(ans, res);
}

// ------------
// update_state
// ------------

TEST(Voting, update_state1) {
        vector<int> l = {2};
        vector<Candidate> c = {Candidate("Joe", 2, {0, 1}),
                                Candidate("Sue", 2, {2, 3}),
                                Candidate("Bob", 1, {4})};        

        vector<Ballot> b = {Ballot({1, 2, 3}, 0), Ballot({1, 3, 2}, 1),
                            Ballot({2, 1, 3}, 2), Ballot({2, 2, 1}, 3),                            Ballot({3, 1, 2}, 4)};        
        vector<int> index_ans = {0, 0, 0, 0, 1};        
        vector<int> vote_ans = {0, 0, 0};       
        vector<int> cand_ballots = {};
        
        update_state(l, c, b);
        
        for (unsigned int x = 0; x < b.size(); x++) {
                ASSERT_EQ(b[x].index, index_ans[x]);        }        

        for (unsigned int x = 0; x < c.size(); x++) {
                ASSERT_EQ(c[x].votes, vote_ans[x]);
                ASSERT_EQ(c[x].ballots, cand_ballots);
        }
}

TEST(Voting, update_state2) {
        vector<int> l = {};
        vector<Candidate> c = {Candidate("Joe", 2, {0, 1}),
                                Candidate("Sue", 2, {2, 3})};        

        vector<Ballot> b = {Ballot({1, 2}, 0), Ballot({1, 2}, 1),
                            Ballot({2, 1}, 2), Ballot({2, 1}, 3)};        
        vector<int> index_ans = {0, 0, 0, 0};        
        vector<int> vote_ans = {0, 0, 0};       
        vector<int> cand_ballots = {};
        
        update_state(l, c, b);
        
        for (unsigned int x = 0; x < b.size(); x++) {
                ASSERT_EQ(b[x].index, index_ans[x]);
        }        

        for (unsigned int x = 0; x < c.size(); x++) {
                ASSERT_EQ(c[x].votes, vote_ans[x]);
                ASSERT_EQ(c[x].ballots, cand_ballots);
        }
}

TEST(Voting, update_state3) {
        vector<int> l = {1, 2};
        vector<Candidate> c = {Candidate("Joe", 2, {0, 1}),
                                Candidate("Sue", 1, {2}),
                                Candidate("Bob", 1, {3})};        

        vector<Ballot> b = {Ballot({1, 2, 3}, 0), Ballot({1, 3, 2}, 1),
                            Ballot({2, 1, 3}, 2), Ballot({3, 2, 1}, 3)};        
        vector<int> index_ans = {0, 0, 1, 2};        
        vector<int> vote_ans = {0, 0, 0};       
        vector<int> cand_ballots = {};
        
        update_state(l, c, b);
        
        for (unsigned int x = 0; x < b.size(); x++) {
                ASSERT_EQ(b[x].index, index_ans[x]);        }        

        for (unsigned int x = 0; x < c.size(); x++) {
                ASSERT_EQ(c[x].votes, vote_ans[x]);
                ASSERT_EQ(c[x].ballots, cand_ballots);
        }
}

// ----------
// find_start
// ----------

TEST(Voting, start_1) {
    vector<Candidate> c = {Candidate("Joe", 1), Candidate("Sue", 2)};
    vector<int> z;
    int s = find_start(c, z);
    vector<int> ez;

    ASSERT_EQ(s, 0);
    ASSERT_EQ(z.size(), ez.size());
    for(unsigned int i = 0; i < z.size(); i++){
        ASSERT_EQ(z[i], ez[i]);
    }
}

TEST(Voting, start_2) {
    vector<Candidate> c = {Candidate("Joe", 0), Candidate("Sue", 2)};
    vector<int> z;
    int s = find_start(c, z);
    vector<int> ez = {0};

    ASSERT_EQ(s, 1);
    ASSERT_EQ(z.size(), ez.size());
    for(unsigned int i = 0; i < z.size(); i++){
        ASSERT_EQ(z[i], ez[i]);
    }
}

TEST(Voting, start_3) {
    vector<Candidate> c = {Candidate("Joe", 0), Candidate("Sue", 0),
    Candidate("Bob", 0), Candidate("Jake", 2), Candidate("Mark", 0)};
    vector<int> z;
    int s = find_start(c, z);
    vector<int> ez = {0, 1, 2};

    ASSERT_EQ(s, 3);
    ASSERT_EQ(z.size(), ez.size());
    for(unsigned int i = 0; i < z.size(); i++){
        ASSERT_EQ(z[i], ez[i]);
    }
}

// -----------
// find_losers
// -----------

TEST(Voting, losers_1) {
    vector<Candidate> c = {Candidate("Joe", 2), Candidate("Sue", 1),
                            Candidate("Bob", 2)};
    vector<int> z;
    vector<int> l;
    int s = 0;
    vector<int> el = {1};

    find_losers(c, z, l, s);


    ASSERT_EQ(l.size(), el.size());
    for(unsigned int i = 0; i < l.size(); i++){
        ASSERT_EQ(l[i], el[i]);
    }
}

TEST(Voting, losers_2) {
    vector<Candidate> c = {Candidate("Joe", 1), Candidate("Sue", 0),
                            Candidate("Bob", 2), Candidate("Joe", 3)};
    vector<int> z;
    vector<int> l;
    int s = 0;
    vector<int> el = {0, 1};

    find_losers(c, z, l, s);


    ASSERT_EQ(l.size(), el.size());
    for(unsigned int i = 0; i < l.size(); i++){
        ASSERT_EQ(l[i], el[i]);
    }
}

TEST(Voting, losers_3) {
    vector<Candidate> c = {Candidate("Joe", 0), Candidate("Sue", 2),
                            Candidate("Bob", 3), Candidate("Joe", 2)};
    vector<int> z;
    vector<int> l;
    int s = 1;
    vector<int> el = {1, 3};

    find_losers(c, z, l, s);


    ASSERT_EQ(l.size(), el.size());
    for(unsigned int i = 0; i < l.size(); i++){
        ASSERT_EQ(l[i], el[i]);
    }
}