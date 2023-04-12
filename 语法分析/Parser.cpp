#include "Parser.h"

#ifdef _UPTODATE_
void Parser::collectT(ProductSet product_set)
{
	PIterator spi = product_set.begin();
	while (spi != product_set.end())
	{
		string right = (*spi).getRight();
		// 处理右部
		string::iterator si = right.begin();
		while (si < right.end())
		{
			// 如果当前字符为终结符字符
			if (isTer(*si))
			{
				auto ret = this->ter_set.find(*si);
				// 如果没有找到则记录
				if (ret == this->ter_set.end())
				{
					this->ter_set.insert(*si);
				}
			}
			// 如果所属非终结符的符号则跳过
			si++;
		}
		spi++;
	}
	this->VtNum = this->ter_set.size();
}

void Parser::collectNonT(ProductSet product_set)
{
	ProductSet::iterator spi = product_set.begin();
	this->start = spi->getLeft();
	while (spi != product_set.end())
	{
		NTER left = spi->getLeft();
		// 处理左部
		auto ret = this->nter_set.find(left);
		// 如果非终结符集合中没有记录该非终结符
		if (ret == this->nter_set.end())
		{
			this->nter_set.insert(left);
			this->VnNum++;
		}
		spi++;
	}
	this->VnNum = this->nter_set.size();
}

TOKEN Parser::getNextToken(const string & source, int & iterator) const
{
	int length = source.length();
	while (iterator < length)
	{
		char curChar = source.at(iterator);
		// 如果当前字符是终结符则直接返回
		if (isTer(curChar))
		{
			if (this->ter_set.count(curChar) == 0)
			{
				string message = "检查到非法字符";
				message.push_back(curChar);
				message.push_back('\n');
				throw exception(message.c_str());
			}
			return TOKEN(curChar, string());
		}
		// 将之后的非终结符合到一个string中返回
		string nter = string("");
		while (!isTer(curChar))
		{
			nter.push_back(curChar);
			// 如果找到的字符串是现有的终结符
			if (this->nter_set.count(nter) != 0)
			{
				break;
			}
			iterator++;
			try
			{
				curChar = source.at(iterator);
			}
			catch (const std::exception& e)
			{
				break;
			}
		}
		if (this->nter_set.count(nter) == 0)
		{
			string message = "非法非终结符" + nter;
			message.push_back('\n');
			throw exception(message.c_str());
		}
		return TOKEN(NULL, nter);
	}
	throw exception("字符串遍历越界\n");
}

void Parser::validate_grammar() const
{
	// 检查是否是上下文无关文法
	NIterator nit = this->nter_set.begin();
	NIterator end = this->nter_set.end();
	while (nit != end)
	{
		for (int i = 0; i < nit->length(); i++)
		{
			// 如果检查出非终结符中包含终结符
			// 则说明是上下文有关文法
			if (isTer(nit->at(i)))
			{
				throw exception("请输入上下文无关文法\n");
			}
		}
		nit++;
	}
	// 检查所有非终结符是否都有产生式
	PIterator p_start = this->product_set.begin();
	PIterator p_end = this->product_set.end();
	while (p_start != p_end)
	{
		// 检查产生式右部是否有非法字符
		string right = p_start->getRight();
		for (int i = 0; i < right.length(); i++)
		{
			this->getNextToken(right, i);
		}
		p_start++;
	}
	// 如果没有终结符则抛出异常
	if (this->ter_set.size() == 1)
	{
		throw exception("该文法没有终结符\n");
	}
}

bool Parser::deduceEpsilon(NTER nter, set<NTER>& skip)
{
	PIterator start, end, pi;
	this->findProduct_OfANter(nter, start, end);
	// 遍历所有产生式
	pi = start;
	while (pi != end)
	{
		string right = pi->getRight();
		int iterator = 0;
		int length = right.length();
		bool hasEP = true;
		while (iterator < length)
		{
			auto ret = getNextToken(right, iterator);
			// 如果遇到了终结符，如果不是epsilon就检查下一产生式
			if (ret.first != NULL)
			{
				if (ret.first == '~')
				{
					return true;
				}
				hasEP = false;
				break;
			}
			// 检查遇到的非终结符能否推导出epsilon
			// 前提是不需要跳过该非终结符，避免左递归带来的死循环
			if (skip.count(ret.second) == 0)
			{
				skip.insert(ret.second);
				auto can = deduceEpsilon(ret.second, skip);
				skip.erase(ret.second);
				if (!can)
				{
					// 不能推导出epsilon则跳到下一个产生式
					hasEP = false;
					break;
				}
				// 能推导出epsilon则继续检查下个符号
			}
			else
			{
				// 因为外层函数已经处理过本符号
				// 如果本产生式能够推导出epsilon
				// 则表示s一定通过其他的直接推导得到ε
				// 所以这里直接跳到下一个产生式
				hasEP = false;
				break;
			}
			iterator++;
		}
		// 检查完了该产生式，没能推出终结符
		if (hasEP)
		{
			return true;
		}
		pi++;
	}
	// 所有产生式都没能推出epsilon
	return false;
}

