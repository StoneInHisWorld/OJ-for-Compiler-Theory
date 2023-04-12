#include "LL(1)_Table_Gen.h"

LL1_Table_Gen::LL1_Table_Gen()
{
}

void LL1_Table_Gen::init()
{
	Parser::init();
	// ���δ�����FOLLOW��
	if (this->NTER_to_FOLLOW.size() == 0)
	{
		Follow_Gen fog = *this;
		fog.init();
		fog.compute();
		*this = fog;
	}
}

void LL1_Table_Gen::compute()
{
	// ����Ѿ�������������ظ�����
	if (this->CON_To_Pro.size() != 0)
	{
		return;
	}
	// ����SELECT����
	for_Product
	{
		SELECT select = this->compute_SELECT(pi);
		this->setSELECT(pi, select);
	}
	//print(true, false);
	//// ����Ԥ�������
	//validate_grammar();
	compute_Prediction_And_Analysis_Table();
}

void LL1_Table_Gen::print(const bool SELECT_printer, const bool table_printer) const
{
	if (SELECT_printer)
	{
		string select_of_ep;
		for (auto pro_to_sel : this->Pro_To_SELECT)
		{
			if (pro_to_sel.first->getLeft() == "#")
			{
				select_of_ep = toString(pro_to_sel.second);
				continue;
			}
			cout << toString(pro_to_sel.second) << endl;
		}
		// ��EPSILON��SELECT���ŵ�������
		cout << select_of_ep;
	}
	if (table_printer)
	{
		cout << get_headline(true, false);
		for (string line : get_OutputStrings_of_PreAndAnaTable())
		{
			cout << line;
		}
		cout << endl;
	}
}

map<LL1_CON, PIterator> LL1_Table_Gen::get_CON_To_Pro() const
{
	return this->CON_To_Pro;
}

SELECT LL1_Table_Gen::compute_SELECT(PIterator & pi)
{
	// ��ʼ��һ��FIRST�����ɶ��󣬼������ʽ�Ҳ���FIRST��
	First_Gen fir_g = *this;
	string right = pi->getRight();
	FIRST first = fir_g.computeFIRST(right);
	TSet first_set = first.selfFIRST;
	// ������а�����epsilon
	if (contains_set(EP, first_set) != first_set.end())
	{
		NTER left = pi->getLeft();
		first_set.erase(EP);
		TSet follow_set = Follow_Gen::getFOLLOW(this->NTER_to_FOLLOW, left).selfFOLLOW;
		merge_set(first_set, follow_set);
	}
	return SELECT{ pi->toString(), first_set };
}

void LL1_Table_Gen::setSELECT(PIterator & pi, const SELECT select)
{
	pair<PIterator, SELECT> temp = pair<PIterator, SELECT>(pi, select);
	auto ret = this->Pro_To_SELECT.insert(temp);
	if (ret.second == false)
	{
		string message = "�ظ�����" + pi->toString() + "��SELECT��";
		throw exception(message.data());
	}
}

void LL1_Table_Gen::compute_Prediction_And_Analysis_Table()
{
	// �������в���ʽ��SELECT
	for (auto pro_to_sel : this->Pro_To_SELECT)
	{
		PIterator pro = pro_to_sel.first;
		// ������ǰ����ʽ��SELECT������Ԥ��������񣬲������
		for (auto t : pro_to_sel.second.t_set)
		{
			LL1_CON l_con = { pro->getLeft(), t };
			insert_con_to_pro(l_con, pro);
		}
	}
	// ���޶���ķ��ս��-�ս��������ϳ����־
	for_NTER
	{
		for_TER
		{
			NTER nter = *ni;
			TER ter = *ti;
			// ��EP�ս��ת��Ϊ'#'
			if (ter == EP)
			{
				ter = '#';
			}
			LL1_CON l_con = { nter, ter };
			try
			{
				insert_con_to_pro(l_con, NULL_PRO);
			}
			catch (const std::exception &e)
			{
				// ���ظ�����exception����
				string message = e.what();
				if (message.find("�������ʽ���Ƶ�") == string::npos)
				{
					throw e;
				}
			}
		}
	}
}

