#include "declaration.h"
#include "declaration.h"

// �����ַ���תNFA
vector<NFAState> nfaInit(){
    vector<NFAState> nfa;
    string s;
	// ��ȡ�����NFA״̬�ַ�����ת����NFA������������
    while (getline(cin, s, '\n')){
		STATE_ID headState, tailState;
		VALUE processingValue;
        int length = s.length();
        int scaner = 0;
        headState = s[scaner];
        length -= 2;
        scaner += 2;
		// ÿ��״̬ת������NFA��
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

// ��ʼ�������б�״̬���������ַ���
// get_value�������ȡ�ַ�|�����ȡ״̬��Ĭ�ϣ�
CLUSTER getIndexState(const vector<NFAState>& nfa, bool get_value = false){
    CLUSTER states;

    // ��ȡNFA״̬�����д�ŵĴ����ַ�����״̬������������
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

    // ȥ���ظ���Ԫ�أ�������
    sort(states.begin(),states.end());
    states.erase(unique(states.begin(), states.end()), states.end());

    return states;
}

// ��ȡ��״̬'s'��������ֵ'v'�ܹ����������״̬
CLUSTER findNfa(const vector<NFAState>& nfa, STATE_ID s, VALUE v){
    CLUSTER res; // ��ȡ����״̬��Ⱥ
    for (auto & i : nfa){
        if (i.headState == s && i.processingValue == v)
            res.push_back(i.tailState);
    }
    return res;
}

// ��ȡ״̬'s'��epsilon�հ�
void getEpsilonCluster(const vector<NFAState>& nfa, CLUSTER &cluster, STATE_ID s){
	// ��ȡ��s״̬��ͨ��~���������״̬
    CLUSTER res = findNfa(nfa,s,'~');
    cluster.push_back(s);
    cluster.insert(cluster.end(), res.begin(), res.end());

    for (STATE_ID & c : res){
        getEpsilonCluster(nfa, cluster, c);
    }
	// ��û��ͨ��~�ܵ����״̬�򷵻�
    if (res.size() == 0){
        sort(cluster.begin(), cluster.end());
        cluster.erase(unique(cluster.begin(), cluster.end()), cluster.end());
        return;
    }
}

// ����ÿ��״̬��Iv���е�J��v��Ǩ�Ƶ��ַ�����
CLUSTER move(const vector<NFAState>& nfa, CLUSTER c, VALUE v){
    CLUSTER res;
    for (STATE_ID & s : c) {
		// ���J����Ǩ��״̬��
        CLUSTER temp = findNfa(nfa, s, v);
        res.insert(res.end(), temp.begin(), temp.end());
    }
    sort(res.begin(),res.end());
    res.erase(unique(res.begin(), res.end()), res.end());

    return res;
}

// ���Iv��Ҳ����epsilon-closure(clusterMove)
CLUSTER joinCluster(vector<CLUSTER> epsilonClusters, CLUSTER clusterMove){
    CLUSTER joinRes;
	// ��move״̬���е�����״̬��epsilon�հ����뵽joinres��
    for(STATE_ID & s : clusterMove){
		// ����״̬'X'
        if (s == 'X')
            joinRes.insert(
                    joinRes.end(),
                   epsilonClusters[epsilonClusters.size() - 2].begin(),
                   epsilonClusters[epsilonClusters.size() - 2].end()
                   );
		// ����״̬'Y'
        else if (s == 'Y')
            joinRes.insert(
                    joinRes.end(),
                    epsilonClusters[epsilonClusters.size() - 1].begin(),
                    epsilonClusters[epsilonClusters.size() - 1].end()
                    );
		// ��������״̬
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

// ������״̬ת���е�״̬����������ʼ״̬����Ϊ'X'����ֹ״̬����Ϊ'Y'
vector<DFAState> renameDFA(vector<DFAState> dfa, STATE_ID start, STATE_ID terminate){
    for (DFAState &d : dfa){
        // ����ͷ״̬
        if (d.headState == start) d.headState = 'X';
        else if (d.headState == terminate) d.headState = 'Y';
        else d.headState--;
        // ����β״̬
        if (d.tailState == start) d.tailState = 'X';
        else if (d.tailState == terminate) d.tailState = 'Y';
        else d.tailState--;
    }
    return dfa;
}

// ���DFA�е�״̬ת��
void outputDFA(vector<DFAState> dfa, int stateNum){
    // �����ʼ״̬
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
    // �������״̬
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

    // ��ȡ����״̬��epsilon�հ�
    vector<CLUSTER> epsilonClusters;
    for(STATE_ID &s : stateList){
        CLUSTER temp;
        getEpsilonCluster(nfa, temp, s);
        epsilonClusters.push_back(temp);
    }

    // ����DFA
    vector<CLUSTER> newCluster;
	// ����ʼ״̬������newCluster��
	int startStatePos = epsilonClusters.size() - 2;
    newCluster.push_back(epsilonClusters[startStatePos]);
    unsigned int i = 0;
    while(i < newCluster.size()){
        for(VALUE & v : valueList){
            if (v == '~') continue; // ���������ַ�epsilon

			// ���Ǩ��״̬��J = I����״̬ͨ��vǨ�Ƶ���״̬
            CLUSTER clusterMove = move(nfa, newCluster[i], v);
            if (clusterMove.empty()) continue; // �����ռ�

            CLUSTER clusterTemp = joinCluster(epsilonClusters, clusterMove);
            // �����Iv����newCluster�в�����
            if (!count(newCluster.begin(), newCluster.end(), clusterTemp)){
                newCluster.push_back(clusterTemp);		 // ��DFA�в����µ�״̬
				// ����һ���µ�DFA״̬ת�����Ž�DFA�У���ת����i״̬����v�󵽴���״̬
                DFAState dfaTemp;
                dfaTemp.headState = '0' + i;
                dfaTemp.processingValue = v;
                dfaTemp.tailState = '0' + newCluster.size() - 1;
                dfa.push_back(dfaTemp);
            } else {
				// �����lv����newCluster�д��ڣ�������������λ��
                int position = distance(
                        newCluster.begin(),
                        find(newCluster.begin(), newCluster.end(), clusterTemp)
                        );
				// ����һ��DFA״̬ת��
                DFAState dfaTemp;
                dfaTemp.headState = '0' + i;
                dfaTemp.processingValue = v;
                dfaTemp.tailState = '0' + position;
                dfa.push_back(dfaTemp);
            }
        }
        i++;
    }
    // ������DFA�е�״̬
    STATE_ID start = '0';
    STATE_ID terminate = '0' + newCluster.size() - 1;
    dfa = renameDFA(dfa, start, terminate);

    int stateNum = newCluster.size() - 2;
    outputDFA(dfa, stateNum);
}



