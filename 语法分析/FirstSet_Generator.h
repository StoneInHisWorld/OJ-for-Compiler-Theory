#pragma once
#include "Parser.h"
#include "UseCase_AutoGen.h"

typedef struct FIRST {
	// ����NTER
	NTER nter;
	set<TER> selfFIRST;
}FIRST;
typedef set<TER> FIRSTSet; // �׷��ż�
typedef FIRSTSet::iterator FIRIterator; // ���ս���ĵ�����

// ����nter��FIRST����forѭ��������������Ϊfir_i
#define for_FIRST(first) TSet first_set = first.selfFIRST;\
for (TIterator fir_i = first_set.begin(), end = first_set.end(); \
fir_i != end; fir_i++)
// FIRST���������ʽ
#define FIR_FORM(nter) "FIRST<" + nter + ">��"

#ifdef _UPTODATE_
#ifndef _TEST_USECASE_AUTOGEN_
class First_Gen : public Parser
{
private:
	void insert_TER_to_FIRST(TER ter, NTER nter);
	inline void setFIRST(const NTER nter, const FIRST first);
	// ����һ���ַ�����FIRST����skip����������㴦����ķ��ս��
	// �����һ����first�Լ���skip��
	void computeFIRST(const string right, FIRST &first, set<NTER> &skip) const;

private:
	// ���ս������FIRST����ӳ��
	map<NTER, FIRST> NTER_to_FIRST;

public:
	First_Gen();
	First_Gen(const Parser &parser);
	// ���м���FIRST���ĳ�ʼ������
	void init();
	// ����FIRST����
	void compute();
	map<NTER, FIRST> getNTER_to_FIRST() const;
	inline FIRST get_FIRST(const NTER nter) const;
	static FIRST get_FIRST(const map<NTER, FIRST> aggregate, const NTER nter);

public:
	void print(bool vtvn_printer, bool Parser_printer);
	// �����ַ�����FIRST�����ⲿ�ӿڣ�ͨ������ʽ�Ҳ����㣩
	FIRST computeFIRST(const string right) const;
};
#endif // !_TEST_USECASE_AUTOGEN_

#ifdef _TEST_USECASE_AUTOGEN_
class First_Gen : public UseCase_AutoGen
{
public:
	First_Gen();
	First_Gen(const UseCase_AutoGen & UA);
	// ���м���FIRST���ĳ�ʼ������
	void init();
	// ����FIRST����
	void compute();

private:
	void insert_TER_to_FIRST(TER ter, NTER nter);

#ifdef _UPTODATE_
private:
	// ���ս������FIRST����ӳ��
	map<NTER, FIRST> NTER_to_FIRST;

private:
	inline void setFIRST(const NTER nter, const FIRST first);
	// ����һ���ַ�����FIRST����skip����������㴦����ķ��ս��
	// �����һ����first�Լ���skip��
	void computeFIRST(const string right, FIRST &first, set<NTER> &skip) const;

public:
	void empty();
	void print(bool vtvn_printer, bool Parser_printer);
	map<NTER, FIRST> getNTER_to_FIRST() const;
	inline FIRST get_FIRST(const NTER nter) const;
	static FIRST get_FIRST(const map<NTER, FIRST> aggregate, const NTER nter);
	// �����ַ�����FIRST�����ⲿ�ӿڣ�ͨ������ʽ�Ҳ����㣩
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
		string message = "δ�ҵ�" + nter + "��FIRST���������Ƿ��н��м���\n";
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
		string message = "δ�ҵ�" + nter + "��FIRST���������Ƿ��н��м���\n";
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
		string message = "�ظ�����" + nter + "��FIRST��\n";
		throw exception(message.data());
	}
}
#endif // _UPTODATE_

