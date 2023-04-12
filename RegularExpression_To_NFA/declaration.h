#pragma once
// ԭ���ߣ�������
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <map>

using namespace std;

#define DIV_CHAR '_'    // ���������ָ����ַ�����ʾ����

class State
{
public:
	int ID{};
	map<char, State *> transitions;
	vector<State *> transitions_e; // �洢��״̬��epsilonת��
	bool isEnd;
	
public:
	State() = default;

	explicit State(int ID)
	{
		this->ID = ID;
	}

	State(int ID, bool isEnd)
	{
		this->ID = ID;
		this->isEnd = isEnd;
	}
};

// �洢����״̬���ת��
class Fragment
{
public:
	State *state_start;
	State *state_end;
public:

	Fragment() = default;

	Fragment(State *start, State *end)
	{
		this->state_start = start;
		this->state_end = end;
	}
};

class NFA
{
public:
	vector<State *> states;

public:
	NFA() = default;

	void removeState(const State *state)
	{
		for (auto it = this->states.begin(); it != this->states.end(); it++)
		{
			if ((*it)->ID == state->ID)
			{
				State *p = *it;
				this->states.erase(it);
				if (p != nullptr)
				{
					delete p;
					break;
				}
			}
		}
	}

	~NFA()
	{
		for (const State *i : states)
		{
			delete i;
		}
	}
};

// ��������ʽתNFA�Ĳ�����
class Regex2Nfa
{
private:
	const char explicitC = DIV_CHAR;
	std::string input;          // ԭʼ�������ַ���
	std::string strBeInsert;    // ����ָ�������ַ���
	std::string strPostfix;	// ��׺���ʽ
	std::string alphaBet;       // ��ĸ��
	std::map<char, int> op_priority;  // ��������ȼ�
	NFA nfa;

	// �������
public:
	Regex2Nfa();
	// ��ȡ�����������input��
	void getInput();
	void setInput(const std::string &inputT);
	void outputOrigin();
	void output();
	// ��������ʽ�в���ָ�����
	void insertExplicit();
	// ת�ɺ�׺���ʽ
	void convertToPostfix();
	// Using Thompson Alg to construct
	void constructToNFA();
	// ������
	void printInputByChar();
	void printInsertStrByChar();
	void printPostfixStrByChar();
	// ���һ���ַ��Ƿ����ַ�������
	bool isCharInStr(const string &s, const char c);
	int getMaxStateID(const NFA &nfa);
};



