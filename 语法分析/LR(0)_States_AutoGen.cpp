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
			// 如果是探测下一字符越界，则跳过本项目
			if (strcmp(message, "字符串遍历越界\n") == 0)
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
			// 如果是探测下一字符越界，则跳过本项目
			if (strcmp(message, "字符串遍历越界\n") == 0)
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
	// 检查得到的CLOSURE是否为已求出状态的CLOSURE
	LR0_State temp = LR0_State(closure);
	for (StaIterator start = this->states.begin(), end = this->states.end();
		start != end; start++)
	{
		if (*start == temp)
		{
			return start;
		}
	}
	// 如果没有找到相同CLOSURE的状态，则创建新状态
	LR0_State new_state = LR0_State(closure, ++id);
	auto ret = this->states.insert(this->states.end(), new_state);
	// 计算新状态的转换
	GO(ret, id);
	// 返回新状态的迭代器
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
	// 如果大小无变化，则包装成CLOSURE返回
	if (last_size == new_size)
	{
		return CLOSURE(init_set, item_set);
	}
	else
	{
		throw exception("闭包计算后，项目集变小！");
	}
}

map<T_CON, ACT> LR0_Analysis_AutoGen::get_REDUCE(LR0_State &state) const
{
	map<T_CON, ACT> transitions;
	// 遍历寻找归约项目
	for (LR0_Item item : state.get_fullSet())
	{
		try
		{
			this->get_postItem(item);
		}
		catch (const std::exception &e)
		{
			string message = e.what();
			if (message.find("无后继项目") != string::npos)
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
						throw exception("文法含有归约-归约冲突。\
							请更换其他文法，或尝试更具普适性的分析方法如SLR(1)分析法\n");
					}
				}
			}
		}
	}
	// 检查是否有接受项目，即归约的产生式编号为0
	if (!transitions.empty() &&  transitions.begin()->second.second == 0)
	{
		// 将所有归约清除，只留下接受转换
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
		// 未涉及的非终结符置为错误转换
		for_NTER
		{
			// 跳过拓广文法的开始符号
			if (*ni == this->start)
			{
				continue;
			}
			// 状态
			NT_CON nt_con = NT_CON(state.get_ID(), *ni);
			// 形成新GO
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
		// 计算本状态的移进ACTION以及归约ACTION
		ACTIONTable shift_items = state.get_SHIFTS();
		ACTIONTable reduce_items = this->get_REDUCE(state);
		// 将新产生的映射项合并
		try
		{
			merge_map(shift_items, reduce_items);
		}
		catch (const std::exception &e)
		{
			string message = e.what();
			if (message.find("重复键值") != string::npos)
			{
				// 如果规约动作中只有含有一个动作，则说明当前状态为接收状态
				// 且发生了移进-接受冲突
				if (reduce_items.size() == 1)
				{
					throw exception("文法含有移进-接受冲突。请更换其他文法，或尝试更具普适性的分析方法，如SLR(1)分析法\n");
				}
				throw exception("文法含有移进-归约冲突。请更换其他文法，或尝试更具普适性的分析方法，如SLR(1)分析法\n");
			}
			else
			{
				throw e;
			}
		}
		// 将新产生的映射项和总集合并
		try
		{
			merge_map(aggregate, shift_items);
		}
		catch (const std::exception &e)
		{
			string message = e.what();
			if (message.find("重复键值") != string::npos)
			{
				throw exception("在原先已存在T_CON下插入了新的状态\n");
			}
			else
			{
				throw e;
			}
		}
		// 将未设置的T_CON设置为错误动作
		for_TER
		{
			// 状态
			T_CON t_con = T_CON(state.get_ID(), *ti);
			// 形成新ACTION
			ACT new_act = { Err, NULL };
			insert_map(t_con, new_act, aggregate);
		}
	}
	return aggregate;
}

