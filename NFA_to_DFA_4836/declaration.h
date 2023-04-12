// 原作者：软件1801 武睿祺
#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#define STATE_ID char // state is a char type
#define VALUE char // value is a char type
#define CLUSTER vector<STATE_ID> // cluster is a set of states

using namespace std;

typedef struct NFAState {
	STATE_ID headState;
	STATE_ID tailState;
	VALUE processingValue;
}NFAState, DFAState;

vector<NFAState> nfaInit();

CLUSTER getIndexState(const vector<NFAState>& nfa, bool get_value);

CLUSTER findNfa(const vector<NFAState>& nfa, STATE_ID s, VALUE v);

void getEpsilonCluster(const vector<NFAState>& nfa, CLUSTER & cluster, STATE_ID s);

CLUSTER move(const vector<NFAState>& nfa, CLUSTER c, VALUE v);

CLUSTER joinCluster(vector<CLUSTER> epsilonClusters, CLUSTER clusterMove);

vector<DFAState> renameDFA(vector<DFAState> dfa, STATE_ID start, STATE_ID terminate);

void outputDFA(vector<DFAState> dfa, int stateNum);

void NFA2DFA();