void LL1_Table_Gen::insert_con_to_pro(const LL1_CON con, PIterator pro)
{
	auto temp_pair = pair<LL1_CON, PIterator>(con, pro);
	auto ret = this->CON_To_Pro.insert(temp_pair);
	if (ret.second == false)
	{
		string message = con.row + "����" + con.col +
			"ʱ�ж������ʽ���Ƶ������ķ�������LL(1)�ķ�������������ʵ��﷨������\n";
		throw exception(message.data());
	}
}

string LL1_Table_Gen::toString(const SELECT select) const
{
	string output = SEL_FORM(select.product);
	for (auto t : select.t_set)
	{
		output += t;
		output += SET_DIV;
	}
	return output;
}

string LL1_Table_Gen::get_headline(const bool analysis_table, 
	const bool process_table) const
{
	if (process_table)
	{
		return fit_cube("����", SHORT_COL_INTER) + fit_cube("����ջ", LONG_COL_INTER) +
			fit_cube("���봮", LONG_COL_INTER) + fit_cube("�Ƶ����ò���ʽ��ƥ��", LONG_COL_INTER) + "\n";
	}
	else if (analysis_table)
	{
		// ��������ս����Ϊ��ͷ
		string headline = SHORT_COL_INTER;
		string last_nter = this->CON_To_Pro.begin()->first.row;
		for (auto con_to_pro : this->CON_To_Pro)
		{
			NTER cur_nter = con_to_pro.first.row;
			TER cur_ter = con_to_pro.first.col;
			if (last_nter != cur_nter)
			{
				break;
			}
			if (cur_ter == '#')
			{
				continue;
			}
			headline += fit_cube(string() + cur_ter, LONG_COL_INTER);
		}
		headline += fit_cube("#", LONG_COL_INTER) + "\n";
		return headline;
	}
	return string();
}

list<string> LL1_Table_Gen::get_OutputStrings_of_PreAndAnaTable() const
{
	list<string> output;
	NTER last_nter = "";
	string line, EP_cube, pro, start_line;
	for (auto con_to_pro : this->CON_To_Pro)
	{
		NTER cur_nter = con_to_pro.first.row;
		TER cur_ter = con_to_pro.first.col;
		PIterator pi = con_to_pro.second;
		// ������ս�������˸�����˵��һ�������γ����
		if (cur_nter != last_nter)
		{
			// ����ʼ�����б���������������
			if (last_nter == this->start)
			{
				start_line = line + EP_cube + "\n";
			}
			else
			{
				// ����#�����ݷ���һ�е�ĩβ
				output.push_back(line + EP_cube + "\n");
			}
			line = "";
			EP_cube = "";
			line += fit_cube(cur_nter, SHORT_COL_INTER);
		}
		// ����NULL_PRO�����
		if (pi != NULL_PRO)
		{
			pro = pi->toString();
		}
		else
		{
			// �ո��ʾ����
			//pro = "err";
			pro = "";
		}
		if (cur_ter == '#')
		{
			EP_cube = fit_cube(pro, LONG_COL_INTER);
			last_nter = cur_nter;
			continue;
		}
		line += fit_cube(pro, LONG_COL_INTER);
		last_nter = cur_nter;
	}
	if (start_line != "")
	{
		// ����Ѿ������˿�ʼ������
		output.push_back(line + EP_cube + "\n");
		// ������׵Ŀ����Լ���#����
		output.erase(output.begin());
		output.erase(output.begin());
		output.push_front(start_line);
	}
	else
	{
		// ������һ��Ϊ��ʼ������
		output.erase(output.begin());
		output.erase(output.begin());
		output.push_front(line + EP_cube + "\n");
	}
	return output;
}

