#include "LR(0)_Analysis_AutoGen.h"

CLOSURE LR0_Analysis_AutoGen::GO(LR0_ItemSet full_set, const TER common_ter)
{
	LR0_ItemSet init_set;
	for (LR0_Item item : full_set)
	{
		try
		{
			if (getNextToken(item).first == common_ter)
			{
				init_set.insert(this->get_postItem(item));
			}
		}
		catch (const std::exception &e)
		{
			const char *message = e.what();
			// �����̽����һ�ַ�Խ�磬����������Ŀ
			if (strcmp(message, "�ַ�������Խ��\n") == 0)
			{
				continue;
			}
			else throw e;
		}
	}
	return get_CLOSURE(init_set);
}

CLOSURE LR0_Analysis_AutoGen::GO(LR0_ItemSet full_set, const NTER common_nter)
{
	LR0_ItemSet init_set;
	for (LR0_Item item : full_set)
	{
		try
		{
			if (getNextToken(item).second == common_nter)
			{
				init_set.insert(this->get_postItem(item));
			}
		}
		catch (const std::exception &e)
		{
			const char *message = e.what();
			// �����̽����һ�ַ�Խ�磬����������Ŀ
			if (strcmp(message, "�ַ�������Խ��\n") == 0)
			{
				continue;
			}
			else throw e;
		}
	}
	return get_CLOSURE(init_set);
}

StaIterator LR0_Analysis_AutoGen::insert_state(const CLOSURE &closure, int &id)
{
	// ���õ���CLOSURE�Ƿ�Ϊ�����״̬��CLOSURE
	LR0_State temp = LR0_State(closure);
	for (StaIterator start = this->states.begin(), end = this->states.end();
		start != end; start++)
	{
		if (*start == temp)
		{
			return start;
		}
	}
	// ���û���ҵ���ͬCLOSURE��״̬���򴴽���״̬
	LR0_State new_state = LR0_State(closure, ++id);
	auto ret = this->states.insert(this->states.end(), new_state);
	// ������״̬��ת��
	GO(ret, id);
	// ������״̬�ĵ�����
	return ret;
}

CLOSURE LR0_Analysis_AutoGen::get_CLOSURE(LR0_ItemSet &init_set)
{
	set<NTER> added;
	LR0_ItemSet item_set = init_set;
	int last_size = -1, new_size = 0;
	do
	{
		last_size = item_set.size();
		item_set = this->get_CLOSURE(item_set, added);
		new_size = item_set.size();
	} while (last_size < new_size);
	// �����С�ޱ仯�����װ��CLOSURE����
	if (last_size == new_size)
	{
		return CLOSURE(init_set, item_set);
	}
	else
	{
		throw exception("�հ��������Ŀ����С��");
	}
}

map<T_CON, ACT> LR0_Analysis_AutoGen::get_REDUCE(LR0_State &state) const
{
	map<T_CON, ACT> transitions;
	// ����Ѱ�ҹ�Լ��Ŀ
	for (LR0_Item item : state.get_fullSet())
	{
		try
		{
			this->get_postItem(item);
		}
		catch (const std::exception &e)
		{
			string message = e.what();
			if (message.find("�޺����Ŀ") != string::npos)
			{
				int sta_id = state.get_ID();
				int pro_id = item.getId();
				for_TER
				{
					T_CON t_con = { sta_id, *ti };
					ACT new_act = {Reduce, pro_id};
					auto ret = transitions.insert(pair<T_CON, ACT>(t_con, new_act));
					if (ret.second == false)
					{
						throw exception("�ķ����й�Լ-��Լ��ͻ��\
							����������ķ������Ը��������Եķ���������SLR(1)������\n");
					}
				}
			}
		}
	}
	// ����Ƿ��н�����Ŀ������Լ�Ĳ���ʽ���Ϊ0
	if (!transitions.empty() &&  transitions.begin()->second.second == 0)
	{
		// �����й�Լ�����ֻ���½���ת��
		transitions.clear();
		int state_id = state.get_ID();
		T_CON acc_con = { state_id, '#' };
		ACT acc_act = { Acc, NULL };
		insert_map(acc_con, acc_act, transitions);
	}
	return transitions;
}

GOTOTable LR0_Analysis_AutoGen::compute_GOTOTable()
{
	GOTOTable aggregate;
	for (LR0_State state : this->states)
	{
		GOTOTable goto_table_item = state.get_GOTOS();
		merge_map(aggregate, goto_table_item);
		// δ�漰�ķ��ս����Ϊ����ת��
		for_NTER
		{
			// �����ع��ķ��Ŀ�ʼ����
			if (*ni == this->start)
			{
				continue;
			}
			// ״̬
			NT_CON nt_con = NT_CON(state.get_ID(), *ni);
			// �γ���GO
			insert_map(nt_con, NULL_GO, aggregate);
		}
	}
	return aggregate;
}

