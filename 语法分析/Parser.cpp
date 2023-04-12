#include "Parser.h"

#ifdef _UPTODATE_
void Parser::collectT(ProductSet product_set)
{
	PIterator spi = product_set.begin();
	while (spi != product_set.end())
	{
		string right = (*spi).getRight();
		// �����Ҳ�
		string::iterator si = right.begin();
		while (si < right.end())
		{
			// �����ǰ�ַ�Ϊ�ս���ַ�
			if (isTer(*si))
			{
				auto ret = this->ter_set.find(*si);
				// ���û���ҵ����¼
				if (ret == this->ter_set.end())
				{
					this->ter_set.insert(*si);
				}
			}
			// ����������ս���ķ���������
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
		// ������
		auto ret = this->nter_set.find(left);
		// ������ս��������û�м�¼�÷��ս��
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
		// �����ǰ�ַ����ս����ֱ�ӷ���
		if (isTer(curChar))
		{
			if (this->ter_set.count(curChar) == 0)
			{
				string message = "��鵽�Ƿ��ַ�";
				message.push_back(curChar);
				message.push_back('\n');
				throw exception(message.c_str());
			}
			return TOKEN(curChar, string());
		}
		// ��֮��ķ��ս���ϵ�һ��string�з���
		string nter = string("");
		while (!isTer(curChar))
		{
			nter.push_back(curChar);
			// ����ҵ����ַ��������е��ս��
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
			string message = "�Ƿ����ս��" + nter;
			message.push_back('\n');
			throw exception(message.c_str());
		}
		return TOKEN(NULL, nter);
	}
	throw exception("�ַ�������Խ��\n");
}

void Parser::validate_grammar() const
{
	// ����Ƿ����������޹��ķ�
	NIterator nit = this->nter_set.begin();
	NIterator end = this->nter_set.end();
	while (nit != end)
	{
		for (int i = 0; i < nit->length(); i++)
		{
			// ����������ս���а����ս��
			// ��˵�����������й��ķ�
			if (isTer(nit->at(i)))
			{
				throw exception("�������������޹��ķ�\n");
			}
		}
		nit++;
	}
	// ������з��ս���Ƿ��в���ʽ
	PIterator p_start = this->product_set.begin();
	PIterator p_end = this->product_set.end();
	while (p_start != p_end)
	{
		// ������ʽ�Ҳ��Ƿ��зǷ��ַ�
		string right = p_start->getRight();
		for (int i = 0; i < right.length(); i++)
		{
			this->getNextToken(right, i);
		}
		p_start++;
	}
	// ���û���ս�����׳��쳣
	if (this->ter_set.size() == 1)
	{
		throw exception("���ķ�û���ս��\n");
	}
}

bool Parser::deduceEpsilon(NTER nter, set<NTER>& skip)
{
	PIterator start, end, pi;
	this->findProduct_OfANter(nter, start, end);
	// �������в���ʽ
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
			// ����������ս�����������epsilon�ͼ����һ����ʽ
			if (ret.first != NULL)
			{
				if (ret.first == '~')
				{
					return true;
				}
				hasEP = false;
				break;
			}
			// ��������ķ��ս���ܷ��Ƶ���epsilon
			// ǰ���ǲ���Ҫ�����÷��ս����������ݹ��������ѭ��
			if (skip.count(ret.second) == 0)
			{
				skip.insert(ret.second);
				auto can = deduceEpsilon(ret.second, skip);
				skip.erase(ret.second);
				if (!can)
				{
					// �����Ƶ���epsilon��������һ������ʽ
					hasEP = false;
					break;
				}
				// ���Ƶ���epsilon���������¸�����
			}
			else
			{
				// ��Ϊ��㺯���Ѿ������������
				// ���������ʽ�ܹ��Ƶ���epsilon
				// ���ʾsһ��ͨ��������ֱ���Ƶ��õ���
				// ��������ֱ��������һ������ʽ
				hasEP = false;
				break;
			}
			iterator++;
		}
		// ������˸ò���ʽ��û���Ƴ��ս��
		if (hasEP)
		{
			return true;
		}
		pi++;
	}
	// ���в���ʽ��û���Ƴ�epsilon
	return false;
}

void Parser::findProduct_OfANter(const NTER nter,
	PIterator & start, PIterator & end) const
{
	start = this->product_set.begin();
	PIterator set_end = this->product_set.end();
	// ������Ѱ�ҡ�#���Ĳ���ʽ
	if (nter == "#")
	{
		start = set_end;
		end = start;
		return;
	}
	while (start != set_end && start->getLeft() != nter) start++;
	end = start;
	while (end != set_end && end->getLeft() == nter) end++;
	// ���û�ҵ�����ʽ
	if (start == set_end)
	{
		string message = "δ�ҵ�" + nter + "�Ĳ���ʽ";
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
	printf("��������A->abc�Ĳ���ʽ������~��ʾ�գ��������ֹͣ���룺 \n");
	int id_pro = 0;
	string temp;
	while (getline(cin, temp), temp != "")
	{
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
		id_pro++;
	}
}

void Parser::init()
{
	// ����Ѿ���ʼ������
	if (this->ter_set.size() != 0)
	{
		return;
	}
	// ��'#'���뵽ʽ����
	this->ter_set.insert(EP);
	this->nter_set.insert(string("#"));
	this->collectNonT(this->product_set);
	this->collectT(this->product_set);
	// ������ķ��ĺϷ���
	this->validate_grammar();
}

void Parser::compute()
{
	if (this->NTER_to_ifEP.size() != 0)
	{
		return;
	}
	// �������ս�����ϣ��������з��ս���Ƿ����Ƶ���epsilon
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
	cout << "�û�������Ĳ���ʽΪ��" << endl;
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
		cout << "���ķ����ս��������";
		for_TER
		{
			cout << ' ' << *ti;
		}
		cout << endl << "���ķ��ķ��ս��������";
		for_NTER
		{
			cout << ' ' << *ni;
		}
		cout << endl;
	}
	// �������ʽ
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