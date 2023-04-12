#include "declaration.h"
#include "declaration.h"

// 读入字符串转NFA
vector<NFAState> nfaInit(){
    vector<NFAState> nfa;
    string s;
	// 获取输入的NFA状态字符串，转化成NFA并存入向量中
    while (getline(cin, s, '\n')){
		STATE_ID headState, tailState;
		VALUE processingValue;
        int length = s.length();
        int scaner = 0;
        headState = s[scaner];
        length -= 2;
        scaner += 2;
		// 每个状态转换读入NFA中
        while (length > 0){
            NFAState temp;
            tailState = s[scaner + 5];
            processingValue = s[scaner + 2];

            temp.headState = headState;
            temp.processingValue = processingValue;
            temp.tailState = tailState;

            nfa.push_back(temp);

            length -= 7;
            scaner += 7;
        }
    }
    return nfa;
}

// 初始化向量列表（状态或者输入字符）
// get_value：真则获取字符|假则获取状态（默认）
CLUSTER getIndexState(const vector<NFAState>& nfa, bool get_value = false){
    CLUSTER states;

    // 获取NFA状态向量中存放的处理字符或者状态，存入向量中
    if(get_value){
        for(auto & i : nfa){
            states.push_back(i.processingValue);
        }
    } else{
        for(auto & i : nfa){
            states.push_back(i.headState);
            states.push_back(i.tailState);
        }
    }

    // 去除重复的元素，并排序
    sort(states.begin(),states.end());
    states.erase(unique(states.begin(), states.end()), states.end());

    return states;
}

// 获取在状态's'下输入数值'v'能够到达的所有状态
CLUSTER findNfa(const vector<NFAState>& nfa, STATE_ID s, VALUE v){
    CLUSTER res; // 获取到的状态集群
    for (auto & i : nfa){
        if (i.headState == s && i.processingValue == v)
            res.push_back(i.tailState);
    }
    return res;
}

// 获取状态's'的epsilon闭包
void getEpsilonCluster(const vector<NFAState>& nfa, CLUSTER &cluster, STATE_ID s){
	// 获取在s状态下通过~到达的所有状态
    CLUSTER res = findNfa(nfa,s,'~');
    cluster.push_back(s);
    cluster.insert(cluster.end(), res.begin(), res.end());

    for (STATE_ID & c : res){
        getEpsilonCluster(nfa, cluster, c);
    }
	// 若没有通过~能到达的状态则返回
    if (res.size() == 0){
        sort(cluster.begin(), cluster.end());
        cluster.erase(unique(cluster.begin(), cluster.end()), cluster.end());
        return;
    }
}

// 计算每个状态的Iv集中的J（v是迁移的字符串）
CLUSTER move(const vector<NFAState>& nfa, CLUSTER c, VALUE v){
    CLUSTER res;
    for (STATE_ID & s : c) {
		// 求出J，即迁移状态集
        CLUSTER temp = findNfa(nfa, s, v);
        res.insert(res.end(), temp.begin(), temp.end());
    }
    sort(res.begin(),res.end());
    res.erase(unique(res.begin(), res.end()), res.end());

    return res;
}

// 获得Iv，也就是epsilon-closure(clusterMove)
CLUSTER joinCluster(vector<CLUSTER> epsilonClusters, CLUSTER clusterMove){
    CLUSTER joinRes;
	// 将move状态集中的所有状态的epsilon闭包加入到joinres中
    for(STATE_ID & s : clusterMove){
		// 处理状态'X'
        if (s == 'X')
            joinRes.insert(
                    joinRes.end(),
                   epsilonClusters[epsilonClusters.size() - 2].begin(),
                   epsilonClusters[epsilonClusters.size() - 2].end()
                   );
		// 处理状态'Y'
        else if (s == 'Y')
            joinRes.insert(
                    joinRes.end(),
                    epsilonClusters[epsilonClusters.size() - 1].begin(),
                    epsilonClusters[epsilonClusters.size() - 1].end()
                    );
		// 处理数字状态
        else
            joinRes.insert(
                    joinRes.end(),
                    epsilonClusters[s - '0'].begin(),
                    epsilonClusters[s - '0'].end()
                    );
    }
    sort(joinRes.begin(),joinRes.end());
    joinRes.erase(unique(joinRes.begin(),joinRes.end()),joinRes.end());

    return joinRes;
}

