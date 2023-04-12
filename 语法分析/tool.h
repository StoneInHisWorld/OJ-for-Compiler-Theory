#pragma once
#include "declaration.h"

// 判断是否是终结符的规则
inline bool isTer(char x)
{
	if (x >= 'A' && x <= 'Z')
		return false;
	if (x == '\'')
	{
		return false;
	}
	if (x == '#')
	{
		return false;
	}
	return true;
}

inline string toString(int i)
{
	if (i == 0)
	{
		return "0";
	}
	if (i > 0)
	{
		string ret = string("");
		// digit是对应位置上的数字
		for (int digit = i % 10; i != 0; i /= 10, digit = i % 10)
		{
			char c = digit + '0';
			ret.insert(ret.begin(), c);
		}
		return ret;
	}
	else
	{
		return "-" + toString(abs(i));
	}
}

inline string toString(TOKEN token)
{
	if (token.first != NULL)
	{
		return string() + token.first;
	}
	else
	{
		return token.second;
	}
}

inline int toInt(string s)
{
	int len = s.length();
	int ret = 0;
	for (int i = len - 1, degree = 1; i >= 0; i--)
	{
		// 字符转化成数字后乘以阶数
		ret += (s[i] - '0') * degree;
		degree *= 10;
	}
	return ret;
}

template <typename T>
// 将set2的元素归并到set1中
inline void merge_set(set<T>& set1, const set<T>& set2)
{
	auto sti = set2.begin();
	auto end = set2.end();
	while (sti != end)
	{
		set1.insert(*sti);
		sti++;
	}
}

template <typename T1, typename T2>
// 将map2的元素归并到map1中
inline void merge_map(map<T1, T2>& map1, const map<T1, T2>& map2)
{
	auto sti = map2.begin();
	auto end = map2.end();
	while (sti != end)
	{
		auto ret = map1.insert(*sti);
		if (ret.second == false)
		{
			throw exception("有重复键值！\n");
		}
		sti++;
	}
}

template <typename T>
// 在source中寻找target
inline auto contains_set(const T & target, const set<T>& source)
{
	for (auto begin = source.begin(), end = source.end(); begin != end; begin++)
	{
		if (*begin == target)
		{
			return begin;
		}
	}
	return source.end();
}

template <typename T>
// 在source中寻找target
inline auto contains_list(const T & target, const list<T>& source)
{
	for (auto begin = source.begin(), end = source.end(); begin != end; begin++)
	{
		if (*begin == target)
		{
			return begin;
		}
	}
	return source.end();
}

template <typename T1, typename T2>
inline auto insert_map(const T1& key, const T2& value, map<T1, T2> & target)
{
	pair<T1, T2> temp = pair<T1, T2>(key, value);
	return target.insert(temp);
}

inline string fit_cube(string content, string interval)
{
	if (interval.find_first_not_of('\t') == string::npos)
	{
		// 加上格子的分隔符长度
		int len_of_content = content.length() + string(COL_DIV).length();
		int num_of_provided_tab = interval.length();
		int num_of_tab_needed = num_of_provided_tab - len_of_content / TABL;
		if (num_of_tab_needed > 0)
		{
			for (int i = 0; i < num_of_tab_needed; i++)
			{
				content.push_back('\t');
			}
			return COL_DIV + content;
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
	return COL_DIV;
}