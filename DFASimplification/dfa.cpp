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

	//��������״̬
	DFA_State dfa_s = DFA_State();
	while (dfa_s.getState() != ' ')
	{
		dfaStateList.push_back(dfa_s);
		//������״̬�������ַ�������������ַ���DFA��
		Accept_Chars accepts = dfa_s.getAcceptChars();
		Accept_Chars::iterator vi = accepts.begin();
		while (vi != accepts.end())
		{
			// ��������������
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
	// �������б���г�ʼ����������̬�ͷ���̬�洢���ֺ��±��ڶ�Ӧӳ����
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
	// ���Ӽ�����ӳ������Ӽ�������
	this->subSets.push_back(terminate);
	this->subSets.push_back(non_terminate);
	this->mapName_To_SubsetNo();
}

/*��һ���Ӽ����л��֣�����һ���Ӽ�����*/
Sta_Subsets DFA::subSetDivide(Sta_Subset subset)
{
	Sta_Subsets divided_subsets;
	// ���ֻ��һ��״̬����ֱ�ӷ��أ�����һ��״̬�Ż���л���
	if (subset.size() > 1)
	{
		// �����Ӽ���������״̬�����ַ���õ�����һ״̬���з��࣬�����ӷ�����
		for (Sta_Subset::iterator ci = this->accept_chars.begin();
			ci < this->accept_chars.end(); ci++)
		{
			divided_subsets = this->subSetDivide_bySingleChar(subset, *ci);
			// ����ֳ������Ӽ���������1����˵�������˻���
			if (divided_subsets.size() > 1)
			{
				return divided_subsets;
			}
			// �������1��˵��û�н��л��֣�������һ���ַ���������
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
	// �������Ӽ���״̬��һ��״̬��������״̬�������ȼ�Ϊ״̬���ԽСԽ��
	this->simplify_subsets();
	// ���Ӽ��������򣬴���״̬�������ȼ�Ϊ״̬���ԽСԽ��
	this->sort_subsets();
	// ˳�����
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
		// �����Ƿ���ת���������Ӽ���״̬
		mcvi = nextStateSubset_to_curState.find(nextSubsetNo);
		if ( mcvi == nextStateSubset_to_curState.end() )
		{
			// û�����½�
			Sta_Subset newSubset;
			newSubset.push_back(curState);
			nextStateSubset_to_curState[nextSubsetNo] = newSubset;
		}
		else
		{
			// ����ǰ״̬����ԭ���б�
			mcvi->second.push_back(curState);
		}
	}

	mcvi = nextStateSubset_to_curState.begin();
	// ��ӳ���е�ȫ���Ӽ��ŵ�������ص��Ӽ��б���
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

// �������Ӽ���״̬��һ��״̬��������״̬�������ȼ�Ϊ״̬���ԽСԽ��
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
			// ����״̬��Ž������޼�����
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
	bool isSwap = true; // ð�����������־
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

