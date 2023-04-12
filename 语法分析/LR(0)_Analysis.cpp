#include "LR(0)_Analysis.h"

LR0_Analysis::LR0_Analysis(const int state_num)
{
	this->state_num = state_num;
}

void LR0_Analysis::init()
{
	// ��ʼ���趨�ı��
	const int StateNum = 12, VtNum = 6, VnNum = 3;
	string ActionTable[StateNum][VtNum] = {
		// �Լ�����ʹ�õ�ACTION�����Ҫ��#
		// #, (, ), *, +, i, 
		NULL_ACTION, "s4", NULL_ACTION, NULL_ACTION, NULL_ACTION, "s5",
		"acc", NULL_ACTION, NULL_ACTION, NULL_ACTION, "s6", NULL_ACTION,
		"r2", NULL_ACTION, "r2", "s7", "r2", NULL_ACTION,
		"r4", NULL_ACTION, "r4", "r4", "r4", NULL_ACTION,
		NULL_ACTION, "s4", NULL_ACTION, NULL_ACTION, NULL_ACTION, "s5",
		"r6", NULL_ACTION, "r6", "r6", "r6", NULL_ACTION,
		NULL_ACTION, "s4", NULL_ACTION, NULL_ACTION, NULL_ACTION, "s5",
		NULL_ACTION, "s4", NULL_ACTION, NULL_ACTION, NULL_ACTION, "s5",
		NULL_ACTION, NULL_ACTION, "s11", NULL_ACTION, "s6", NULL_ACTION,
		"r1", NULL_ACTION, "r1", "s7", "r1", NULL_ACTION,
		"r3", NULL_ACTION, "r3", "r3", "r3", NULL_ACTION,
		"r5", NULL_ACTION, "r5", "r5", "r5", NULL_ACTION,
	};
	int GotoTable[StateNum][VnNum] = {
		1, 3, 2,
		-1, -1, -1,
		-1, -1, -1,
		-1, -1, -1,
		8, 3, 2,
		-1, -1, -1,
		-1, 3, 9,
		-1, 10, -1,
		-1, -1, -1,
		-1, -1, -1,
		-1, -1, -1,
		-1, -1, -1 };
	this->state_num = StateNum;
	string **p_action = new string*[StateNum];
	int **p_goto = new int*[StateNum];
	for (int i = 0; i < StateNum; i++)
	{
		p_action[i] = ActionTable[i];
		p_goto[i] = GotoTable[i];
	}
	// ��֤����
	validate_input();
	// ����~���滻�ɡ�#��
	auto pos = this->ter_set.find(EP);
	this->ter_set.erase(pos);
	this->ter_set.insert('#');
	// �ѷ��ս���е�"#"ɾ��
	auto nter_pos = this->nter_set.find("#");
	this->nter_set.erase(nter_pos);
	this->VnNum--;
	// ��ʼ��ACTION���Լ�GOTO��
	this->ACTIONarray_to_ACTIONtable(p_action);
	this->GOTOarray_to_GOTOtable(p_goto);
}

void LR0_Analysis::validate_input()
{
	for (int i = 0; i < this->input.size(); i++)
	{
		auto ret = Parser::getNextToken(this->input, i);
		// �����һ���ַ��Ƿ��ս��
		if (ret.first == NULL)
		{
			throw exception("���봮�в��ܰ������ս��\n");
		}
	}
	input += "#";
}

void LR0_Analysis::compute()
{
	int input_ite = 0;
	// ��"#"ѹ��ջ
	this->symbol_stack.push('#');
	this->state_stack.push(INI_STA_ID);
	int step = 1;
	string output_ofAction = "initial";
	// ��������������ͳ�����ֹͣ
	while (output_ofAction != "acc" && output_ofAction != "error")
	{
		// ��ȡ��ǰ����ĸ�������ַ�������ƴ�ӵ�һ��
		string output_ofCurStep = fit_cube(toString(step), SHORT_COL_INTER);
		string output_ofStaStack = fit_cube(this->getOutputString_State_Stack(), LONG_COL_INTER);
		string output_ofSymStack = fit_cube(this->getOutputString_Symbol_Stack(), LONG_COL_INTER);
		string output_RemainInputStr = fit_cube(this->getOutputString_Remaining_InputString(input_ite), LONG_COL_INTER);
		output_ofAction = this->getOutputString_NextAction(input_ite);
		this->processTable.push_back(output_ofCurStep + output_ofStaStack
			+ output_ofSymStack + output_RemainInputStr + 
			COL_DIV + output_ofAction + "\n");
		step++;
	}
}