#ifndef _TEST_USECASE_AUTOGEN_
LL1_Table_Gen::LL1_Table_Gen(const Follow_Gen & fol_g)
{
	this->ter_set = fol_g.getTSet();
	this->nter_set = fol_g.getNTSet();
	this->product_set = fol_g.getProductSet();
	this->start = this->product_set.begin()->getLeft();
	this->VnNum = this->nter_set.size();
	this->VtNum = this->ter_set.size();
	this->NTER_to_ifEP = fol_g.getNTER_to_ifEP();
	this->NTER_to_FIRST = fol_g.getNTER_to_FIRST();
	this->NTER_to_FOLLOW = fol_g.getNTER_to_FOLLOW();
}

LL1_Table_Gen::LL1_Table_Gen(const Parser & parser)
{
	this->ter_set = parser.getTSet();
	this->nter_set = parser.getNTSet();
	this->product_set = parser.getProductSet();
	this->NTER_to_ifEP = parser.getNTER_to_ifEP();
	this->start = this->product_set.begin()->getLeft();
	this->VnNum = this->nter_set.size();
	this->VtNum = this->ter_set.size();
}
#endif // !_TEST_USECASE_AUTOGEN_

#ifdef _TEST_USECASE_AUTOGEN_
LL1_Table_Gen::LL1_Table_Gen(const Follow_Gen & fol_g)
{
	this->ter_set = fol_g.getTSet();
	this->nter_set = fol_g.getNTSet();
	this->product_set = fol_g.getProductSet();
	this->start = this->product_set.begin()->getLeft();
	this->VnNum = this->nter_set.size();
	this->VtNum = this->ter_set.size();
	this->NTER_to_ifEP = fol_g.getNTER_to_ifEP();
	this->NTER_to_FIRST = fol_g.getNTER_to_FIRST();
	this->NTER_to_FOLLOW = fol_g.getNTER_to_FOLLOW();
	this->inputTable = fol_g.getInput();
}

LL1_Table_Gen::LL1_Table_Gen(const UseCase_AutoGen & UA)
{
	this->ter_set = UA.getTSet();
	this->nter_set = UA.getNTSet();
	this->product_set = UA.getProductSet();
	this->start = this->product_set.begin()->getLeft();
	this->VnNum = this->nter_set.size();
	this->VtNum = this->ter_set.size();
	this->NTER_to_ifEP = UA.getNTER_to_ifEP();
	this->inputTable = UA.getInput();
}

void LL1_Table_Gen::empty()
{
	UseCase_AutoGen::empty();
	this->NTER_to_FIRST.clear();
	this->NTER_to_FOLLOW.clear();
	this->Pro_To_SELECT.clear();
	this->CON_To_Pro.clear();
}

OutputTable LL1_Table_Gen::getOutput() const
{
	string headline = ANA_TABLE_WELCOME + this->get_headline(true, false);
	OutputTable outputTable = this->get_OutputStrings_of_PreAndAnaTable();
	// ��ǰ�������
	outputTable.push_front(headline);
	return outputTable;
}

void LL1_Table_Gen::validate_grammar() const
{
	if (this->Pro_To_SELECT.size() == 0)
	{
		throw exception("���ȼ���SELECT���ٽ����ķ���֤\n");
	}
	else
	{
		NTER last_left = "#", cur_left = "#";
		TSet sel_of_same_nter;
		for (auto pro_to_sel : this->Pro_To_SELECT)
		{
			NTER cur_left = pro_to_sel.first->getLeft();
			if (cur_left != last_left)
			{
				// �����ʼ������һ�����ս����SELECT
				// ������Ѿ���¼���ս��
				sel_of_same_nter.clear();
			}
			// ����ǰ����ʽSELECT�����ս��
			// ���뵽��ͬ�󲿵Ĳ���ʽSELECT���У��Լ���Ƿ����ظ�Ԫ��
			for (auto t : pro_to_sel.second.t_set)
			{
				auto ret = sel_of_same_nter.insert(t);
				// ������벻�ɹ�˵�����ֲ���ʽSELECT���н���
				if (ret.second == false)
				{
					string message = cur_left + "��������ʽ��SELECT���н���\
						�����ķ�������LL(1)�ķ�����ʹ������ķ���������\n";
					throw exception(message.data());
				}
			}
			last_left = cur_left;
		}
	}
}
#endif // _TEST_USECASE_AUTOGEN_
