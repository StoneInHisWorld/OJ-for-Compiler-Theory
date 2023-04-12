#pragma once
// ԭ���ߣ����1801 ����
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
	/*��ӡ��״̬*/
	void print();
	/*�������л�ȡһ��״̬*/
	State_Name getState();
	/*��ȡ״̬��*/
	State_Name getName();
	/*��ȡ�����ַ�*/
	Accept_Chars getAcceptChars();
	/*������һ״̬*/
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
	/*��һ���Ӽ����л��֣�����һ���Ӽ�����*/
	Sta_Subsets subSetDivide(Sta_Subset subset);
	Sta_Subsets getSubsets();
	void setSubsets(Sta_Subsets newSubsets);
	void outputSubsets();

private:
	DFA_States dfaStates_list;
	Accept_Chars accept_chars; // ���ڴ洢��������ַ�
	Sta_Subsets subSets; // ���ڴ洢״̬�Ӽ�
	map<Sta_Subset, int> subSets_to_subsetNo; // ���ڴ洢״̬�Ӽ�ת������һ�Ӽ���ӳ��
	map<State_Name, int> name_to_subsetNo; // ���ڴ洢״̬�������������Ӽ���ŵ�ӳ��
	map<State_Name, int> name_to_listpos; // ״̬����������dfa_States_list�е�λ�õ�ӳ��

	/*��һ���Ӽ����л��֣�����һ���Ӽ�����*/
	Sta_Subsets subSetDivide_bySingleChar(Sta_Subset subset, char basis);
	void mapName_To_SubsetNo(); // ����״̬�����Ӽ���ŵ�ӳ��
	void simplify_subsets(); // �������Ӽ���״̬��һ��״̬��������״̬�������ȼ�Ϊ״̬���ԽСԽ��
	void sort_subsets(); // ���Ӽ�����
};


/*����һ�ַ���replace_by�滻�ַ���source�е�target*/
string replaceAll(string source, const string target, const string replace_by);
/*�������в���ĳԪ�أ��ɹ������±꣬���򷵻�-1*/
template<class T>
int isExisit(vector<T> source, T target);
void vector_swap(Sta_Subsets::iterator target1,
	Sta_Subsets::iterator target2);