void LR0_Analysis::print(bool parser_printer, bool table_printer)
{
	if (parser_printer)
	{
		cout << "���ڴ�ӡ�ķ�..." << endl;
		Parser::print(true);
		cout << endl;
	}
	// ���Ҫ��ӡLR(0)������
	if (table_printer)
	{
		cout << "���ڴ�ӡLR(0)������..." << endl;
		vector<string> output_action = this->getOutputString_Table(this->actionTable);
		vector<string> output_goto = this->getOutputString_Table(this->gotoTable);
		string headline = this->get_headline(true, false);
		cout << headline;
		if (output_action.size() != output_goto.size())
		{
			throw exception("ACTION����GOTO����Ȳ�һ��\n");
		}
		for (int i = 0; i < output_action.size(); i++)
		{
			cout << output_action[i];
			cout << output_goto[i];
			cout << endl;
		}
		cout << endl;
	}
	// ��ӡ����ִ�б�
	cout << "���ڴ�ӡ����ִ�б�..." << endl;
	cout << this->get_headline(false, true);
	int length = this->processTable.size();
	for (int i = 0; i < length; i++)
	{
		cout << this->processTable[i];
	}
	cout << endl;
}

bool LR0_Analysis::reduce(int id_proToReduce)
{
	Product pro = this->product_set[id_proToReduce];
	string right = pro.getRight();
	string left = pro.getLeft();
	STATE_ID curSta;
	// ������ʽ�Ҳ�ȫ��Ԫ�شӷ���ջ�е���
	for (int i = right.length() - 1; i >= 0; i--)
	{
		char ch;
		this->symbol_stack.getTop(ch);
		if (right[i] == ch)
		{
			this->symbol_stack.pop(ch);
			this->state_stack.pop(curSta);
		}
		else
		{
			throw exception("��Լʱ����ջ��Ԫ�������ʽ��ƥ�����");
		}
	}
	// ������ʽ����ջ
	for (int i = 0; i < left.length(); i++)
	{
		this->symbol_stack.push(left[i]);
	}
	this->state_stack.getTop(curSta);
	// ��GOTO��
	try
	{
		getGOTO(curSta, left);
		if (next_sta == NULL_GO)
		{
			throw exception("GOTOת����������\n");
		}
		this->state_stack.push(next_sta);
		return true;
	}
	catch (const std::exception &e)
	{
		throw e;
	}
}

string LR0_Analysis::getOutputString_State_Stack()
{
	string content = string("");
	vector<STATE_ID> allElems = this->state_stack.getAllElems();
	for (int i = 0; i < allElems.size(); i++)
	{
		STATE_ID curState = allElems[i];
		if (curState > 9)
		{
			content = "." + toString(curState) + "." + content;
		}
		else
		{
			content = toString(curState) + content;
		}
	}
	return content;
}

string LR0_Analysis::getOutputString_Symbol_Stack()
{
	string content = string("");
	vector<SYMBOL> allElems = this->symbol_stack.getAllElems();
	for (int i = 0; i < allElems.size(); i++)
	{
		SYMBOL curSym = allElems[i];
		content = curSym + content;
	}
	return content;
}

string LR0_Analysis::getOutputString_Remaining_InputString(int & input_ite)
{
	return this->input.substr(input_ite);
}

