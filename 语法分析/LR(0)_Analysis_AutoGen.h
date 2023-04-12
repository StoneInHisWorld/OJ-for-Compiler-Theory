#pragma once
#pragma once
#include "LR(0)_Analysis.h"
#include "LR(0)_Item.h"
#include "LR(0)_State.h"

class LR0_Analysis_AutoGen : public LR0_Analysis
{
private:
	// 存储计算得出的状态
	LR0_States states;

public:
	void test();
	void read();
	void init();

private:
	void validate_input();

// 用于GOTO和ACTION的自动构造
private:
	TOKEN getNextToken(LR0_Item item);
	// 获取LR(0)项目的后继项目
	LR0_Item get_postItem(LR0_Item &item) const;
	// 对外接口，计算特定状态遇到文法符号后，下一个CLOSURE是什么
	void GO(StaIterator &p_state, int & id);
	// 对一个LR0项目集求CLOSURE，对外接口
	CLOSURE get_CLOSURE(LR0_ItemSet &init_set);
	// 计算GOTO表
	GOTOTable compute_GOTOTable();
	// 计算ACTION表
	ACTIONTable compute_ACTIONTable();

private:
	// 寻找相同下一字符的LR0_Items
	CLOSURE GO(LR0_ItemSet full_set, const TER common_ter);
	CLOSURE GO(LR0_ItemSet full_set, const  NTER common_nter);
	// 对一个新的CLOSURE生成LR0状态
	// 如果已经存在相同CLOSURE的状态，则返回已存在的状态
	StaIterator insert_state(const CLOSURE &closure, int &id);
	// 单步计算项目集的CLOSURE，遍历init_set，加入所有下一非终结符的产生式的项目
	LR0_ItemSet get_CLOSURE(const LR0_ItemSet &init_set, set<NTER> &added);
	// 获取状态的规约动作
	map<T_CON, ACT> get_REDUCE(LR0_State &state) const;

#ifdef _TEST_USECASE_AUTOGEN_
public:
	void empty();

public:
	OutputTable getOutput();
	void saveUseCases(UseCases useCases, string file_name);
#endif
};