ACTIONTable LR0_Analysis_AutoGen::compute_ACTIONTable()
{
	ACTIONTable aggregate;
	for (LR0_State state : this->states)
	{
		// ���㱾״̬���ƽ�ACTION�Լ���ԼACTION
		ACTIONTable shift_items = state.get_SHIFTS();
		ACTIONTable reduce_items = this->get_REDUCE(state);
		// ���²�����ӳ����ϲ�
		try
		{
			merge_map(shift_items, reduce_items);
		}
		catch (const std::exception &e)
		{
			string message = e.what();
			if (message.find("�ظ���ֵ") != string::npos)
			{
				// �����Լ������ֻ�к���һ����������˵����ǰ״̬Ϊ����״̬
				// �ҷ������ƽ�-���ܳ�ͻ
				if (reduce_items.size() == 1)
				{
					throw exception("�ķ������ƽ�-���ܳ�ͻ������������ķ������Ը��������Եķ�����������SLR(1)������\n");
				}
				throw exception("�ķ������ƽ�-��Լ��ͻ������������ķ������Ը��������Եķ�����������SLR(1)������\n");
			}
			else
			{
				throw e;
			}
		}
		// ���²�����ӳ������ܼ��ϲ�
		try
		{
			merge_map(aggregate, shift_items);
		}
		catch (const std::exception &e)
		{
			string message = e.what();
			if (message.find("�ظ���ֵ") != string::npos)
			{
				throw exception("��ԭ���Ѵ���T_CON�²������µ�״̬\n");
			}
			else
			{
				throw e;
			}
		}
		// ��δ���õ�T_CON����Ϊ������
		for_TER
		{
			// ״̬
			T_CON t_con = T_CON(state.get_ID(), *ti);
			// �γ���ACTION
			ACT new_act = { Err, NULL };
			insert_map(t_con, new_act, aggregate);
		}
	}
	return aggregate;
}

LR0_ItemSet LR0_Analysis_AutoGen::get_CLOSURE(const LR0_ItemSet &init_set, set<NTER> &added)
{
	LR0_ItemSet full_set;
	// ������ʼ��
	for_LR0_ITEMSET(init_set)
	{
		LR0_Item item = *lri;
		try
		{
			auto ret = getNextToken(item);
			// �����һ�ַ�Ϊ���ս��
			if (ret.first == NULL)
			{
				NTER nter = ret.second;
				if (added.count(nter) == 0)
				{
					PIterator start, pro_end;
					this->findProduct_OfANter(nter, start, pro_end);
					// �������ս�������в���ʽ���������в���ʽ����Ŀ���뵽full_set��
					for (Product pro = *start; start != pro_end; start++)
					{
						pro = *start;
						full_set.insert(LR0_Item(pro));
					}
					added.insert(nter);
				}
			}
		}
		catch (const std::exception &e)
		{
			const char *message = e.what();
			// �����̽����һ�ַ�Խ�磬����������Ŀ
			if (strcmp(message, "�ַ�������Խ��\n") == 0)
			{
				continue;
			}
			else throw e;
		}
	}
	merge_set(full_set, init_set);
	return full_set;
}

LR0_Item LR0_Analysis_AutoGen::get_postItem(LR0_Item &item) const 
{
	string right = item.getRight();
	int pos_point = item.getPos_Point();
	if (pos_point >= (int)right.length())
	{
		string message = "����Ŀ" + item.toString() + "�޺����Ŀ\n";
		throw exception(message.data());
	}
	auto ret = Parser::getNextToken(right, pos_point);
	return LR0_Item(item, pos_point + 1);
}

void LR0_Analysis_AutoGen::GO(StaIterator &p_state, int & id)
{
	LR0_State &state = *p_state;
	// �洢�����ı�ʶ��
	set<NTER> checked_nters;
	set<TER> checked_ters;
	LR0_ItemSet full_set = state.get_fullSet();
	for_LR0_ITEMSET(full_set)
	{
		try
		{
			LR0_Item item = *lri;
			auto token_ret = getNextToken(item);
			// ������ս��
			if (token_ret.first != NULL)
			{
				TER ter = token_ret.first;
				// �������δ�����ս�������ҵ�
				if (checked_ters.count(ter) == 0)
				{
					CLOSURE new_closure = GO(full_set, ter);
					// �����״̬
					StaIterator add_ret = this->insert_state(new_closure, id);
					// ���״̬ת��
					state.add_GO(token_ret, add_ret);
					checked_ters.insert(ter);
				}
				else
				{
					continue;
				}
			}
			// ���ս��
			else
			{
				NTER nter = token_ret.second;
				// �������δ���ķ��ս��
				if (checked_nters.count(nter) == 0)
				{
					CLOSURE new_closure = GO(full_set, nter);
					// �����״̬
					StaIterator new_state = this->insert_state(new_closure, id);
					// ���״̬��ת��
					state.add_GO(token_ret, new_state);
					checked_nters.insert(nter);
				}
				else
				{
					continue;
				}
			}
		}
		catch (const std::exception &e)
		{
			const char *message = e.what();
			// �����̽����һ�ַ�Խ�磬����������Ŀ
			if (strcmp(message, "�ַ�������Խ��\n") == 0)
			{
				continue;
			}
			else throw e;
		}
	}
}

