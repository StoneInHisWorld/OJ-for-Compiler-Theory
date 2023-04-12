#pragma once
#include "Parser.h"
#include "Printable_Stack.h"
#include "UseCase_AutoGen.h"

enum Action { Reduce, Shift, Err, Acc };

#define STATE_ID int
#define INI_STA_ID 0
#define NULL_GO -1
#define NULL_ACTION string(" ")
#define SYMBOL TER
// Action == Reduce���һ������Ϊ����ʽ���
// Action == Shift���һ������Ϊ��һ��״̬
// Action == NULL_Action���һ������ΪNULL
#define ACT pair<Action, STATE_ID>
// CON������STATE_ID״̬�¶���TER
#define T_CON  pair<STATE_ID, TER>
// CON������STATE_ID״̬�¶���NTER
#define NT_CON  pair<STATE_ID, NTER>
// ����curSta��next_sym����һ��act
#define getACT(next_sym, curSta) 	T_CON tc = T_CON(curSta, next_sym);\
	ACT next_act = this->actionTable.find(tc)->second
// ����curSta��nter����GOTO��״̬
#define getGOTO(curSta, nter) 	NT_CON nc = NT_CON(curSta, left);\
	STATE_ID next_sta = this->gotoTable.find(nc)->second;
// ����һ�����������������������Ϊitem
#define for_ACTIONTable(table) 	for (pair<T_CON, ACT> item : table)
// ����һ��GOTO�����������������Ϊitem
#define for_GOTOTable(table) for (pair<NT_CON, STATE_ID> item : table)

typedef map<T_CON, ACT> ACTIONTable;
typedef map<NT_CON, STATE_ID> GOTOTable;
typedef Printable_Stack<STATE_ID> State_Stack;
typedef Printable_Stack<SYMBOL> Symbol_Stack;

#ifdef _TEST_USECASE_AUTOGEN_
class LR0_Analysis : public UseCase_AutoGen
#endif
#ifndef _TEST_USECASE_AUTOGEN_
class LR0_Analysis : public Parser
#endif
{
public:
	LR0_Analysis(const int state_num = 0);
	// ACTION��GOTO��Ҫ����ASCII��������
	void init();
	void compute();
	void read();
	void print(bool parser_printer, bool table_printer);

protected:
	// �洢Ҫ����Ĺ��̱�
	ProcessTable processTable;
	string input;
	int input_ite;
	ACTIONTable actionTable;
	GOTOTable gotoTable;
	State_Stack state_stack;
	Symbol_Stack symbol_stack;
	int state_num = 0;

protected:
	void validate_input();
	string get_headline(bool analysis_table, bool process_table);
	vector<string> getOutputString_Table(const ACTIONTable &table) const;
	vector<string> getOutputString_Table(const GOTOTable &table) const;

private:
	bool reduce(int id_proToReduce);
	string getOutputString_State_Stack();
	string getOutputString_Symbol_Stack();
	string getOutputString_Remaining_InputString(int &input_ite);
	string getOutputString_NextAction(int &input_ite);
	void GOTOarray_to_GOTOtable(STATE_ID **GOTO);
	void ACTIONarray_to_ACTIONtable(string **ACTION);

#ifdef _TEST_USECASE_AUTOGEN_
public:
	void empty();

public:
	OutputTable getOutput();
	void saveUseCases(UseCases useCases, string file_name);
#endif
};


