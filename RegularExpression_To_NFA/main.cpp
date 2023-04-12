#include "declaration.h"

bool CHECK_ON = false;  // �Ƿ��������ݵ��������

int main()
{
	Regex2Nfa regex2nfa;

	// ��ȡ������ʽ���磺(a|b)*c,abed
	//		regex2nfa.setInput("(a|b)*abb");
	regex2nfa.getInput();
	//    regex2nfa.setInput("abb");

	// [����] ������ʽ�в����ض����ӷ����磺(a|b)*c -> (a|b)*_c
	regex2nfa.insertExplicit();

	// [����] ת��Ϊ��׺��ʾ�����磺 (a|b)*_c -> ab|*c
	regex2nfa.convertToPostfix();

	// [����] ����һ��NFA
	regex2nfa.constructToNFA();

	if (CHECK_ON)
	{
		// [���] ���������ʽ
		regex2nfa.printInputByChar();
		// [���] ������ָ�����������ʽ
		regex2nfa.printInsertStrByChar();
		// [���] ����׺���ʽ
		regex2nfa.printPostfixStrByChar();
	}

	// [���] ������յ�NFA
	regex2nfa.output();

	return 0;
}

