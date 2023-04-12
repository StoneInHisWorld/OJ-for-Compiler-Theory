#include "LR(0)_State.h"

LR0_State::LR0_State(const CLOSURE & closure, const int id) :
	CLOSURE(closure), id(id)
{
}

bool LR0_State::operator<(const LR0_State & right) const
{
	return this->id < right.id;
}

bool LR0_State::operator==(const LR0_State & right) const
{
	// 如果比较的State是一个临时生成的State，则比较CLOSURE
	if (right.id == -1)
	{
		return this->CLOSURE::operator==(right);
	}
	return this->id == right.id;
}

void LR0_State::add_GO(const TER ter, StaIterator &next)
{
	pair<TER, StaIterator> temp = pair<TER, StaIterator>(ter, next);
	this->TER_to_p_State.insert(temp);
	return;
}

void LR0_State::add_GO(const NTER nter, StaIterator &next)
{
	pair<NTER, StaIterator> temp = pair<NTER, StaIterator>(nter, next);
	this->NTER_to_p_State.insert(temp);
	return;
}

void LR0_State::add_GO(const TOKEN token, StaIterator &next)
{
	if (token.first != NULL)
	{
		this->add_GO(token.first, next);
	}
	else
	{
		this->add_GO(token.second, next);
	}
}

ACTIONTable LR0_State::get_SHIFTS() const
{
	map<T_CON, ACT> transitions;
	// 插入状态中记录的转换动作，如果有多个动作则抛出异常
	for (auto tran : this->TER_to_p_State)
	{
		// 遇到的转换终结符
		TER ter = tran.first;
		STATE_ID cur_id = this->id;
		STATE_ID next_id = tran.second->get_ID();
		// 状态
		T_CON t_con = T_CON(cur_id, ter);
		// 形成新ACTION
		ACT new_act = { Shift, next_id };
		// 插入新转换，如果插入失败则说明有多个动作
		if (insert_map(t_con, new_act, transitions).second == false)
		{
			string message = "在状态" + toString(cur_id) + "遇到" + ter + "时有多个转换。\
				该文法不属于LR(0)文法，请更换其他文法，或尝试更具普适性的分析方法，如SLR(1)分析法\n";
			throw exception(message.data());
		}
	}
	return transitions;
}

GOTOTable LR0_State::get_GOTOS() const
{
	GOTOTable transitions;
	// 根据状态中保存的转换映射生成转换，覆盖之前插入的错误转换
	for (auto tran : this->NTER_to_p_State)
	{
		// 遇到的转换终结符
		NTER nter = tran.first;
		STATE_ID cur_id = this->id;
		STATE_ID next_id = tran.second->get_ID();
		// 状态
		NT_CON nt_con = NT_CON(cur_id, nter);
		// 形成新GO，如果插入失败则报错
		if (insert_map(nt_con, next_id, transitions).second == false)
		{
			string message = "在状态" + toString(cur_id) + "遇到" + nter + "时有多个转换。\
				该文法不属于LR(0)文法，请更换其他文法，或尝试更具普适性的分析方法，如SLR(1)分析法\n";
			throw exception(message.data());
		}
	}
	return transitions;
}

int LR0_State::get_ID() const
{
	return this->id;
}

void LR0_State::print() const
{
	cout << "State" << this->id << endl;
	this->CLOSURE::print();
}