string LR0_Analysis::getOutputString_NextAction(int &input_ite)
{
	STATE_ID curSta;
	SYMBOL next_sym = input[input_ite];
	bool ret = this->state_stack.getTop(curSta);
	if (ret == false)
	{
		throw exception("״̬ջ��\n");
	}
	try
	{
		getACT(next_sym, curSta);
		switch (next_act.first)
		{
		case Reduce: {
			int id_proToReduce = next_act.second;
			this->reduce(id_proToReduce);
			this->state_stack.getTop(curSta);
			string content = "r" + toString(next_act.second) + SHORT_COL_INTER;
			content += fit_cube(toString(curSta), SHORT_COL_INTER);
			return content;
		}
		case Shift: {
			// ����ת����״̬
			curSta = next_act.second;
			// ����ǰ״̬�͵�ǰ�ַ�����ջ
			this->state_stack.push(curSta);
			this->symbol_stack.push(next_sym);
			// �����ַ���ָ�������
			input_ite++;
			return "s" + toString(next_act.second);
		}
		case Err:return "error";
		case Acc:return "acc";
		default:throw exception("������δ֪����");
		}
	}
	catch (const std::exception &e)
	{
		throw e;
	}
}

void LR0_Analysis::GOTOarray_to_GOTOtable(STATE_ID ** GOTO)
{
	NTER start = (this->product_set.begin())->getLeft();
	// ����ÿ��״̬
	for (int curSta = 0; curSta < this->state_num; curSta++)
	{
		int id_nter = 0;
		for_NTER
		{
			NTER nter = *ni;
			// ����#�Լ��ع��ķ��Ŀ�ʼ����
			if (nter == "#" || nter == start)
			{
				continue;
			}
			NT_CON nc = NT_CON(curSta, nter);
			STATE_ID nextSta = GOTO[curSta][id_nter];
			pair<NT_CON, STATE_ID> temp = pair<NT_CON, STATE_ID>(nc, nextSta);
			this->gotoTable.insert(temp);
			id_nter++;
		}
	}
}

void LR0_Analysis::ACTIONarray_to_ACTIONtable(string ** ACTION)
{
	// ����ÿ��״̬
	for (int curSta = 0; curSta < this->state_num; curSta++)
	{
		int id_ter = 0;
		for_TER
		{
			TER ter = *ti;
			// ��EPת��Ϊ'#'����LR(0)����
			if (ter == EP)
			{
				ter = '#';
			}
			T_CON tc = T_CON(curSta, ter);
			string act_string = ACTION[curSta][id_ter];
			if (act_string.find('#') != string::npos)
			{
				throw exception("������ʶ���к��зǷ��ַ�\n");
			}
			ACT act = ACT();
			switch (act_string[0])
			{
			//case 's':act.first = Shift; act.second = act_string[1] - '0';
			case 's':act.first = Shift; act.second = toInt(act_string.substr(1));
				break;
			case 'r':act.first = Reduce; act.second = toInt(act_string.substr(1));
				break;
			case 'a':act.first = Acc; act.second = NULL; 
				break;
			case ' ':act.first = Err; act.second = NULL; 
				break;
			default:throw exception("�����ACTION���г�����δ֪����");
			}
			pair<T_CON, ACT> temp = pair<T_CON, ACT>(tc, act);
			this->actionTable.insert(temp);
			id_ter++;
		}
	}
}

