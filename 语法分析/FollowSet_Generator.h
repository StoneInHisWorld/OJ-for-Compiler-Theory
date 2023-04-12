#pragma once
#include "FirstSet_Generator.h"
#include "Follow_Graph.h"

#ifdef _UPTODATE_
typedef set<TER> FOLLOWSet; // ������ż�
typedef FOLLOWSet::iterator FOLIterator; // ���ս���ĵ�����

// ����nter��FOLLOW����forѭ��������������Ϊfol_i
#define for_FOLLOW(follow) TSet follow_set = follow.selfFOLLOW; \
for (TIterator fol_i = follow_set.begin(), end = follow_set.end(); \
fol_i != end; fol_i++)
// FOLLOW���������ʽ
#define FOL_FORM(nter) "FOLLOW<" + nter + ">��"

#ifndef _TEST_USECASE_AUTOGEN_
class Follow_Gen : public Parser
{
private:
	Follow_Graph FG;
	// ���ս������FIRST����ӳ��
	map<NTER, FIRST> NTER_to_FIRST;
	// ���ս������FOLLOW����ӳ��
	map<NTER, FOLLOW> NTER_to_FOLLOW;

private:
	void draw_RelationGraph();
	void draw_graph_ofAPro(Product product);
	void draw_graph_ofAFIRST(VNode *v);
	FOLLOW compute_FOLLOW(NTER nter);
	inline void setFOLLOW(const NTER nter, const FOLLOW follow);

public:
	Follow_Gen();
	Follow_Gen(const First_Gen &fir_g);
	Follow_Gen(const Parser &parser);
	void init();
	void compute();
	void print(bool Parser_printer, bool Graph_painter);
	map<NTER, FIRST> getNTER_to_FIRST() const;
	map<NTER, FOLLOW> getNTER_to_FOLLOW() const;
	inline FOLLOW getFOLLOW(const NTER nter) const;
	static FOLLOW getFOLLOW(map<NTER, FOLLOW> aggregate, const NTER nter);
};
#endif //!_TEST_USECASE_AUTOGEN_

#ifdef _TEST_USECASE_AUTOGEN_
class Follow_Gen :public UseCase_AutoGen
{
private:
	Follow_Graph FG;
	// ���ս������FIRST����ӳ��
	map<NTER, FIRST> NTER_to_FIRST;
	// ���ս������FOLLOW����ӳ��
	map<NTER, FOLLOW> NTER_to_FOLLOW;

private:
	void empty();
	void draw_RelationGraph();
	void draw_graph_ofAPro(Product product);
	void draw_graph_ofAFIRST(VNode *v);
	FOLLOW compute_FOLLOW(NTER nter);
	inline void setFOLLOW(const NTER nter, const FOLLOW follow);

public:
	Follow_Gen();
	Follow_Gen(const First_Gen &fir_g);
	Follow_Gen(const UseCase_AutoGen &UA);
	void init();
	void compute();
	void print(bool Parser_printer, bool Graph_painter);
	map<NTER, FIRST> getNTER_to_FIRST() const;
	map<NTER, FOLLOW> getNTER_to_FOLLOW() const;
	inline FOLLOW getFOLLOW(const NTER nter) const;
	static FOLLOW getFOLLOW(map<NTER, FOLLOW> aggregate, const NTER nter);
	OutputTable getOutput() const;
};
#endif // _TEST_USECASE_AUTOGEN_

inline void Follow_Gen::setFOLLOW(const NTER nter, const FOLLOW follow)
{
	pair<NTER, FOLLOW> temp = pair<NTER, FOLLOW>(nter, follow);
	auto ret = this->NTER_to_FOLLOW.insert(temp);
	if (!ret.second)
	{
		string message = "�ظ�����" + nter + "��FOLLOW��\n";
		throw exception(message.data());
	}
}

inline FOLLOW Follow_Gen::getFOLLOW(const NTER nter) const
{
	auto ret = this->NTER_to_FOLLOW.find(nter);
	if (ret != this->NTER_to_FOLLOW.end())
	{
		return ret->second;
	}
	else
	{
		string message = "δ�ҵ�" + nter + "��FOLLOW��\n";
		throw exception(message.data());
	}
}

inline FOLLOW Follow_Gen::getFOLLOW(map<NTER, FOLLOW> aggregate, const NTER nter)
{
	auto ret = aggregate.find(nter);
	if (ret != aggregate.end())
	{
		return ret->second;
	}
	else
	{
		string message = "δ�ҵ�" + nter + "��FOLLOW��\n";
		throw exception(message.data());
	}
}
#endif // _UPTODATE_
