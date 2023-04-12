#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <map>
#include <vector>
#include <list>
#include <stack>
#include <math.h>

using namespace std;

// 编译控制
//#define _DEPREDATE_ "使用废弃版本的代码"
#define _UPTODATE_ "使用最新代码"
//#define _TEST_FIRSTSET_GEN_ "测试FIRST集生成"
//#define _TEST_FOLLOWSET_GEN_ "测试FOLLOW集生成"
//#define _TEST_FOLLOW_GRAPH_ "测试用于FOLLOW集计算的关系图"
//#define _TEST_LR0_ANALYSIS_ "测试LR(0)驱动程序"
//#define _TEST_LR0_AUTO_ "测试LR(0)分析表自动生成"
//#define _TEST_LL1_TABLE_GEN_ "测试LL(1)分析"
#define _TEST_USECASE_AUTOGEN_ "测试用例自动生成"

// 符号设置
#define EP '~' // 表示epsilon
#define INPUT_END '#'// 输入终止符
#define TER char // 终结符要求只能有一个字符
#define NTER string // 非终结符可以有多个字符
#define TOKEN pair<TER, NTER> // 标识符，当first为NULL时存储的是非终结符，否则为终结符
typedef vector<string> ProcessTable; // 输出表格，向量中的每个string代表一行输出

// 标准输出和文档输出设置
#define COL_DIV "|"
#define SET_DIV " "
#ifdef _TEST_USECASE_AUTOGEN_
// 存到.txt中时，单个制表符只能空出4个
#define TABL 4 // 单个制表符能空出的最多空位
#define LONG_COL_INTER "\t\t\t"
#define SHORT_COL_INTER "\t"
#endif // _TEST_USECASE_AUTOGEN_
#ifndef _TEST_USECASE_AUTOGEN_
#define TABL 8 // 单个制表符能空出的最多空位
#define LONG_COL_INTER "\t\t"
#define SHORT_COL_INTER "\t"
#endif // !_TEST_USECASE_AUTOGEN_

class Product
{
protected:
	int id;
	NTER left;
	string right;

public:
	// 返回一个产生式的左部，并将完整的产生式除去左部以及"->"
	string recLeft(string &LRproduct);
	void setId(int id);
	void setLeft(string left);
	void setRight(string right);
	NTER getLeft() const;
	string getRight() const;
	int getId() const;
	bool operator<(const Product pright) const;
	bool operator==(const Product pright) const;
	string toString() const;
};

typedef set<TER> TSet; // 终结符集
typedef set<NTER> NTSet; // 非终结符集
typedef vector<Product> ProductSet; // 产生式集
typedef ProductSet::const_iterator PIterator; // 产生式的迭代器
typedef TSet::iterator TIterator; // 终结符的迭代器
typedef NTSet::iterator NIterator; // 非终结符的迭代器

// 遍历TER_set的for循环，迭代器名称为ti
#define for_TER for (TIterator ti = this->ter_set.begin(), \
end = this->ter_set.end(); ti != end; ti++)
// 遍历NTER_set的for循环，迭代器名称为ni
#define for_NTER for (NIterator ni = this->nter_set.begin(), \
end = this->nter_set.end(); ni != end; ni++)
// 遍历Product_set的for循环，迭代器名称为pi
#define for_Product for (PIterator pi = this->product_set.begin(), \
end = this->product_set.end(); pi != end; pi++)