string LR0_Analysis::get_headline(bool analysis_table, bool process_table)
{
	if (process_table)
	{
		return fit_cube("����", SHORT_COL_INTER) + fit_cube("״̬ջ", LONG_COL_INTER) +
			fit_cube("����ջ", LONG_COL_INTER) + fit_cube("���봮", LONG_COL_INTER) +
			fit_cube("ACTION", SHORT_COL_INTER) + fit_cube("GOTO", SHORT_COL_INTER) + "\n";
	}
	else if (analysis_table)
	{
		// �洢��ͷ���ս������ս�������ձ���˳��
		vector<TER> ters_in_ACTIONS;
		vector<NTER> nters_in_GOTOs;
		for_ACTIONTable(this->actionTable)
		{
			T_CON t_con = item.first;
			if (t_con.first != 0)
			{
				break;
			}
			ters_in_ACTIONS.push_back(t_con.second);
		}
		for_GOTOTable(this->gotoTable)
		{
			NT_CON nt_con = item.first;
			if (nt_con.first != 0)
			{
				break;
			}
			nters_in_GOTOs.push_back(nt_con.second);
		}
		string headline = SHORT_COL_INTER;
		// ��һ��������ͷ
		string cube_width;
		for (int i = 0; i < ters_in_ACTIONS.size(); i++)
		{
			cube_width += SHORT_COL_INTER;
		}
		headline += fit_cube("ACTION", cube_width);
		cube_width = "";
		// ������ع��ķ��Ŀ�ʼ����
		for (int i = 0; i < nters_in_GOTOs.size(); i++)
		{
			cube_width += SHORT_COL_INTER;
		}
		headline += fit_cube("GOTO", cube_width);
		// �ڶ�������ս������ս������Ϊ�����ͷ
		headline += string("\n") + SHORT_COL_INTER;
		for (TER t : ters_in_ACTIONS)
		{
			headline += COL_DIV;
			headline += t;
			headline += SHORT_COL_INTER;
		}
		for (NTER nt : nters_in_GOTOs)
		{
			headline += COL_DIV;
			headline += nt;
			headline += SHORT_COL_INTER;
		}
		headline += "\n";
		return headline;
	}
	else return "";
}

vector<string> LR0_Analysis::getOutputString_Table(const ACTIONTable &table) const
{
	vector<string> table_items;
	string line;
	int last_state_id = -1, new_state_id = -1;
	for (auto action : table)
	{
		// ����
		T_CON t_con = action.first;
		// ����������µ�״̬��������һ��
		new_state_id = t_con.first;
		if (new_state_id != last_state_id)
		{
			table_items.push_back(line);
			line = fit_cube(toString(new_state_id), SHORT_COL_INTER);
		}
		ACT act = action.second;
		string content = NULL_ACTION;
		switch (act.first)
		{
		case Reduce: { content = "r" + toString(act.second); } break;
		case Shift: { content = "s" + toString(act.second); } break;
		case Err: { content = "-"; } break;
		case Acc: { content = "acc"; } break;
		default: throw exception("������δ֪����\n");
		}
		line += fit_cube(content, SHORT_COL_INTER);
		last_state_id = new_state_id;
	}
	table_items.push_back(line);
	// ȥ�����׵Ŀ���
	table_items.erase(table_items.begin());
	return table_items;
}

vector<string> LR0_Analysis::getOutputString_Table(const GOTOTable &table) const
{
	vector<string> table_items;
	string line;
	int last_state_id = -1, new_state_id = -1;
	for (auto go : table)
	{
		// ����
		NT_CON nt_con = go.first;
		// ����������µ�״̬��������һ��
		new_state_id = nt_con.first;
		if (new_state_id != last_state_id)
		{
			table_items.push_back(line);
			line = string();
		}
		STATE_ID next_id = go.second;
		if (next_id >= this->state_num || next_id < -1)
		{
			throw exception("��״̬ת������״̬��Χ\n");
		}
		string content;
		if (next_id == -1)
		{
			content = "-";
			//line += "|-\t";
		}
		else
		{
			content = toString(next_id);
		}
		line += fit_cube(content, SHORT_COL_INTER);
		last_state_id = new_state_id;
	}
	table_items.push_back(line);
	// ȥ�����׵Ŀ���
	table_items.erase(table_items.begin());
	return table_items;
}

#ifndef _TEST_USECASE_AUTOGEN_
void LR0_Analysis::read()
{
	// ʹ��Ԥ������Ĳ���ʽ��
	string products[7] = { "S->E", "E->E+T", "E->T", "T->T*F", "T->F",
		"F->(E)", "F->i" };
	for (int id_pro = 0; id_pro < 7; id_pro++)
	{
		string temp = products[id_pro];
		Product ps;
		// ������ʽ������ȡ������ֵ��ps����
		ps.setLeft(ps.recLeft(temp));
		if (temp == "")
		{
			throw exception("���Ҳ�Ӧʹ��~��ʾ\n");
		}
		ps.setRight(temp);
		ps.setId(id_pro);
		this->product_set.push_back(ps);
	}

	// ����'#'���뵽ʽ����
	this->ter_set.insert(EP);
	this->nter_set.insert(string("#"));
	this->collectNonT(this->product_set);
	this->collectT(this->product_set);
	cout << "������Ҫ�������ַ�����������#��β����" << endl;
	cin >> input;
	return;
}
#endif

