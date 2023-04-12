#pragma once
#include "declaration.h"
#include "Parser.h"

#ifdef _TEST_USECASE_AUTOGEN_
// �ļ�����
#define FILE_TYPE ".txt"
#define USECASE_TITLE(no) "����" + toString(no) + "\n"
#define USECASE_INPUT "��׼���룺\n"
#define USECASE_OUTPUT "���������\n"
#define USECASE_ENDING "\n"
#define for_input(usecase) 	for (InputTable::iterator iti = usecase.input.begin(), \
end = usecase.input.end();iti != end; iti++)
#define for_output(usecase) 	for (OutputTable::iterator oti = usecase.output.begin(), \
end = usecase.output.end(); oti != end; oti++)

typedef list<string> OutputTable;
typedef list<string> InputTable;
typedef struct use_case {
	int no;
	InputTable input;
	OutputTable output;
}UseCase;
typedef list<UseCase> UseCases;
typedef UseCases::const_iterator UIterator;

// ���Զ������������﷨������
class UseCase_AutoGen :public Parser
{
public:
	UseCase_AutoGen();
	virtual UseCases getUseCases();
	virtual void saveUseCases(const UseCases useCases, const string file_name) const;
	// �����������Զ����ɵ�����
	virtual void read();
	virtual void print(const OutputTable outputTable) const;
	virtual InputTable getInput() const;

protected:
	// ����������������Խ�����һ������
	virtual void empty();
	// �������ռ���������
	InputTable inputTable;
	virtual OutputTable getOutput() const = 0;
};
#endif // _TEST_USECASE_AUTOGEN_