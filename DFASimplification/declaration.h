#pragma once
// 原作者：软件1801 王广
#include <cstdio>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <set>
#include <map>
#include <list>
#include <vector>

using namespace std;
typedef char State_Name;
typedef vector<char> Accept_Chars;
typedef vector<char> Transfer_Func;

class DFA_State 
{
private:
	State_Name name;
	Accept_Chars accept;
	Transfer_Func transferList;

public:
	/*打印本状态*/
	void print();
	/*从输入中获取一个状态*/
	State_Name getState();
	/*获取状态名*/
	State_Name getName();
	/*获取输入字符*/
	Accept_Chars getAcceptChars();
	/*计算下一状态*/
	State_Name transfer(char inputChar);
};

typedef vector<DFA_State> DFA_States;
typedef vector<char> Sta_Subset;
typedef vector<Sta_Subset> Sta_Subsets;

class DFA
{
public:
	DFA();
	~DFA();
	DFA_States getStates();
	void setStatesList(DFA_States &list);
	/*将一个子集进行划分，返回一个子集向量*/
	Sta_Subsets subSetDivide(Sta_Subset subset);
	Sta_Subsets getSubsets();
	void setSubsets(Sta_Subsets newSubsets);
	void outputSubsets();

private:
	DFA_States dfaStates_list;
	Accept_Chars accept_chars; // 用于存储可输入的字符
	Sta_Subsets subSets; // 用于存储状态子集
	map<Sta_Subset, int> subSets_to_subsetNo; // 用于存储状态子集转换到另一子集的映射
	map<State_Name, int> name_to_subsetNo; // 用于存储状态名称与其所属子集序号的映射
	map<State_Name, int> name_to_listpos; // 状态名称与其在dfa_States_list中的位置的映射

	/*将一个子集进行划分，返回一个子集向量*/
	Sta_Subsets subSetDivide_bySingleChar(Sta_Subset subset, char basis);
	void mapName_To_SubsetNo(); // 计算状态名到子集编号的映射
	void simplify_subsets(); // 将所有子集的状态由一个状态代表，代表状态名字优先级为状态编号越小越大
	void sort_subsets(); // 将子集排序
};


/*用另一字符串replace_by替换字符串source中的target*/
string replaceAll(string source, const string target, const string replace_by);
/*在向量中查找某元素，成功返回下标，否则返回-1*/
template<class T>
int isExisit(vector<T> source, T target);
void vector_swap(Sta_Subsets::iterator target1,
	Sta_Subsets::iterator target2);