#pragma once
#include "LL(1)_Table_Gen.h"
#include "Printable_Stack.h"

#define PUSH(symbol) this->stack.push(symbol);

typedef Printable_Stack<TOKEN> Analysis_Stack;
typedef string LL1_ACT;
typedef TOKEN SYMBOL;

#ifdef _TEST_USECASE_AUTOGEN_
class LL1_Analysis : public UseCase_AutoGen
{
private:
	string input;
	int input_iterator;
	// LL(1)的预测分析表
	map<LL1_CON, PIterator> Pre_And_Ana_Table;
	// 过程输出表
	ProcessTable processTable;
	// 分析栈
	Analysis_Stack stack;

public:
	LL1_Analysis();
	LL1_Analysis(const LL1_Table_Gen &table_gen);
	void init();
	void read();
	void compute();
	OutputTable getOutput() const;
};
#endif // _TEST_USECASE_AUTOGEN_

#ifndef _TEST_USECASE_AUTOGEN_
class LL1_Analysis : public Parser
{
private:
	string input;
	int input_iterator;
	// LL(1)的预测分析表
	map<LL1_CON, PIterator> Pre_And_Ana_Table;
	// 过程输出表
	ProcessTable processTable;
	// 分析栈
	Analysis_Stack stack;

private:
	LL1_ACT next_action(SYMBOL x, TER next_ter);
	string getOutputString_Symbol_Stack();
	string getOutputString_Remaining_InputString(int &input_ite);

public:
	LL1_Analysis();
	LL1_Analysis(const LL1_Table_Gen &table_gen);
	void init();
	void read();
	void compute();
};
#endif // !_TEST_USECASE_AUTOGEN_

