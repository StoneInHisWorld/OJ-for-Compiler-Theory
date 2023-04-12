#include "FirstSet_Generator.h"

void First_Gen::insert_TER_to_FIRST(TER ter, NTER nter)
{
	try
	{
		// 注意看下this中的FIRST集是否有改变
		FIRST *old = &this->NTER_to_FIRST.find(nter)->second;
		old->selfFIRST.insert(ter);
	}
	catch (const std::exception &e)
	{
		cout << "找不到" + nter + "的FIRST映射";
	}
}

First_Gen::First_Gen()
{}

map<NTER, FIRST> First_Gen::getNTER_to_FIRST() const
{
	return this->NTER_to_FIRST;
}

#ifdef _UPTODATE_
void First_Gen::init()
{
	Parser::init();
	Parser::compute();
}

void First_Gen::compute()
{
	if (this->NTER_to_FIRST.size() != 0)
	{
		return;
	}
	// 对每个非终结符进行单独计算FISRT集
	for_NTER
	{
		NTER nter = *ni;
		// 字符串输入终止符FIRST集为空
		if (nter == "#")
		{
			FIRST first = { nter, TSet() };
			setFIRST(nter, first);
			continue;
		}
		set<NTER> skip;
		FIRST first = { nter, TSet() };
		computeFIRST(nter, first, skip);
		setFIRST(nter, first);
	}
}

void First_Gen::print(bool vtvn_printer, bool Parser_printer)
{
	if (Parser_printer)
	{
		Parser::print(vtvn_printer);
	}
	string *output_string = new string[this->VnNum];
	// 0号位留给第一个输入的产生式的左部
	int iterator = 1;
	for_NTER
	{
		string temp = FIR_FORM(*ni);
		for_FIRST(this->get_FIRST(*ni))
		{
			temp.push_back(*fir_i);
			temp += SET_DIV;
		}
		temp.push_back('\n');
		// 如果是第一个输入的非终结符
		if (*ni == this->start)
		{
			output_string[0] = temp;
		}
		// 如果是“#”则放到最后一个
		else if (*ni == "#")
		{
			output_string[this->VnNum - 1] = temp;
		}
		else
		{
			output_string[iterator] = temp;
			iterator++;
		}
	}
	for (int i = 0; i < this->VnNum; i++)
	{
		cout << output_string[i];
	}
	cout << endl;
}

FIRST First_Gen::computeFIRST(const string right) const
{
	FIRST first;
	NTSet skip;
	this->computeFIRST(right, first, skip);
	return first;
}

void First_Gen::computeFIRST(const string right, FIRST &first, set<NTER> &skip) const
{
	// 遍历右部
	for (int si = 0; si < right.length(); si++)
	{
		auto curToken = this->getNextToken(right, si);
		// 如果是终结符
		if (curToken.first != NULL)
		{
			first.selfFIRST.insert(curToken.first);
			break;
		}
		// 如果是非终结符
		else
		{
			NTER nter = curToken.second;
			// 如果外层尚未处理过
			if (skip.count(nter) == 0)
			{
				skip.insert(nter);
				// 遍历其每个产生式寻找FIRST集
				PIterator start, end;
				this->findProduct_OfANter(nter, start, end);
				while (start != end)
				{
					computeFIRST(start->getRight(), first, skip);
					first.selfFIRST.erase(EP);
					start++;
				}
				skip.erase(nter);
			}
			// 处理过则跳过
			if (!nullable(nter))
			{
				// 如果不可以推出epsilon则返回
				break;
			}
			else
			{
				// 如果si是最后一个，则加入EP
				if (si == right.length() - 1)
				{
					first.selfFIRST.insert(EP);
				}
			}
		}
	}
	first.nter = right;
}
#endif // _UPTODATE_

#ifndef _TEST_USECASE_AUTOGEN_
First_Gen::First_Gen(const Parser & parser)
{
	this->ter_set = parser.getTSet();
	this->nter_set = parser.getNTSet();
	this->product_set = parser.getProductSet();
	this->start = this->product_set.begin()->getLeft();
	this->VnNum = this->nter_set.size();
	this->VtNum = this->ter_set.size();
	this->NTER_to_ifEP = parser.getNTER_to_ifEP();
}
#endif // !_TEST_USECASE_AUTOGEN_


#ifdef _TEST_USECASE_AUTOGEN_
First_Gen::First_Gen(const UseCase_AutoGen & UA)
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

void First_Gen::empty()
{
	UseCase_AutoGen::empty();
	this->NTER_to_FIRST.clear();
}

OutputTable First_Gen::getOutput() const
{
	OutputTable outputTable;
	string *output_string = new string[this->VnNum];
	// 0号位留给第一个输入的产生式的左部
	int iterator = 1;
	for_NTER
	{
		string temp = FIR_FORM(*ni);
		for_FIRST(this->get_FIRST(*ni))
		{
			temp.push_back(*fir_i);
			temp += SET_DIV;
		}
		temp.push_back('\n');
		// 如果是第一个输入的非终结符
		if (*ni == this->start)
		{
			output_string[0] = temp;
		}
		// 如果是“#”则放到最后一个
		else if (*ni == "#")
		{
			output_string[this->VnNum - 1] = temp;
		}
		else
		{
			output_string[iterator] = temp;
			iterator++;
		}
	}
	for (int i = 0; i < this->VnNum; i++)
	{
		outputTable.push_back(output_string[i]);
	}
	return outputTable;
}
#endif // _TEST_USECASE_AUTOGEN_