LR0_ItemSet LR0_Analysis_AutoGen::get_CLOSURE(const LR0_ItemSet &init_set, set<NTER> &added)
{
	LR0_ItemSet full_set;
	// 遍历初始集
	for_LR0_ITEMSET(init_set)
	{
		LR0_Item item = *lri;
		try
		{
			auto ret = getNextToken(item);
			// 如果下一字符为非终结符
			if (ret.first == NULL)
			{
				NTER nter = ret.second;
				if (added.count(nter) == 0)
				{
					PIterator start, pro_end;
					this->findProduct_OfANter(nter, start, pro_end);
					// 遍历该终结符的所有产生式，并将所有产生式的项目加入到full_set中
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
			// 如果是探测下一字符越界，则跳过本项目
			if (strcmp(message, "字符串遍历越界\n") == 0)
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
		string message = "该项目" + item.toString() + "无后继项目\n";
		throw exception(message.data());
	}
	auto ret = Parser::getNextToken(right, pos_point);
	return LR0_Item(item, pos_point + 1);
}

void LR0_Analysis_AutoGen::GO(StaIterator &p_state, int & id)
{
	LR0_State &state = *p_state;
	// 存储检查过的标识符
	set<NTER> checked_nters;
	set<TER> checked_ters;
	LR0_ItemSet full_set = state.get_fullSet();
	for_LR0_ITEMSET(full_set)
	{
		try
		{
			LR0_Item item = *lri;
			auto token_ret = getNextToken(item);
			// 如果是终结符
			if (token_ret.first != NULL)
			{
				TER ter = token_ret.first;
				// 如果是尚未检查的终结符，则找到
				if (checked_ters.count(ter) == 0)
				{
					CLOSURE new_closure = GO(full_set, ter);
					// 添加新状态
					StaIterator add_ret = this->insert_state(new_closure, id);
					// 添加状态转换
					state.add_GO(token_ret, add_ret);
					checked_ters.insert(ter);
				}
				else
				{
					continue;
				}
			}
			// 非终结符
			else
			{
				NTER nter = token_ret.second;
				// 如果是尚未检查的非终结符
				if (checked_nters.count(nter) == 0)
				{
					CLOSURE new_closure = GO(full_set, nter);
					// 添加新状态
					StaIterator new_state = this->insert_state(new_closure, id);
					// 添加状态新转换
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
			// 如果是探测下一字符越界，则跳过本项目
			if (strcmp(message, "字符串遍历越界\n") == 0)
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
	// 将‘~’替换成‘#’
	auto pos = this->ter_set.find(EP);
	this->ter_set.erase(pos);
	this->ter_set.insert('#');
	// 把非终结符中的"#"删掉
	auto nter_pos = this->nter_set.find("#");
	this->nter_set.erase(nter_pos);
	this->VnNum--;
	// 验证输入
	validate_input();
	// 形成开始CLOSURE
	LR0_ItemSet itemSet;
	LR0_Item item = LR0_Item(*this->product_set.begin());
	itemSet.insert(item);
	CLOSURE init_closure = get_CLOSURE(itemSet);
	// 构造开始状态
	LR0_State init_state = LR0_State(init_closure, 0);
	// 从开始状态生成其他状态
	int id = 0;
	auto insert_ret = this->states.insert(this->states.begin(), init_state);
	this->GO(insert_ret, id);
	//for (LR0_State state : this->states)
	//{
	//	state.print();
	//}
	this->state_num = states.size();
	// 生成ACTION表
	this->actionTable = this->compute_ACTIONTable();
	this->gotoTable = this->compute_GOTOTable();
}

void LR0_Analysis_AutoGen::validate_input()
{
	Parser::validate_grammar();
	LR0_Analysis::validate_input();
	// 如果检测到输入的文法不是拓广文法
	if (this->product_set.front().getRight().length() != 1)
	{
		throw exception("请输入拓广文法\n");
	}
}

#ifdef _TEST_USECASE_AUTOGEN_
void LR0_Analysis_AutoGen::read()
{
	UseCase_AutoGen::read();
	cout << "请输入要分析的字符串（无需以#结尾）：" << endl;
	string temp;
	if (getline(cin, temp), temp != "")
	{
		this->input = temp;
		this->inputTable.push_back(this->input + "\n");
	}
	else
	{
		throw exception("空输入\n");
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
	// 打印LR(0)分析表
	cout << "正在打印LR(0)分析表..." << endl;
	vector<string> output_action = this->getOutputString_Table(this->actionTable);
	vector<string> output_goto = this->getOutputString_Table(this->gotoTable);
	string headline = this->get_headline(true, false);
	table.push_back("LR(0)分析表：\n");
	table.push_back(headline);
	if (output_action.size() != output_goto.size())
	{
		throw exception("ACTION表与GOTO表长度不一致\n");
	}
	for (int i = 0; i < output_action.size(); i++)
	{
		table.push_back(output_action[i] + output_goto[i] + "\n");
	}
	// 接上过程执行表
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
	cout << "请输入要分析的字符串（无需以#结尾）：" << endl;
	cin >> input;
	return;
}
#endif // !_TEST_USECASE_AUTOGEN_