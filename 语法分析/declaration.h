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

// �������
//#define _DEPREDATE_ "ʹ�÷����汾�Ĵ���"
#define _UPTODATE_ "ʹ�����´���"
//#define _TEST_FIRSTSET_GEN_ "����FIRST������"
//#define _TEST_FOLLOWSET_GEN_ "����FOLLOW������"
//#define _TEST_FOLLOW_GRAPH_ "��������FOLLOW������Ĺ�ϵͼ"
//#define _TEST_LR0_ANALYSIS_ "����LR(0)��������"
//#define _TEST_LR0_AUTO_ "����LR(0)�������Զ�����"
//#define _TEST_LL1_TABLE_GEN_ "����LL(1)����"
#define _TEST_USECASE_AUTOGEN_ "���������Զ�����"

// ��������
#define EP '~' // ��ʾepsilon
#define INPUT_END '#'// ������ֹ��
#define TER char // �ս��Ҫ��ֻ����һ���ַ�
#define NTER string // ���ս�������ж���ַ�
#define TOKEN pair<TER, NTER> // ��ʶ������firstΪNULLʱ�洢���Ƿ��ս��������Ϊ�ս��
typedef vector<string> ProcessTable; // �����������е�ÿ��string����һ�����

// ��׼������ĵ��������
#define COL_DIV "|"
#define SET_DIV " "
#ifdef _TEST_USECASE_AUTOGEN_
// �浽.txt��ʱ�������Ʊ��ֻ�ܿճ�4��
#define TABL 4 // �����Ʊ���ܿճ�������λ
#define LONG_COL_INTER "\t\t\t"
#define SHORT_COL_INTER "\t"
#endif // _TEST_USECASE_AUTOGEN_
#ifndef _TEST_USECASE_AUTOGEN_
#define TABL 8 // �����Ʊ���ܿճ�������λ
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
	// ����һ������ʽ���󲿣����������Ĳ���ʽ��ȥ���Լ�"->"
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

typedef set<TER> TSet; // �ս����
typedef set<NTER> NTSet; // ���ս����
typedef vector<Product> ProductSet; // ����ʽ��
typedef ProductSet::const_iterator PIterator; // ����ʽ�ĵ�����
typedef TSet::iterator TIterator; // �ս���ĵ�����
typedef NTSet::iterator NIterator; // ���ս���ĵ�����

// ����TER_set��forѭ��������������Ϊti
#define for_TER for (TIterator ti = this->ter_set.begin(), \
end = this->ter_set.end(); ti != end; ti++)
// ����NTER_set��forѭ��������������Ϊni
#define for_NTER for (NIterator ni = this->nter_set.begin(), \
end = this->nter_set.end(); ni != end; ni++)
// ����Product_set��forѭ��������������Ϊpi
#define for_Product for (PIterator pi = this->product_set.begin(), \
end = this->product_set.end(); pi != end; pi++)
