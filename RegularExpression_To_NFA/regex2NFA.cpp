#include "declaration.h"

Regex2Nfa::Regex2Nfa() 
{
	alphaBet = std::string();
	char tempChar;
	// 将字母表初始化为：[A,B,C,...,a,b,c,...,0,1,2,...]
	for (int i = 'A'; i < 'Z' + 1; i++)
	{
		tempChar = char(i);
		alphaBet.push_back(tempChar);
		tempChar = char(i + 'a' - 'A');
		alphaBet.push_back(tempChar);
	}
	for (int i = '0'; i < '9' + 1; i++)
	{
		tempChar = char(i);
		alphaBet.push_back(tempChar);
	}
	op_priority['*'] = 4;
	op_priority['|'] = 3;
	op_priority[explicitC] = 2;
	op_priority['('] = 0;
}

void Regex2Nfa::getInput()
{
	std::cin >> this->input;
}

void Regex2Nfa::setInput(const std::string & inputT)
{
	input = inputT;
}

void Regex2Nfa::outputOrigin()
{
	for (const State *state : nfa.states)
	{
		if (state == nfa.states.back())
		{
			continue;
		}
		std::cout << state->ID << " ";

		for (State *const state_item : state->transitions_e)
		{
			std::cout << state->ID << "-~->" << state_item->ID << " ";
		}

		for (std::pair<const char, State *> pairTemp : state->transitions)
		{
			std::cout << state->ID << "-" <<
				pairTemp.first << "->" << pairTemp.second->ID << " ";
		}
		std::cout << std::endl;
	}
}

void Regex2Nfa::output()
{
	for (const State *state : nfa.states)
	{
		// 第一个状态为开始状态
		if (state == nfa.states.front())
		{
			cout << "X ";
			// 输出所有epsilon转换
			for (State *const state_item : state->transitions_e)
			{
				// 如果下一状态是最终状态
				if (state_item->ID == nfa.states.back()->ID)
				{
					cout << "X-~->Y";
				}
				else
				{
					cout << "X-~->" << state_item->ID << " ";
				}
			}
			// 将所有字符转换输出
			for (std::pair<const char, State *> pairTemp : state->transitions)
			{
				if (pairTemp.second->ID == nfa.states.back()->ID)
				{
					cout << "X-" <<
						pairTemp.first << "->Y";
				}
				else
				{
					std::cout << "X-" <<
						pairTemp.first << "->" << pairTemp.second->ID << " ";
				}
			}
			// 开始状态后输出最终状态
			std::cout << std::endl << "Y" << std::endl;
		}
		// 最终状态不输出
		else if (state == nfa.states.back())
		{
			continue;
		}
		else
		{
			std::cout << state->ID << " ";
			// 输出epsilon转换
			for (State *const state_item : state->transitions_e)
			{
				if (state_item->ID == nfa.states.back()->ID)
				{
					cout << state->ID << "-" <<
						'~' << "->Y ";
				}
				else
				{
					cout << state->ID << "-" <<
						'~' << "->" << state_item->ID << " ";
				}
			}
			// 输出字符转换
			for (pair<const char, State *> pairTemp : state->transitions)
			{

				if (pairTemp.second->ID == nfa.states.back()->ID)
				{
					std::cout << state->ID << "-" <<
						pairTemp.first << "->" << "Y";
				}
				else
				{
					std::cout << state->ID << "-" <<
						pairTemp.first << "->" << pairTemp.second->ID << " ";
				}
			}
			cout << endl;
		}

	}
}

void Regex2Nfa::insertExplicit()
{
	strBeInsert = input;
	bool shouldInsert = false;
	int count = 0;  // 用于标记当前扫描到的字符
	for (unsigned int i = 0; i < input.length() - 1; i++, count++)
	{
		// 对于如ab,a(c),a*(c),(a)b,a*b,五种状态
		// 替换成a_b,a_(c),a*_(c),(a)_b,a*_b
		if (isCharInStr(alphaBet, strBeInsert[count]) &&
			(isCharInStr(alphaBet, strBeInsert[count + 1]) || strBeInsert[count + 1] == '('))
		{
			// 对于两个字母相邻或者字母+'('，插入分隔
			shouldInsert = true;
		}
		else if (strBeInsert[count] == '*' &&
			strBeInsert[count + 1] == '(')
		{
			// 对于字母或者'*'与'('相邻，插入分隔
			shouldInsert = true;
		}
		else if (strBeInsert[count] == ')' && isCharInStr(alphaBet, strBeInsert[count + 1]))
		{
			// 对于')'和字母相邻，插入分隔
			shouldInsert = true;
		}
		else if (strBeInsert[count] == '*' && isCharInStr(alphaBet, strBeInsert[count + 1]))
		{
			// 对于*和字母相邻，插入分隔
			shouldInsert = true;
		}

		if (shouldInsert)
		{
			strBeInsert.insert(count + 1, std::string(1, explicitC));
			count++;
			shouldInsert = false;
		}
	}
}

