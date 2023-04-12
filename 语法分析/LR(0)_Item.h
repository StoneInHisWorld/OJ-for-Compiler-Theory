#pragma once
#include "declaration.h"
#include "LR(0)_Analysis.h"

// 遍历一个LR0项目集，迭代器指针为lri
#define for_LR0_ITEMSET(item_set) for (LR0_ItemSet::iterator lri = item_set.begin(), \
end = item_set.end(); lri != end; lri++)

class LR0_Item : public Product
{
private:
	int pos_point;

public:
	LR0_Item(const Product &ps, const int pos_point = 0);
	bool operator<(const LR0_Item &right) const;
	bool operator==(const LR0_Item &right) const;
	int getPos_Point() const;
	string toString() const;
};

// LR(0)项目集是CLOSURE的主体
typedef set<LR0_Item> LR0_ItemSet;
class CLOSURE {
protected:
	// 初始化使用的集合，用于比较大小以及是否相等
	const LR0_ItemSet init_set;
	// 包含完整的所有项目的集合
	LR0_ItemSet full_set;

public:
	CLOSURE(LR0_ItemSet init_set, LR0_ItemSet full_set);
	bool operator==(const CLOSURE &right) const;
	LR0_ItemSet get_fullSet() const;
	void print() const;
};

