#include "declaration.h"

// ����һ������ʽ���󲿣����������Ĳ���ʽ��ȥ��
string Product::recLeft(string &LRproduct)
{
	string L = string("");
	for (int i = 0; i < LRproduct.length() - 1; i++)
	{
		// �ų��ո�
		if (LRproduct.at(i) == ' ')
		{
			continue;
		}
		// ���ʶ����'->'
		if (LRproduct.at(i) == '-' && LRproduct.at(i + 1) == '>')
		{
			// �����û���κη���
			if (L == "") throw exception("�յĲ���ʽ��\n");
			LRproduct = LRproduct.substr(i + 2);
			return L;
		}
		// ��ʶ�𵽵����ַ����뵽L��
		L.push_back(LRproduct.at(i));
		//L.insert(L.end(), LRproduct.at(i));
	}
	// ���û��ʶ��'->'��˵���Ǵ������ʽ
	throw exception("����Ĳ���ʽ\n");
}

void Product::setId(int id)
{
	this->id = id;
}

void Product::setLeft(string left)
{
	this->left = left;
}

void Product::setRight(string right)
{
	this->right = right;
}

NTER Product::getLeft() const
{
	return this->left;
}

string Product::getRight() const
{
	return this->right;
}

int Product::getId() const
{
	return this->id;
}

bool Product::operator<(const Product pright) const
{
	// �����󲿱ȽϽ��
	int ret = this->left.compare(pright.left);
	if (ret > 0)
	{
		return false;
	}
	// �������ͬ������Ҳ�
	else if (ret == 0)
	{
		ret = this->right.compare(pright.right);
		if (ret > 0)
		{
			return false;
		}
	}
	return true;
}

bool Product::operator==(const Product pright) const
{
	if (pright.left == this->left) 
	{
		if (pright.right == this->right) 
		{
			return true;
		}
	}
	return false;
}

string Product::toString() const
{
	return this->left + "->" + this->right;
}