void Regex2Nfa::convertToPostfix()
{
	string outputPostfix;
	vector<char> op_stack;
	char tempChar;

	//    转后缀表达式
	//    1. 遇到操作数，直接输出；
	//    2. 栈为空时，遇到运算符，入栈；
	//    3. 遇到左括号，将其入栈；
	//    4. 遇到右括号，执行出栈操作，并将出栈的元素输出，直到弹出栈的是左括号，左括号不输出；
	//    5. 遇到其他运算符“+”“-”“*”“/”时，弹出所有优先级大于或等于该运算符的栈顶元素，然后将该运算符入栈；
	//    6. 最终将栈中的元素依次出栈，输出。
	//    举例：a+b*c+(d*e+f)g ———> abc*+de*f+g*+
	for (char ch : strBeInsert)
	{
		// 如果是操作数，则进入输出字符串
		if (isCharInStr(alphaBet, ch))
		{
			outputPostfix.push_back(ch);
		}
		// 运算符
		else
		{
			// 栈空或'('则入栈
			if (op_stack.empty() || ch == '(')
			{
				op_stack.push_back(ch);
			}
			else if (ch == ')')
			{
				// 将所有符号推入输出字符串，弹出栈，直到遇到'('
				while (op_stack.back() != '(')
				{
					tempChar = op_stack.back();
					outputPostfix.push_back(tempChar);
					op_stack.pop_back();
				}
				op_stack.pop_back();
			}
			else
			{
				// 将所有优先级大于等于本运算符的推入输出字符串，弹出栈
				while (!op_stack.empty() && op_priority[ch] <= op_priority[op_stack.back()])
				{
					tempChar = op_stack.back();
					outputPostfix.push_back(tempChar);
					op_stack.pop_back();
				}
				// 本运算符入栈
				op_stack.push_back(ch);
			}
		}
	}
	// 遍历完成后将所有栈内运算符推入输出字符串
	while (!op_stack.empty())
	{
		tempChar = op_stack.back();
		outputPostfix.push_back(tempChar);
		op_stack.pop_back();
	}
	strPostfix = outputPostfix;
}