void Parser::findProduct_OfANter(const NTER nter,
	PIterator & start, PIterator & end) const
{
	start = this->product_set.begin();
	PIterator set_end = this->product_set.end();
	// 不主动寻找“#”的产生式
	if (nter == "#")
	{
		start = set_end;
		end = start;
		return;
	}
	while (start != set_end && start->getLeft() != nter) start++;
	end = start;
	while (end != set_end && end->getLeft() == nter) end++;
	// 如果没找到产生式
	if (start == set_end)
	{
		string message = "未找到" + nter + "的产生式";
		throw exception(message.data());
	}
}

Parser::Parser()
{
	this->VnNum = 0;
	this->VtNum = 0;
}

Parser::Parser(const Parser & parser)
{
	this->ter_set = parser.getTSet();
	this->nter_set = parser.getNTSet();
	this->NTER_to_ifEP = parser.getNTER_to_ifEP();
	this->product_set = parser.getProductSet();
	this->VnNum = this->nter_set.size();
	this->VtNum = this->ter_set.size();
}

void Parser::read()
{
	printf("输入形如A->abc的产生式（其中~表示空）输入空行停止输入： \n");
	int id_pro = 0;
	string temp;
	while (getline(cin, temp), temp != "")
	{
		Product ps;
		// 将产生式的左部提取出来赋值给ps的左部
		ps.setLeft(ps.recLeft(temp));
		if (temp == "")
		{
			throw exception("空右部应使用~表示\n");
		}
		ps.setRight(temp);
		ps.setId(id_pro);
		this->product_set.push_back(ps);
		id_pro++;
	}
}

void Parser::init()
{
	// 如果已经初始化过了
	if (this->ter_set.size() != 0)
	{
		return;
	}
	// 将'#'加入到式子中
	this->ter_set.insert(EP);
	this->nter_set.insert(string("#"));
	this->collectNonT(this->product_set);
	this->collectT(this->product_set);
	// 检验该文法的合法性
	this->validate_grammar();
}

void Parser::compute()
{
	if (this->NTER_to_ifEP.size() != 0)
	{
		return;
	}
	// 遍历非终结符集合，计算所有非终结符是否能推导出epsilon
	for_NTER
	{
		set<NTER> skip;
		skip.insert(*ni);
		pair<NTER, bool> temp1 = pair<NTER, bool>(*ni, deduceEpsilon(*ni, skip));
		this->NTER_to_ifEP.insert(temp1);
	}
}

void Parser::showProducts() const
{
	cout << "用户所输入的产生式为：" << endl;
	int index = 1;
	for_Product
	{
		string left = pi->getLeft();
		string right = pi->getRight();
		cout << "(" << index << ")" << "\t" <<
			left << "->" << right << endl;
		index++;
	}
}

void Parser::print(bool vtvn_printer) const
{
	if (vtvn_printer)
	{
		cout << "该文法的终结符包括：";
		for_TER
		{
			cout << ' ' << *ti;
		}
		cout << endl << "该文法的非终结符包括：";
		for_NTER
		{
			cout << ' ' << *ni;
		}
		cout << endl;
	}
	// 输出产生式
	this->showProducts();
}

TSet Parser::getTSet() const
{
	return this->ter_set;
}

NTSet Parser::getNTSet() const
{
	return this->nter_set;
}

ProductSet Parser::getProductSet() const
{
	return this->product_set;
}

map<NTER, bool> Parser::getNTER_to_ifEP() const
{
	return this->NTER_to_ifEP;
}

#endif // _UPTODATE_