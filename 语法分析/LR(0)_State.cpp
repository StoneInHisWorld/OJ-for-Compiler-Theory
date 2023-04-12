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
	// ����Ƚϵ�State��һ����ʱ���ɵ�State����Ƚ�CLOSURE
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
	// ����״̬�м�¼��ת������������ж���������׳��쳣
	for (auto tran : this->TER_to_p_State)
	{
		// ������ת���ս��
		TER ter = tran.first;
		STATE_ID cur_id = this->id;
		STATE_ID next_id = tran.second->get_ID();
		// ״̬
		T_CON t_con = T_CON(cur_id, ter);
		// �γ���ACTION
		ACT new_act = { Shift, next_id };
		// ������ת�����������ʧ����˵���ж������
		if (insert_map(t_con, new_act, transitions).second == false)
		{
			string message = "��״̬" + toString(cur_id) + "����" + ter + "ʱ�ж��ת����\
				���ķ�������LR(0)�ķ�������������ķ������Ը��������Եķ�����������SLR(1)������\n";
			throw exception(message.data());
		}
	}
	return transitions;
}

GOTOTable LR0_State::get_GOTOS() const
{
	GOTOTable transitions;
	// ����״̬�б����ת��ӳ������ת��������֮ǰ����Ĵ���ת��
	for (auto tran : this->NTER_to_p_State)
	{
		// ������ת���ս��
		NTER nter = tran.first;
		STATE_ID cur_id = this->id;
		STATE_ID next_id = tran.second->get_ID();
		// ״̬
		NT_CON nt_con = NT_CON(cur_id, nter);
		// �γ���GO���������ʧ���򱨴�
		if (insert_map(nt_con, next_id, transitions).second == false)
		{
			string message = "��״̬" + toString(cur_id) + "����" + nter + "ʱ�ж��ת����\
				���ķ�������LR(0)�ķ�������������ķ������Ը��������Եķ�����������SLR(1)������\n";
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