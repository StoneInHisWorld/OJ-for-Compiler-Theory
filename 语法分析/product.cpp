#include "declaration.h"

// 返回一个产生式的左部，并将完整的产生式除去左部
string Product::recLeft(string &LRproduct)
{
	string L = string("");
	for (int i = 0; i < LRproduct.length() - 1; i++)
	{
		// 排除空格
		if (LRproduct.at(i) == ' ')
		{
			continue;
		}
		// 如果识别到了'->'
		if (LRproduct.at(i) == '-' && LRproduct.at(i + 1) == '>')
		{
			// 如果左部没有任何符号
			if (L == "") throw exception("空的产生式左部\n");
			LRproduct = LRproduct.substr(i + 2);
			return L;
		}
		// 将识别到的左部字符加入到L中
		L.push_back(LRproduct.at(i));
		//L.insert(L.end(), LRproduct.at(i));
	}
	// 如果没有识别到'->'则说明是错误产生式
	throw exception("错误的产生式\n");
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
	// 计算左部比较结果
	int ret = this->left.compare(pright.left);
	if (ret > 0)
	{
		return false;
	}
	// 如果左部相同则计算右部
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
