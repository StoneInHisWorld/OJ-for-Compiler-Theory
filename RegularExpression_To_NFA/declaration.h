#pragma once
// 原作者：李子煜
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <map>

using namespace std;

#define DIV_CHAR '_'    // 这是用来分隔的字符，表示连接

class State
{
public:
	int ID{};
	map<char, State *> transitions;
	vector<State *> transitions_e; // 存储本状态的epsilon转换
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

// 存储两个状态间的转换
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

// 进行正规式转NFA的操作类
class Regex2Nfa
{
private:
	const char explicitC = DIV_CHAR;
	std::string input;          // 原始的输入字符串
	std::string strBeInsert;    // 插入分隔符后的字符串
	std::string strPostfix;	// 后缀表达式
	std::string alphaBet;       // 字母表
	std::map<char, int> op_priority;  // 运算符优先级
	NFA nfa;

	// 输入输出
public:
	Regex2Nfa();
	// 获取输入存入对象的input中
	void getInput();
	void setInput(const std::string &inputT);
	void outputOrigin();
	void output();
	// 向正则表达式中插入分隔符号
	void insertExplicit();
	// 转成后缀表达式
	void convertToPostfix();
	// Using Thompson Alg to construct
	void constructToNFA();
	// 检查变量
	void printInputByChar();
	void printInsertStrByChar();
	void printPostfixStrByChar();
	// 检查一个字符是否在字符串里面
	bool isCharInStr(const string &s, const char c);
	int getMaxStateID(const NFA &nfa);
};



