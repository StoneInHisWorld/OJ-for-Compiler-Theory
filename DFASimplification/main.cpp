#include "declaration.h"

using namespace std;

int main() {
	DFA dfa;
	vector<DFA_State> dfaState_list = dfa.getStates();
	dfa.setStatesList(dfaState_list);
	vector<Sta_Subset> subsets = dfa.getSubsets();
	vector<Sta_Subset>::iterator vvci;
	vector<char>::iterator vci;

	// 对每一个子集进行划分
	int oldLength = 0, newLength = 1;
	while (oldLength != newLength)
	{
		oldLength = subsets.size();
		for ( vvci = subsets.begin(); vvci != subsets.end(); vvci++)
		{
			Sta_Subsets dividedSubsets = dfa.subSetDivide(*vvci);
			// 如果进行了划分，则将该子集从原有子集集合中剔除，并加入新的划分
			if (dividedSubsets.size() > 1) {
				subsets.erase(vvci);
				for (int i = 0; i < dividedSubsets.size(); i++)
				{
					subsets.push_back(dividedSubsets[i]);
				}
				// 刷新DFA中的子集，并更新子集编号
				dfa.setSubsets(subsets);
				break;
			}
			// 对现在的子集并未进行划分，则对下一个子集进行划分
		}
		newLength = subsets.size();
	}

	dfa.outputSubsets();
	return 0;
	//vector<DFA_State> dfaStateList;

	////读入所有状态
	//DFA_State dfa_s = DFA_State();
	//while (dfa_s.getState() != ' ') 
	//{
	//	dfaStateList.push_back(dfa_s);
	//	dfa_s = DFA_State();
	//}
	////剔除相同状态点
	//char save[20];
	//char del[20];
	//int k = 0;
	//for (int i = 0; i < dfaStateList.size(); i++) {
	//	for (int j = i + 1; j < dfaStateList.size(); j++) {
	//		if (dfaStateList[i].accept == dfaStateList[j].accept 
	//		&& dfaStateList[i].transferList == dfaStateList[j].transferList
	//		&& i != 1 && j != 1) {
	//			save[k] = dfaStateList[i].name;
	//			del[k] = dfaStateList[j].name;
	//			k++;
	//		}
	//	}
	//}
	//for (int i = 0; i < k; i++) {
	//	for (int j = 0; j < dfaStateList.size(); j++) {
	//		if (dfaStateList[j].name == del[i]) {
	//			dfaStateList.erase(dfaStateList.begin() + j);
	//		}
	//		for (int t = 0; t < dfaStateList[j].transferList.size(); t++) {
	//			if (dfaStateList[j].transferList[t] == del[i]) {
	//				dfaStateList[j].transferList[t] = save[i];
	//				t = t - 1;
	//			}
	//		}
	//	}
	//}
	//for (int i = 0; i < dfaStateList.size(); i++) {
	//	dfaStateList[i].print();
	//}
}