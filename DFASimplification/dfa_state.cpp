// 原作者：软件1801 王广
#include "declaration.h"

void DFA_State::print()
{
	int len = accept.size();
	if (accept.size() > 0) {
		cout << name << " ";
	}
	else {
		cout << name << endl;
	}
	for (int i = 0; i < len; i++) {
		if (i < len - 1) {
			cout << name << "-" << accept[i] << "->" << transferList[i] << " ";
		}
		else {
			cout << name << "-" << accept[i] << "->" << transferList[i] << endl;
		}
	}
}

char DFA_State::getState()
{
	// 获取状态字符串（每个状态均为一个字符）
	string ss;
	getline(cin, ss);
	// 如果为空则判定输入完成
	if (ss.size() == 0) {
		return ' ';
	}
	this->name = ss[0];
	// 清理多余字符
	ss = ss.substr(2);
	ss = replaceAll(ss, " ", "");
	ss = replaceAll(ss, "-", "");
	ss = replaceAll(ss, ">", "");
	// 遍历字符串获取状态转换函数（每个状态均为一个字符）
	string::iterator si = ss.begin();
	while (si != ss.end())
	{
		if (*si == this->name)
		{
			si++;
			this->accept.push_back(*si);
			si++;
			this->transferList.push_back(*si);
		}
		else
		{
			si++;
			si++;
		}
		si++;
	}
	return this->name;
}

char DFA_State::getName()
{
	return this->name;
}

Accept_Chars DFA_State::getAcceptChars()
{
	return this->accept;
}

State_Name DFA_State::transfer(char inputChar)
{
	int index = isExisit(this->accept, inputChar);
	// 如果可接受
	if (index != -1)
	{
		return this->transferList[index];
	}
	return ' ';
}

string replaceAll(string source, const string target, const string replace_by)
{
	int lastLength = 0;
	int newLength = 1;
	// 新老字符串长度不变则返回
	while (lastLength != newLength)
	{
		lastLength = source.length();
		int index = source.find(target);
		// 找不到目标则返回
		if (index == string::npos)
		{
			return source;
		}
		else
		{
			source.replace(index, target.length(), replace_by);
		}
		newLength = source.length();
	}
	return source;
}


