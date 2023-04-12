#include "declaration.h"

DFA::DFA()
{
}

DFA::~DFA()
{
}

vector<DFA_State> DFA::getStates()
{
	vector<DFA_State> dfaStateList;

	//读入所有状态
	DFA_State dfa_s = DFA_State();
	while (dfa_s.getState() != ' ')
	{
		dfaStateList.push_back(dfa_s);
		//遍历该状态的输入字符，加入可输入字符到DFA中
		Accept_Chars accepts = dfa_s.getAcceptChars();
		Accept_Chars::iterator vi = accepts.begin();
		while (vi != accepts.end())
		{
			// 如果不存在则加入
			if (isExisit(this->accept_chars, *vi) == -1)
			{
				this->accept_chars.push_back(*vi);
			}
			vi++;
		}
		dfa_s = DFA_State();
	}
	return dfaStateList;
}

void DFA::setStatesList(vector<DFA_State> &list)
{
	this->dfaStates_list = list;
	vector<char> terminate, non_terminate;
	// 对名字列表进行初始化，并将终态和非终态存储名字和下标在对应映射中
	for (int i = 0; i < list.size(); i++)
	{
		char name = list[i].getName();
		this->name_to_listpos[name] = i;
		if (name == 'Y')
		{
			terminate.push_back(name);
		}
		else
		{
			non_terminate.push_back(name);
		}
	}
	// 将子集划分映射放入子集向量中
	this->subSets.push_back(terminate);
	this->subSets.push_back(non_terminate);
	this->mapName_To_SubsetNo();
}

/*将一个子集进行划分，返回一个子集向量*/
Sta_Subsets DFA::subSetDivide(Sta_Subset subset)
{
	Sta_Subsets divided_subsets;
	// 如果只有一个状态，则直接返回，大于一个状态才会进行划分
	if (subset.size() > 1)
	{
		// 遍历子集，将各个状态输入字符后得到的下一状态进行分类，逐种子符分类
		for (Sta_Subset::iterator ci = this->accept_chars.begin();
			ci < this->accept_chars.end(); ci++)
		{
			divided_subsets = this->subSetDivide_bySingleChar(subset, *ci);
			// 如果分出来的子集数量大于1，则说明进行了划分
			if (divided_subsets.size() > 1)
			{
				return divided_subsets;
			}
			// 如果等于1则说明没有进行划分，按照下一种字符继续划分
		}
		return divided_subsets;
	}
	divided_subsets.push_back(subset);
	return divided_subsets;
}

Sta_Subsets DFA::getSubsets()
{
	return this->subSets;
}

void DFA::setSubsets(Sta_Subsets newSubsets)
{
	this->subSets = newSubsets;
	this->mapName_To_SubsetNo();
}

void DFA::outputSubsets()
{
	// 将所有子集的状态由一个状态代表，代表状态名字优先级为状态编号越小越大
	this->simplify_subsets();
	// 对子集进行排序，代表状态名字优先级为状态编号越小越大
	this->sort_subsets();
	// 顺序输出
	Sta_Subsets::iterator vvci = this->subSets.begin();
	while (vvci != this->subSets.end())
	{
		int index = this->name_to_listpos[(*vvci)[0]];
		DFA_State dfa_state = this->dfaStates_list[index];
		dfa_state.print();
		vvci++;
	}
}

void vector_swap(Sta_Subsets::iterator target1,
	Sta_Subsets::iterator target2)
{
	Sta_Subset temp = *target1;
	*target1 = *target2;
	*target2 = temp;
}

Sta_Subsets DFA::subSetDivide_bySingleChar(Sta_Subset subset, char basis)
{
	Sta_Subsets divided_subsets;
	map<int, Sta_Subset> nextStateSubset_to_curState;
	map<int, Sta_Subset>::iterator mcvi;

	for (int i = 0; i < subset.size(); i++)
	{
		char curState = subset[i];
		int listPos = this->name_to_listpos[curState];
		DFA_State s = this->dfaStates_list[listPos];
		int nextSubsetNo = this->name_to_subsetNo[s.transfer(basis)];
		// 查找是否有转换到这种子集的状态
		mcvi = nextStateSubset_to_curState.find(nextSubsetNo);
		if ( mcvi == nextStateSubset_to_curState.end() )
		{
			// 没有则新建
			Sta_Subset newSubset;
			newSubset.push_back(curState);
			nextStateSubset_to_curState[nextSubsetNo] = newSubset;
		}
		else
		{
			// 将当前状态加入原有列表
			mcvi->second.push_back(curState);
		}
	}

	mcvi = nextStateSubset_to_curState.begin();
	// 将映射中的全部子集放到结果返回的子集列表中
	while (mcvi != nextStateSubset_to_curState.end())
	{
		divided_subsets.push_back(mcvi->second);
		mcvi++;
	}
	return divided_subsets;
}

void DFA::mapName_To_SubsetNo()
{
	for (int i = 0; i < this->subSets.size(); i++)
	{
		Sta_Subset subset = this->subSets[i];
		for (int j = 0; j < subset.size(); j++)
		{
			this->name_to_subsetNo[subset[j]] = i;
		}
	}
}

// 将所有子集的状态由一个状态代表，代表状态名字优先级为状态编号越小越大
void DFA::simplify_subsets()
{
	for (int i = 0; i < this->subSets.size(); i++)
	{
		Sta_Subset subset = this->subSets[i];
		Sta_Subset::iterator vci = subset.begin();
		while (subset.size() > 1)
		{
			int index_of_curState = this->name_to_listpos[*vci];
			int index_of_nextState = this->name_to_listpos[*(vci + 1)];
			// 根据状态编号进行有限集排序
			if (index_of_curState < index_of_nextState)
			{
				subset.erase(vci + 1);
			}
			else
			{
				vci = subset.erase(vci);
			}
			this->subSets[i] = subset;
		}
	}
}

void DFA::sort_subsets()
{
	Sta_Subsets::iterator vvci;
	bool isSwap = true; // 冒泡排序结束标志
	while (isSwap)
	{
		isSwap = false;
		vvci = this->subSets.begin();
		while (vvci != this->subSets.end() - 1)
		{
			char name_of_cur = (*vvci)[0];
			char name_of_next = (*(vvci + 1))[0];
			int index_of_curState = this->name_to_listpos[name_of_cur];
			int index_of_nextState = this->name_to_listpos[name_of_next];
			if (index_of_curState > index_of_nextState)
			{
				vector_swap(vvci, vvci + 1);
				isSwap = true;
			}
			vvci++;
		}
	}
}

template	<class T>
int isExisit(vector<T> source, T target) 
{
	for (int i = 0; i < source.size(); i++)
	{
		if (source[i] == target)
		{
			return i;
		}
	}
	return -1;
}