void Regex2Nfa::constructToNFA()
{
	vector<Fragment *> stack_frag;
	int stateId = -1;

	// 临时变量
	int i_t;
	State *state_start;
	State *state_end;
	Fragment *frag_start;
	Fragment *frag_end;
	Fragment *newFragment;

	for (const char ch : strPostfix)
	{
		switch (ch)
		{
		case '*':
			// 闭包：遇到*，生成多个运算
			// 将 f2:s2->s3转变为 fn:s2-e->s3, s3-a->s3, s3-e->s4
			// 将栈顶的fragment转换为闭包
			frag_end = stack_frag.back();
			stack_frag.pop_back();
			// 将end状态作为start状态epsilon转换的下一状态
			frag_end->state_start->transitions_e.push_back(frag_end->state_end);
			// 遍历start状态的所有转换，将所有转换的下一状态替换为end状态
			// 将所有转换作为end状态的转换
			for (pair<const char, State *> &pairTemp : frag_end->state_start->transitions)
			{
				pairTemp.second = frag_end->state_end;
				frag_end->state_end->transitions.insert(pairTemp);
			}
			// 清除start状态的所有转换
			frag_end->state_start->transitions.clear();
			frag_end->state_end->isEnd = false;
			// 生成一个新状态
			stateId = getMaxStateID(nfa) + 1;
			state_end = new State(stateId, true);
			stateId++;
			nfa.states.push_back(state_end);
			// 加入一个新的epsilon转换，从end状态转换到新状态
			// 将新状态作为end状态的可转换状态
			frag_end->state_end->transitions_e.push_back(state_end);
			frag_end->state_end = state_end;

			stack_frag.push_back(frag_end);
			break;
		case '|':
			// union 遇到或
			// fragment栈顶及栈顶下一个为“或”运算的操作fragment，将两个fragment取出
			frag_end = stack_frag.back();
			stack_frag.pop_back();
			frag_start = stack_frag.back();
			stack_frag.pop_back();
			// 遍历frag_end的start状态的转换，将每个转换的下一状态更换为frag_start的end状态
			// 将每个转换加入frag_start的start状态转换中
			for (std::pair<const char, State *> it : frag_end->state_start->transitions)
			{
				it.second = frag_start->state_end;
				frag_start->state_start->transitions.insert(it);
			}

			nfa.removeState(frag_end->state_start);
			nfa.removeState(frag_end->state_end);

			stateId = getMaxStateID(nfa) + 1;
			// 将两个fragment合二为一
			newFragment = new Fragment(frag_start->state_start, frag_start->state_end);
			stack_frag.push_back(newFragment);
			delete frag_end;
			delete frag_start;
			break;
		case DIV_CHAR:
			// concat 直接连接两个fragment
			// 将 f_s:s1->s2,f_e:s3->s4
			// 变成 f_n:s1->s2->s4
			// 将分隔符连接的两个fragment提取出来
			frag_end = stack_frag.back();
			stack_frag.pop_back();
			frag_start = stack_frag.back();
			stack_frag.pop_back();
			// 将frag_end中的start状态的转换加入到frag_start的end状态
			frag_start->state_end->transitions = frag_end->state_start->transitions;
			// 将frag_end中的start状态能进行的转换加入到frag_start的end状态中
			frag_start->state_end->transitions_e = frag_end->state_start->transitions_e;
			// 移除frag_end中的state_start
			nfa.removeState(frag_end->state_start);
			// 合并frag_end的start状态与frag_start的end状态
			frag_end->state_start = frag_start->state_end;
			// 重新排序所有NFA状态
			i_t = -1;
			for (State *state : nfa.states)
			{
				state->ID = i_t++;
			}
			// 新建一个fragment存储frag_start的start状态以及frag_end的end状态
			stateId = getMaxStateID(nfa) + 1;
			newFragment = new Fragment(frag_start->state_start, frag_end->state_end);
			stack_frag.push_back(newFragment);
			// 删除新fragment连接的两个fragment
			delete frag_end;
			delete frag_start;
			break;
		default:
			// 遇到普通字符，创建两个新状态start & end
			// 两个新状态作为新转换fragment的起始状态和结束状态，起始状态输入ch转换到end
			// end状态作为fragment的结束状态
			state_start = new State(stateId, false);
			stateId++;
			state_end = new State(stateId, true);
			stateId++;
			state_start->transitions[ch] = state_end;
			newFragment = new Fragment(state_start, state_end);

			nfa.states.push_back(state_start);
			nfa.states.push_back(state_end);

			stack_frag.push_back(newFragment);
			break;
		}
	}
}

// 将输入复读
void Regex2Nfa::printInputByChar()
{
	cout << "This is input: ";
	for (char i : input)
	{
		cout << i;
	}
	cout << endl;
}

// 将插入分隔符后的输入串输出
void Regex2Nfa::printInsertStrByChar()
{
	std::cout << "This is inserted: ";
	for (char i : strBeInsert)
	{
		std::cout << i;
	}
	std::cout << std::endl;
}

// 将输入串的后缀表达式输出
void Regex2Nfa::printPostfixStrByChar()
{
	printf("This is postfix: ");
	for (char i : strPostfix)
	{
		std::cout << i;
	}
	std::cout << std::endl;
}

int Regex2Nfa::getMaxStateID(const NFA &nfa)
{
	int maxState = -1;
	for (const State *state : nfa.states)
	{

		maxState = (maxState > state->ID) ? maxState : state->ID;
	}
	return maxState;
}

// 检查一个字符是否在字符串里面
bool Regex2Nfa::isCharInStr(const string &s, const char c)
{
	size_t idx;
	idx = s.find(c);
	if (idx != string::npos)
	{
		return true;
	}
	else
	{
		return false;
	}
}