TOKEN LR0_Analysis_AutoGen::getNextToken(LR0_Item item)
{
	string right = item.getRight();
	int pos_point = item.getPos_Point();
	return Parser::getNextToken(right, pos_point);
}

void LR0_Analysis_AutoGen::test()
{
	LR0_ItemSet itemSet;
	for_Product{
		LR0_Item item = LR0_Item(*pi);
		itemSet.insert(item);
		break;
	}
	CLOSURE init_closure = get_CLOSURE(itemSet);
	init_closure.print();
	LR0_State init_state = LR0_State(init_closure, 0);
	int id = 0;
	auto insert_ret = this->states.insert(this->states.begin(), init_state);
	this->GO(insert_ret, id);
	for (LR0_State state : this->states)
	{
		state.print();
	}
	return;
}

void LR0_Analysis_AutoGen::init()
{
	// ����~���滻�ɡ�#��
	auto pos = this->ter_set.find(EP);
	this->ter_set.erase(pos);
	this->ter_set.insert('#');
	// �ѷ��ս���е�"#"ɾ��
	auto nter_pos = this->nter_set.find("#");
	this->nter_set.erase(nter_pos);
	this->VnNum--;
	// ��֤����
	validate_input();
	// �γɿ�ʼCLOSURE
	LR0_ItemSet itemSet;
	LR0_Item item = LR0_Item(*this->product_set.begin());
	itemSet.insert(item);
	CLOSURE init_closure = get_CLOSURE(itemSet);
	// ���쿪ʼ״̬
	LR0_State init_state = LR0_State(init_closure, 0);
	// �ӿ�ʼ״̬��������״̬
	int id = 0;
	auto insert_ret = this->states.insert(this->states.begin(), init_state);
	this->GO(insert_ret, id);
	//for (LR0_State state : this->states)
	//{
	//	state.print();
	//}
	this->state_num = states.size();
	// ����ACTION��
	this->actionTable = this->compute_ACTIONTable();
	this->gotoTable = this->compute_GOTOTable();
}

void LR0_Analysis_AutoGen::validate_input()
{
	Parser::validate_grammar();
	LR0_Analysis::validate_input();
	// �����⵽������ķ������ع��ķ�
	if (this->product_set.front().getRight().length() != 1)
	{
		throw exception("�������ع��ķ�\n");
	}
}

#ifdef _TEST_USECASE_AUTOGEN_
void LR0_Analysis_AutoGen::read()
{
	UseCase_AutoGen::read();
	cout << "������Ҫ�������ַ�����������#��β����" << endl;
	string temp;
	if (getline(cin, temp), temp != "")
	{
		this->input = temp;
		this->inputTable.push_back(this->input + "\n");
	}
	else
	{
		throw exception("������\n");
	}
	return;
}

void LR0_Analysis_AutoGen::empty()
{
	this->UseCase_AutoGen::empty();
	this->LR0_Analysis::empty();
	this->states.clear();
}

OutputTable LR0_Analysis_AutoGen::getOutput()
{
	OutputTable table;
	// ��ӡLR(0)������
	cout << "���ڴ�ӡLR(0)������..." << endl;
	vector<string> output_action = this->getOutputString_Table(this->actionTable);
	vector<string> output_goto = this->getOutputString_Table(this->gotoTable);
	string headline = this->get_headline(true, false);
	table.push_back("LR(0)������\n");
	table.push_back(headline);
	if (output_action.size() != output_goto.size())
	{
		throw exception("ACTION����GOTO���Ȳ�һ��\n");
	}
	for (int i = 0; i < output_action.size(); i++)
	{
		table.push_back(output_action[i] + output_goto[i] + "\n");
	}
	// ���Ϲ���ִ�б�
	table.splice(table.end(), LR0_Analysis::getOutput());
	return table;
}

void LR0_Analysis_AutoGen::saveUseCases(UseCases useCases, string file_name)
{
	UseCase_AutoGen::saveUseCases(useCases, file_name);
}
#endif // _TEST_USECASE_AUTOGEN_

#ifndef _TEST_USECASE_AUTOGEN_
void LR0_Analysis_AutoGen::read()
{
	Parser::read();
	Parser::init();
	cout << "������Ҫ�������ַ�����������#��β����" << endl;
	cin >> input;
	return;
}
#endif // !_TEST_USECASE_AUTOGEN_