// 将所有状态转换中的状态名更名，开始状态命名为'X'，终止状态命名为'Y'
vector<DFAState> renameDFA(vector<DFAState> dfa, STATE_ID start, STATE_ID terminate){
    for (DFAState &d : dfa){
        // 更名头状态
        if (d.headState == start) d.headState = 'X';
        else if (d.headState == terminate) d.headState = 'Y';
        else d.headState--;
        // 更名尾状态
        if (d.tailState == start) d.tailState = 'X';
        else if (d.tailState == terminate) d.tailState = 'Y';
        else d.tailState--;
    }
    return dfa;
}

// 输出DFA中的状态转换
void outputDFA(vector<DFAState> dfa, int stateNum){
    // 输出开始状态
    cout << "X";
    for (DFAState &d : dfa){
        if (d.headState == 'X')
            cout << " " << d.headState << "-" << d.processingValue << "->" << d.tailState;
    }
    cout << "\n";
    // output terminate state then
    cout << "Y";
    for (DFAState &d : dfa){
        if (d.headState == 'Y')
            cout << " " << d.headState << "-" << d.processingValue << "->" << d.tailState;
    }
    cout << "\n";
    // 输出其他状态
    for (int i = 0; i < stateNum; ++i) {
        cout << i;
        for (DFAState &d : dfa){
            if (d.headState == '0' + i)
                cout << " " << d.headState << "-" << d.processingValue << "->" << d.tailState;
        }
        cout << "\n";
    }
}

void NFA2DFA(){
    vector<DFAState> dfa;
    vector<NFAState> nfa = nfaInit();
    vector<STATE_ID> stateList = getIndexState(nfa);
    vector<VALUE> valueList = getIndexState(nfa, true);

    // 获取所有状态的epsilon闭包
    vector<CLUSTER> epsilonClusters;
    for(STATE_ID &s : stateList){
        CLUSTER temp;
        getEpsilonCluster(nfa, temp, s);
        epsilonClusters.push_back(temp);
    }

    // 构造DFA
    vector<CLUSTER> newCluster;
	// 将初始状态集放入newCluster中
	int startStatePos = epsilonClusters.size() - 2;
    newCluster.push_back(epsilonClusters[startStatePos]);
    unsigned int i = 0;
    while(i < newCluster.size()){
        for(VALUE & v : valueList){
            if (v == '~') continue; // 跳过输入字符epsilon

			// 求出迁移状态集J = I集中状态通过v迁移到的状态
            CLUSTER clusterMove = move(nfa, newCluster[i], v);
            if (clusterMove.empty()) continue; // 跳过空集

            CLUSTER clusterTemp = joinCluster(epsilonClusters, clusterMove);
            // 如果该Iv集在newCluster中不存在
            if (!count(newCluster.begin(), newCluster.end(), clusterTemp)){
                newCluster.push_back(clusterTemp);		 // 在DFA中插入新的状态
				// 创建一个新的DFA状态转换并放进DFA中，该转换从i状态输入v后到达新状态
                DFAState dfaTemp;
                dfaTemp.headState = '0' + i;
                dfaTemp.processingValue = v;
                dfaTemp.tailState = '0' + newCluster.size() - 1;
                dfa.push_back(dfaTemp);
            } else {
				// 如果该lv集在newCluster中存在，则计算出其所在位置
                int position = distance(
                        newCluster.begin(),
                        find(newCluster.begin(), newCluster.end(), clusterTemp)
                        );
				// 生成一个DFA状态转换
                DFAState dfaTemp;
                dfaTemp.headState = '0' + i;
                dfaTemp.processingValue = v;
                dfaTemp.tailState = '0' + position;
                dfa.push_back(dfaTemp);
            }
        }
        i++;
    }
    // 重命名DFA中的状态
    STATE_ID start = '0';
    STATE_ID terminate = '0' + newCluster.size() - 1;
    dfa = renameDFA(dfa, start, terminate);

    int stateNum = newCluster.size() - 2;
    outputDFA(dfa, stateNum);
}



