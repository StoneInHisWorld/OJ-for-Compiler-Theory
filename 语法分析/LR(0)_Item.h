#pragma once
#include "declaration.h"
#include "LR(0)_Analysis.h"

// ����һ��LR0��Ŀ����������ָ��Ϊlri
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

// LR(0)��Ŀ����CLOSURE������
typedef set<LR0_Item> LR0_ItemSet;
class CLOSURE {
protected:
	// ��ʼ��ʹ�õļ��ϣ����ڱȽϴ�С�Լ��Ƿ����
	const LR0_ItemSet init_set;
	// ����������������Ŀ�ļ���
	LR0_ItemSet full_set;

public:
	CLOSURE(LR0_ItemSet init_set, LR0_ItemSet full_set);
	bool operator==(const CLOSURE &right) const;
	LR0_ItemSet get_fullSet() const;
	void print() const;
};

