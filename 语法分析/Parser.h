#pragma once
#include "declaration.h"
#include "tool.h"

#ifdef _UPTODATE_
class Parser
{
protected:
	int VtNum;
	int VnNum;
	TSet ter_set;
	NTSet nter_set;
	ProductSet product_set;
	// 文法开始符号
	NTER start;
	// 存储非终结符能否推导出EP的映射
	map<NTER, bool> NTER_to_ifEP;

protected:
	void showProducts() const;
	// 收集产生式中的终结符
	void collectT(ProductSet product_set);
	// 收集产生式中的非终结符
	void collectNonT(ProductSet product_set);
	// 获取字符串中的下一个合法字符
	// 返回一个pair，TER为NULL则为非终结符
	TOKEN getNextToken(const string &source, int &iterator) const;
	void validate_grammar() const;
	bool deduceEpsilon(NTER nter, set<NTER> &skip);
	// 计算非终结符产生式的范围
	void findProduct_OfANter(const NTER nter, PIterator &start, PIterator &end) const;
	inline bool nullable(const NTER nter) const;

public:
	Parser();
	Parser(const Parser &parser);
	virtual void read();
	virtual void init();
	virtual void compute();
	virtual void print(const bool vtvn_printer) const;
	bool is_computed() const;
	TSet getTSet() const;
	NTSet getNTSet() const;
	ProductSet getProductSet() const;
	map<NTER, bool> getNTER_to_ifEP() const;
}; 

inline bool Parser::nullable(const NTER nter) const
{
	auto ret = this->NTER_to_ifEP.find(nter);
	if (ret == this->NTER_to_ifEP.end())
	{
		string message = "未找到" + nter + "推导epsilon的结果，请检查是否有进行初始化\n";
		throw exception(message.data());
	}
	else
	{
		return ret->second;
	}
}
#endif // _UPTODATE_