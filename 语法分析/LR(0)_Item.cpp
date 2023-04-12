#pragma once
#include "declaration.h"
#include "LR(0)_Item.h"

LR0_Item::LR0_Item(const Product &ps, const int pos_point): 
	Product(ps), pos_point(pos_point)
{
	int len_right = ps.getRight().length();
	if (pos_point < 0 || pos_point > len_right)
	{
		throw exception("错误的LR(0)项目点位\n");
	}
}

bool LR0_Item::operator<(const LR0_Item & right) const
{
	// 产生式ID小则排前面
	if (this->id < right.id)
	{
		return true;
	}
	else if (this->id == right.id)
	{
		if (this->pos_point > right.pos_point)
		{
			return true;
		}
	}
	return false;
}

bool LR0_Item::operator==(const LR0_Item & right) const
{
	if (Product::operator==(right))
	{
		if (this->pos_point == right.pos_point)
		{
			return true;
		}
	}
	return false;
}

int LR0_Item::getPos_Point() const
{
	return this->pos_point;
}

string LR0_Item::toString() const
{
	string right = this->right;
	right.insert(pos_point, "·");
	return this->left + "->" + right;
}

CLOSURE::CLOSURE(LR0_ItemSet init_set, LR0_ItemSet full_set): init_set(init_set)
{
	this->full_set = full_set;
}

bool CLOSURE::operator==(const CLOSURE & right) const
{
	// 如果初始化的集合中有不包含的，则说明两个CLOSURE不相等
	for_LR0_ITEMSET(this->init_set)
	{
		LR0_Item item = *lri;
		// 如果另一个CLOSURE不包含该项目，则返回false
		if (right.init_set.count(item) == 0)
		{
			return false;
		}
	}
	return true;
}

LR0_ItemSet CLOSURE::get_fullSet() const
{
	return this->full_set;
}

void CLOSURE::print() const
{
	for_LR0_ITEMSET(this->full_set)
	{
		LR0_Item item = *lri;
		cout << item.toString() << endl;
	}
}
