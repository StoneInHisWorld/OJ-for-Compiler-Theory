#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <map>
#include <exception>

using namespace std;

#define TABL 8 // 单个制表符能空出的最多空位
#define EP '~' // 表示epsilon
#define TER char // 终结符要求只能有一个字符
#define NTER string // 非终结符可以有多个字符

string fit_cube(string content, string interval);
inline int toInt(const string s);

int main() {
	string input;
	while (cin >> input)
	{
		try
		{
			int size = 0;
			for (int i = 0; i < input.length(); i++)
			{
				size += sizeof(input[i]);
				cout << input[i] << ' ' << sizeof(input[i]) << '.';
			}
			cout << endl << "size:" << size << "length:" << input.length() << endl;
		}
		catch (const std::exception &e)
		{
			cout << e.what();
		}
	}
}

string fit_cube(string content, string interval)
{
	if (interval.find_first_not_of('\t') == string::npos)
	{
		int len_of_content = content.length();
		int num_of_provided_tab = interval.length();
		int num_of_tab_needed = num_of_provided_tab - len_of_content / TABL;
		if (num_of_tab_needed > 0)
		{
			for (int i = 0; i < num_of_tab_needed; i++)
			{
				content.push_back('\t');
			}
			return content;
		}
		else if (num_of_tab_needed <= 0)
		{
			throw exception("有填表内容超出格子大小，请加大格子间距\n");
		}
	}
	else
	{
		throw exception("格子间隔字符串不能包括非\\t的字符");
	}
}
