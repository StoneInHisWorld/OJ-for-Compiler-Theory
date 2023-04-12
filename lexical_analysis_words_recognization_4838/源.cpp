#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <map>
#include <exception>

using namespace std;

#define TABL 8 // �����Ʊ���ܿճ�������λ
#define EP '~' // ��ʾepsilon
#define TER char // �ս��Ҫ��ֻ����һ���ַ�
#define NTER string // ���ս�������ж���ַ�

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
			throw exception("��������ݳ������Ӵ�С����Ӵ���Ӽ��\n");
		}
	}
	else
	{
		throw exception("���Ӽ���ַ������ܰ�����\\t���ַ�");
	}
}
