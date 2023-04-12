#pragma once
#include "Parser.h"
#include "FollowSet_Generator.h"

// SELECT�������ʽ
#define SEL_FORM(pro) "SELECT<" + pro + ">��"
// �ղ���ʽ
#define NULL_PRO this->product_set.end()
// Ԥ����������������
#define ANA_TABLE_WELCOME "Ԥ�������\n"

typedef struct SELECT_SET {
	// �������
	string product;
	TSet t_set;
}SELECT;

typedef struct LL1_CON {
	NTER row;
	TER col;
	bool operator<(const LL1_CON &right) const
	{
		if (this->row == right.row)
		{
			return this->col < right.col;
		}
		return this->row < right.row;
	}
}LL1_CON;

#ifndef _TEST_USECASE_AUTOGEN_
class LL1_Table_Gen :public Parser
{
private:
	// ���ս������FOLLOW����ӳ��
	map<NTER, FOLLOW> NTER_to_FOLLOW;
	// ���ս������FIRST����ӳ��
	map<NTER, FIRST> NTER_to_FIRST;
	// ����ʽ����������SELSECT����ӳ��
	map<PIterator, SELECT> Pro_To_SELECT;
	// LL(1)��Ԥ�������
	map<LL1_CON, PIterator> CON_To_Pro;

public:
	LL1_Table_Gen();
	LL1_Table_Gen(const Follow_Gen &fol_g);
	LL1_Table_Gen(const Parser &parser);
	void init();
	void compute();
	void print(const bool SELECT_printer, const bool table_printer) const;
	map<LL1_CON, PIterator> get_CON_To_Pro() const;

private:
	SELECT compute_SELECT(PIterator &pi);
	void setSELECT(PIterator &pi, const SELECT select);
	void compute_Prediction_And_Analysis_Table();
	void insert_con_to_pro(LL1_CON con, PIterator pro);
	//// �ж����Ƿ�ΪLL(1)�ķ�
	//void validate_grammar() const;
	string toString(const SELECT select) const;
	string get_headline(const bool analysis_table, const bool process_table) const;
	list<string> get_OutputStrings_of_PreAndAnaTable() const;
};
#endif // !_TEST_USECASE_AUTOGEN_

#ifdef _TEST_USECASE_AUTOGEN_
class LL1_Table_Gen :public UseCase_AutoGen
{
private:
	// ���ս������FOLLOW����ӳ��
	map<NTER, FOLLOW> NTER_to_FOLLOW;
	// ���ս������FIRST����ӳ��
	map<NTER, FIRST> NTER_to_FIRST;
	// ����ʽ����������SELSECT����ӳ��
	map<PIterator, SELECT> Pro_To_SELECT;
	// LL(1)��Ԥ�������
	map<LL1_CON, PIterator> CON_To_Pro;

public:
	LL1_Table_Gen();
	LL1_Table_Gen(const Follow_Gen &fol_g);
	LL1_Table_Gen(const UseCase_AutoGen &UA);
	void init();
	void compute();
	void print(const bool SELECT_printer, const bool table_printer) const;
	map<LL1_CON, PIterator> get_CON_To_Pro() const;

// ����SELECT�����
private:
	SELECT compute_SELECT(PIterator &pi);
	void setSELECT(PIterator &pi, const SELECT select);
	void insert_con_to_pro(const LL1_CON con, PIterator pro);

// ����������
private:
	// �ж����Ƿ�ΪLL(1)�ķ�
	void validate_grammar() const;
	void compute_Prediction_And_Analysis_Table();
	string toString(const SELECT select) const;
	string get_headline(const bool analysis_table, const bool process_table) const;
	list<string> get_OutputStrings_of_PreAndAnaTable() const;

// �����Զ��������
private:
	void empty();
	OutputTable getOutput() const;
};
#endif // _TEST_USECASE_AUTOGEN_

