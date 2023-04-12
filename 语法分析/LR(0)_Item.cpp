#pragma once
#include "declaration.h"
#include "LR(0)_Item.h"

LR0_Item::LR0_Item(const Product &ps, const int pos_point): 
	Product(ps), pos_point(pos_point)
{
	int len_right = ps.getRight().length();
	if (pos_point < 0 || pos_point > len_right)
	{
		throw exception("�����LR(0)��Ŀ��λ\n");
	}
}

bool LR0_Item::operator<(const LR0_Item & right) const
{
	// ����ʽIDС����ǰ��
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
	right.insert(pos_point, "��");
	return this->left + "->" + right;
}

CLOSURE::CLOSURE(LR0_ItemSet init_set, LR0_ItemSet full_set): init_set(init_set)
{
	this->full_set = full_set;
}

bool CLOSURE::operator==(const CLOSURE & right) const
{
	// �����ʼ���ļ������в������ģ���˵������CLOSURE�����
	for_LR0_ITEMSET(this->init_set)
	{
		LR0_Item item = *lri;
		// �����һ��CLOSURE����������Ŀ���򷵻�false
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
