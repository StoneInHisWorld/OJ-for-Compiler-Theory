#include "FollowSet_Generator.h"
#include "Follow_Graph.h"

map<NTER, FIRST> Follow_Gen::getNTER_to_FIRST() const
{
	return this->NTER_to_FIRST;
}

void Follow_Gen::draw_RelationGraph()
{
	PIterator start = this->product_set.begin();
	PIterator end = this->product_set.end();
	// 构造FOLLOW集关系图
	// 首先添加弧 FOLLOW(S) -> '#'
	VNode *follow_s = this->FG.insertVertex(Follow, NULL, start->getLeft());
	VNode *ter_ep = this->FG.insertVertex(T, '#', string(""));
	this->FG.addArc(follow_s, ter_ep);
	while (start != end)
	{
		this->draw_graph_ofAPro(*start);
		start++;
	}
	//  加入所有FIRST集的终结符，将其作为结点加入
	VNode *v = this->FG.getAdjList()->next;
	while (v != NULL)
	{
		// 如果该顶点是FIRST集顶点
		if (v->type == First)
		{
			draw_graph_ofAFIRST(v);
		}
		v = v->next;
	}
}

void Follow_Gen::draw_graph_ofAPro(Product product)
{
	NTER nter = product.getLeft();
	string right = product.getRight();
	// 存储需要与当前结点连成弧的弧尾结点
	set<VNode *> tails;
	tails.insert(freeNode(Follow, NULL, nter));
	for (int i = right.length() - 1; i >= 0; i--)
	{
		auto ret = this->getNextToken(right, i);
		// 如果是终结符
		if (ret.first != NULL)
		{
			char ret_c = ret.first;
			tails.clear();
			tails.insert(freeNode(T, ret_c, string("")));
		}
		// 如果是非终结符
		else
		{
			NTER ret_nt = ret.second;
			VNode *curNter = this->FG.insertVertex(Follow, NULL, ret_nt);
			// 连接curNter到所有tails中的结点
			auto t = tails.begin();
			auto end = tails.end();
			while (t != end)
			{
				// 将tails中的自由结点插入到图中
				VNode *freeNode = *t;
				VNode *tail = this->FG.insertVertex(freeNode->type, 
					freeNode->infor->first, freeNode->infor->second);
				try
				{
					this->FG.addArc(curNter, tail);
				}
				catch (const std::exception &e)
				{
					// 遇到了递归的情况，不生成弧
				}
				t++;
			}
			// 如果不可以推导出epsilon，则清空弧尾结点
			if (!nullable(ret_nt))
			{
				tails.clear();
			}
			tails.insert(freeNode(First, NULL, ret_nt));
		}
	}
}

void Follow_Gen::draw_graph_ofAFIRST(VNode * v)
{
	if (v->type != First)
	{
		throw exception("为非First结点构造关系图");
	}
	NTER nter = v->infor->second;
	// 将nter的FIRST中的所有元素生成结点加入到图中
	for_FIRST(First_Gen::get_FIRST(this->NTER_to_FIRST, nter))
	{
		TER ter = *fir_i;
		if (ter == EP)
		{
			//fir_i++;
			continue;
		}
		VNode *tail = this->FG.insertVertex(T, ter, string(""));
		this->FG.addArc(v, tail);
	}
}

FOLLOW Follow_Gen::compute_FOLLOW(NTER nter)
{
	// 计算跳过“#”
	if (nter == "#")
	{
		return { string("#"), set<TER>() };
	}
	//FOLLOW follow = getFOLLOW(nter);
	// 初始化一个空FOLLOW集
	FOLLOW follow = { nter, TSet() };
	VNode *v = this->FG.getVertex(Follow, NULL, nter);
	if (v == NULL)
	{
		string message = "图中没有" + nter + "的FOLLOW集";
		throw exception(message.data());
	}
	this->FG.DFS(v, follow);
	return follow;
}

Follow_Gen::Follow_Gen()
{
}

void Follow_Gen::init()
{
	Parser::init();
	// 如果没有计算过FIRST集
	if (this->NTER_to_FIRST.size() == 0)
	{
		First_Gen fir_g = *this;
		fir_g.init();
		fir_g.compute();
		*this = fir_g;
	}
	this->FG = Follow_Graph();
}

void Follow_Gen::compute()
{
	// 如果已经计算过了则不再重复计算
	if (this->NTER_to_FOLLOW.size() != 0)
	{
		return;
	}
	this->draw_RelationGraph();
	// 计算FOLLOW集
	for_NTER
	{
		NTER nter = *ni;
		FOLLOW follow = this->compute_FOLLOW(nter);
		setFOLLOW(nter, follow);
	}
}

void Follow_Gen::print(bool Parser_printer, bool Graph_painter)
{
	if (Parser_printer)
	{
		Parser::print(true);
	}
	if (Graph_painter)
	{
		this->FG.printGraph();
	}
	string *output_string = new string[this->VnNum];
	// 0号位留给第一个输入的产生式的左部
	int iterator = 1;
	for_NTER
	{
		string temp = FOL_FORM(*ni);
		for_FOLLOW(this->getFOLLOW(*ni))
		{
			temp.push_back(*fol_i);
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

map<NTER, FOLLOW> Follow_Gen::getNTER_to_FOLLOW() const
{
	return this->NTER_to_FOLLOW;
}

#ifndef _TEST_USECASE_AUTOGEN_
Follow_Gen::Follow_Gen(const First_Gen & fir_g)
{
	this->ter_set = fir_g.getTSet();
	this->nter_set = fir_g.getNTSet();
	this->product_set = fir_g.getProductSet();
	this->start = this->product_set.begin()->getLeft();
	this->VnNum = this->nter_set.size();
	this->VtNum = this->ter_set.size();
	this->NTER_to_ifEP = fir_g.getNTER_to_ifEP();
	this->NTER_to_FIRST = fir_g.getNTER_to_FIRST();
}

Follow_Gen::Follow_Gen(const Parser & parser)
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
Follow_Gen::Follow_Gen(const First_Gen & fir_g)
{
	this->ter_set = fir_g.getTSet();
	this->nter_set = fir_g.getNTSet();
	this->product_set = fir_g.getProductSet();
	this->start = this->product_set.begin()->getLeft();
	this->VnNum = this->nter_set.size();
	this->VtNum = this->ter_set.size();
	this->NTER_to_ifEP = fir_g.getNTER_to_ifEP();
	this->NTER_to_FIRST = fir_g.getNTER_to_FIRST();
	this->inputTable = fir_g.getInput();
}

Follow_Gen::Follow_Gen(const UseCase_AutoGen & UA)
{
	this->ter_set = UA.getTSet();
	this->nter_set = UA.getNTSet();
	this->NTER_to_ifEP = UA.getNTER_to_ifEP();
	this->product_set = UA.getProductSet();
	this->start = this->product_set.begin()->getLeft();
	this->VnNum = this->nter_set.size();
	this->VtNum = this->ter_set.size();
	this->inputTable = UA.getInput();
}

void Follow_Gen::empty()
{
	UseCase_AutoGen::empty();
	this->NTER_to_FIRST.clear();
	this->NTER_to_FOLLOW.clear();
}

OutputTable Follow_Gen::getOutput() const
{
	OutputTable outputTable;
	string *output_string = new string[this->VnNum];
	// 0号位留给第一个输入的产生式的左部
	int iterator = 1;
	for_NTER
	{
		string temp = FOL_FORM(*ni);
		for_FOLLOW(this->getFOLLOW(*ni))
		//for_FOLLOW(*ni)
		{
			temp.push_back(*fol_i);
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