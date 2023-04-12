#pragma once
#include "declaration.h"
#include "LR(0)_Item.h"

// һ�����б�ŵ�CLOSURE��Ϊһ��LR0_State
class LR0_State : public CLOSURE
{
private:
	const int id;
	//CLOSURE closure;
	map<TER, list<LR0_State>::iterator> TER_to_p_State;
	map<NTER, list<LR0_State>::iterator> NTER_to_p_State;

public:
	LR0_State(const CLOSURE &closure, const int id = -1);
	bool operator<(const LR0_State &right) const;
	bool operator==(const LR0_State &right) const;
	void add_GO(const TER ter, list<LR0_State>::iterator &next);
	void add_GO(const NTER nter, list<LR0_State>::iterator &next);
	void add_GO(const TOKEN token, list<LR0_State>::iterator &next);
	// ��ȡ��״̬�Ķ�����¼
	map<T_CON, ACT> get_SHIFTS() const;
	// ��ȡ��״̬��ת����¼
	map<NT_CON, STATE_ID> get_GOTOS() const;
	int get_ID() const;
	void print() const;
};

typedef list<LR0_State> LR0_States;
typedef LR0_States::iterator StaIterator;