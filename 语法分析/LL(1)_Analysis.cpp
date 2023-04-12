#include "LL(1)_Analysis.h"

LL1_Analysis::LL1_Analysis()
{
}

LL1_Analysis::LL1_Analysis(const LL1_Table_Gen & table_gen)
{
	this->ter_set = table_gen.getTSet();
	this->nter_set = table_gen.getNTSet();
	this->product_set = table_gen.getProductSet();
	this->NTER_to_ifEP = table_gen.getNTER_to_ifEP();
	this->Pre_And_Ana_Table = table_gen.get_CON_To_Pro();
	this->start = this->product_set.begin()->getLeft();
	this->VnNum = this->nter_set.size();
	this->VtNum = this->ter_set.size();
}

void LL1_Analysis::init()
{
	// �����δ��ʼ��
	if (this->Pre_And_Ana_Table.size() == 0)
	{
		LL1_Table_Gen table_gen = *this;
		table_gen.init();
		table_gen.compute();
		*this = table_gen;
	}
	this->input_iterator = 0;
}

void LL1_Analysis::read()
{
	Parser::read();
	Parser::init();
	cout << "������Ҫ�������ַ�����������#��β����" << endl;
	cin >> input;
	return;
}

void LL1_Analysis::compute()
{
	this->processTable.clear();
	// ����#�����ķ���ʼ��������ջ��
	TOKEN temp = { '#' , string("")};
	PUSH(temp);
	temp = { NULL, this->start };
	PUSH(temp);
	// ��ȡջ������
	SYMBOL X;
	this->stack.getTop(X);
	// ��ȡ���봮��һ�ս��
	TER next_ter = this->input[this->input_iterator];
	// ִ�з�������
	string act = "initial";
	//for (int step = 0; act != "error" && act != "acc"; step++)
	//{
	//	act = next_action(X, next_ter);
	//	string line = toString(step) + getOutputString_Symbol_Stack() + 
	//		this->getOutputString_Remaining_InputString(this->input_iterator) + 
	//		act;
	//	this->processTable.push_back(line);
	//}
}

#ifdef _TEST_USECASE_AUTOGEN_
OutputTable LL1_Analysis::getOutput() const
{
	return OutputTable();
}
#endif // _TEST_USECASE_AUTOGEN_
