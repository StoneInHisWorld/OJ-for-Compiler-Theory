#pragma once
#include "Parser.h"
#include "UseCase_AutoGen.h"

typedef struct FIRST {
	// 所属NTER
	NTER nter;
	set<TER> selfFIRST;
}FIRST;
typedef set<TER> FIRSTSet; // 首符号集
typedef FIRSTSet::iterator FIRIterator; // 非终结符的迭代器

// 遍历nter的FIRST集的for循环，迭代器名称为fir_i
#define for_FIRST(first) TSet first_set = first.selfFIRST;\
for (TIterator fir_i = first_set.begin(), end = first_set.end(); \
fir_i != end; fir_i++)
// FIRST集的输出格式
#define FIR_FORM(nter) "FIRST<" + nter + ">："

#ifdef _UPTODATE_
#ifndef _TEST_USECASE_AUTOGEN_
class First_Gen : public Parser
{
private:
	void insert_TER_to_FIRST(TER ter, NTER nter);
	inline void setFIRST(const NTER nter, const FIRST first);
	// 计算一个字符串的FIRST集，skip用于跳过外层处理过的非终结符
	// 需给定一个空first以及空skip集
	void computeFIRST(const string right, FIRST &first, set<NTER> &skip) const;

private:
	// 非终结符及其FIRST集的映射
	map<NTER, FIRST> NTER_to_FIRST;

public:
	First_Gen();
	First_Gen(const Parser &parser);
	// 进行计算FIRST集的初始化工作
	void init();
	// 计算FIRST集合
	void compute();
	map<NTER, FIRST> getNTER_to_FIRST() const;
	inline FIRST get_FIRST(const NTER nter) const;
	static FIRST get_FIRST(const map<NTER, FIRST> aggregate, const NTER nter);

public:
	void print(bool vtvn_printer, bool Parser_printer);
	// 计算字符串的FIRST集的外部接口（通过产生式右部计算）
	FIRST computeFIRST(const string right) const;
};
#endif // !_TEST_USECASE_AUTOGEN_

#ifdef _TEST_USECASE_AUTOGEN_
class First_Gen : public UseCase_AutoGen
{
public:
	First_Gen();
	First_Gen(const UseCase_AutoGen & UA);
	// 进行计算FIRST集的初始化工作
	void init();
	// 计算FIRST集合
	void compute();

private:
	void insert_TER_to_FIRST(TER ter, NTER nter);

#ifdef _UPTODATE_
private:
	// 非终结符及其FIRST集的映射
	map<NTER, FIRST> NTER_to_FIRST;

private:
	inline void setFIRST(const NTER nter, const FIRST first);
	// 计算一个字符串的FIRST集，skip用于跳过外层处理过的非终结符
	// 需给定一个空first以及空skip集
	void computeFIRST(const string right, FIRST &first, set<NTER> &skip) const;

public:
	void empty();
	void print(bool vtvn_printer, bool Parser_printer);
	map<NTER, FIRST> getNTER_to_FIRST() const;
	inline FIRST get_FIRST(const NTER nter) const;
	static FIRST get_FIRST(const map<NTER, FIRST> aggregate, const NTER nter);
	// 计算字符串的FIRST集的外部接口（通过产生式右部计算）
	FIRST computeFIRST(const string right) const;
	OutputTable getOutput() const;
#endif // _UPTODATE_
};
#endif // _TEST_USECASE_AUTOGEN_

inline FIRST First_Gen::get_FIRST(const NTER nter) const
{
	auto ret = this->NTER_to_FIRST.find(nter);
	if (ret == this->NTER_to_FIRST.end())
	{
		string message = "未找到" + nter + "的FIRST集，请检查是否有进行计算\n";
		throw exception(message.data());
	}
	else
	{
		return ret->second;
	}
}

inline FIRST First_Gen::get_FIRST(const map<NTER, FIRST> aggregate, const NTER nter)
{
	auto ret = aggregate.find(nter);
	if (ret == aggregate.end())
	{
		string message = "未找到" + nter + "的FIRST集，请检查是否有进行计算\n";
		throw exception(message.data());
	}
	else
	{
		return ret->second;
	}
}

inline void First_Gen::setFIRST(const NTER nter, const FIRST first)
{
	pair<NTER, FIRST> temp = pair<NTER, FIRST>(nter, first);
	auto ret = this->NTER_to_FIRST.insert(temp);
	if (!ret.second)
	{
		string message = "重复插入" + nter + "的FIRST集\n";
		throw exception(message.data());
	}
}
#endif // _UPTODATE_

