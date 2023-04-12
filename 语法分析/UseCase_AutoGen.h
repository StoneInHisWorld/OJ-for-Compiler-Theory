#pragma once
#include "declaration.h"
#include "Parser.h"

#ifdef _TEST_USECASE_AUTOGEN_
// 文件类型
#define FILE_TYPE ".txt"
#define USECASE_TITLE(no) "用例" + toString(no) + "\n"
#define USECASE_INPUT "标准输入：\n"
#define USECASE_OUTPUT "期望输出：\n"
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

// 可自动生成用例的语法分析器
class UseCase_AutoGen :public Parser
{
public:
	UseCase_AutoGen();
	virtual UseCases getUseCases();
	virtual void saveUseCases(const UseCases useCases, const string file_name) const;
	// 适用于用例自动生成的输入
	virtual void read();
	virtual void print(const OutputTable outputTable) const;
	virtual InputTable getInput() const;

protected:
	// 清空自身所有数据以进行下一次输入
	virtual void empty();
	// 适用于收集错误输入
	InputTable inputTable;
	virtual OutputTable getOutput() const = 0;
};
#endif // _TEST_USECASE_AUTOGEN_