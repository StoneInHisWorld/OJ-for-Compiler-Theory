#pragma once
#pragma once
#include "LR(0)_Analysis.h"
#include "LR(0)_Item.h"
#include "LR(0)_State.h"

class LR0_Analysis_AutoGen : public LR0_Analysis
{
private:
	// �洢����ó���״̬
	LR0_States states;

public:
	void test();
	void read();
	void init();

private:
	void validate_input();

// ����GOTO��ACTION���Զ�����
private:
	TOKEN getNextToken(LR0_Item item);
	// ��ȡLR(0)��Ŀ�ĺ����Ŀ
	LR0_Item get_postItem(LR0_Item &item) const;
	// ����ӿڣ������ض�״̬�����ķ����ź���һ��CLOSURE��ʲô
	void GO(StaIterator &p_state, int & id);
	// ��һ��LR0��Ŀ����CLOSURE������ӿ�
	CLOSURE get_CLOSURE(LR0_ItemSet &init_set);
	// ����GOTO��
	GOTOTable compute_GOTOTable();
	// ����ACTION��
	ACTIONTable compute_ACTIONTable();

private:
	// Ѱ����ͬ��һ�ַ���LR0_Items
	CLOSURE GO(LR0_ItemSet full_set, const TER common_ter);
	CLOSURE GO(LR0_ItemSet full_set, const  NTER common_nter);
	// ��һ���µ�CLOSURE����LR0״̬
	// ����Ѿ�������ͬCLOSURE��״̬���򷵻��Ѵ��ڵ�״̬
	StaIterator insert_state(const CLOSURE &closure, int &id);
	// ����������Ŀ����CLOSURE������init_set������������һ���ս���Ĳ���ʽ����Ŀ
	LR0_ItemSet get_CLOSURE(const LR0_ItemSet &init_set, set<NTER> &added);
	// ��ȡ״̬�Ĺ�Լ����
	map<T_CON, ACT> get_REDUCE(LR0_State &state) const;

#ifdef _TEST_USECASE_AUTOGEN_
public:
	void empty();

public:
	OutputTable getOutput();
	void saveUseCases(UseCases useCases, string file_name);
#endif
};