#ifdef _TEST_USECASE_AUTOGEN_
void LR0_Analysis::read()
{
	// ʹ��Ԥ������Ĳ���ʽ��
	string products[7] = { "S->E", "E->E+T", "E->T", "T->T*F", "T->F",
		"F->(E)", "F->i" };
	for (int id_pro = 0; id_pro < 7; id_pro++)
	{
		string temp = products[id_pro];
		Product ps;
		// ������ʽ������ȡ������ֵ��ps����
		ps.setLeft(ps.recLeft(temp));
		if (temp == "")
		{
			throw exception("���Ҳ�Ӧʹ��~��ʾ\n");
		}
		ps.setRight(temp);
		ps.setId(id_pro);
		this->product_set.push_back(ps);
	}

	// ����'#'���뵽ʽ����
	this->ter_set.insert(EP);
	this->nter_set.insert(string("#"));
	this->collectNonT(this->product_set);
	this->collectT(this->product_set);
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
}

void LR0_Analysis::empty()
{
	UseCase_AutoGen::empty();
	// �豣��ȫ���ķ�Ԫ���Լ�ACTION���GOTO��
	this->input = "";
	this->processTable.clear();
	this->state_stack.destroy();
	this->symbol_stack.destroy();
}

OutputTable LR0_Analysis::getOutput()
{
	OutputTable table;
	// ��ӡ����ִ�б�
	cout << "���ڴ�ӡ����ִ�б�..." << endl;
	// �������ִ�б�ı���
	string headline = this->get_headline(false, true);
	table.push_back("����ִ�б��\n");
	table.push_back(headline);
	int length = this->processTable.size();
	for (int i = 0; i < length; i++)
	{
		table.push_back(this->processTable[i]);
	}
	return table;
}

void LR0_Analysis::saveUseCases(UseCases useCases, string file_name)
{
	cout << "���ڱ���������" + file_name + FILE_TYPE + "�ļ���" << endl;
	ofstream ofs;
	ofs.open(file_name + FILE_TYPE, ios::out);
	if (!ofs)
	{
		string message = file_name + FILE_TYPE + "��ʧ�ܣ�";
		throw exception(message.data());
	}
	// ���ʾ���ķ�
	cout << "���ڴ�ӡ�����ķ�..." << endl;
	ofs << "Ŀǰ֧�ֵ��ķ���" << endl;
	for_Product
	{
		ofs << pi->toString() << endl;
	}
	// ���LR(0)������
	cout << "���ڴ�ӡLR(0)������..." << endl;
	vector<string> output_action = this->getOutputString_Table(this->actionTable);
	vector<string> output_goto = this->getOutputString_Table(this->gotoTable);
	if (output_action.size() != output_goto.size())
	{
		throw exception("ACTION����GOTO����Ȳ�һ��\n");
	}
	ofs << "LR(0)�������" << endl;
	ofs << this->get_headline(true, false);
	for (int i = 0; i < output_action.size(); i++)
	{
		ofs << output_action[i] << output_goto[i] << endl;
	}
	ofs << endl;
	// ����ÿ�����������ɹ̶���ʽ�������ı������ָ���ļ���
	UseCases::iterator uci = useCases.begin();
	UseCases::iterator end = useCases.end();
	for (UseCases::iterator uci = useCases.begin(), end = useCases.end();
		uci != end; uci++)
	{
		UseCase usecase = *uci;
		string inputString = string("");
		string outputString = string("");
		for_input(usecase) { inputString += *iti; }
		for_output(usecase) { outputString += *oti; }
		ofs << USECASE_TITLE(usecase.no) << USECASE_INPUT << inputString
			<< USECASE_OUTPUT << outputString << USECASE_ENDING;
	}
	ofs.close();
	cout << "����������ϣ�" << endl;
}
#endif