#include "UseCase_AutoGen.h"

#ifdef _TEST_USECASE_AUTOGEN_
UseCase_AutoGen::UseCase_AutoGen()
{
}

UseCases UseCase_AutoGen::getUseCases()
{
	list<UseCase> useCases;
	//cout << "键入回车以开始。如输入完毕，输入ctrl+Z以结束" << endl;
	cout << "如输入完毕，输入ctrl+Z以结束" << endl;
	for (int no = 1; true; no++)
	{
		InputTable inputs;
		OutputTable outputs;
		try
		{
			this->empty();
			this->read();
			cout << "用例生成中..." << endl;
			this->init();
			this->compute();
		}
		catch (const std::exception& e)
		{
			inputs = this->getInput();
			// 如果是空输入则说明用例已输入完毕，跳出循环
			if (inputs.size() == 0)
			{
				break;
			}
			// 将异常信息加入到输出表中
			cout << "出现异常，将异常信息保存到用例中" << endl;
			outputs.push_back(e.what());
			UseCase usecase = { no, inputs, outputs };
			useCases.push_back(usecase);
			cout << "用例生成完毕！" << endl << endl;
			continue;
		}
		inputs = this->getInput();
		outputs = this->getOutput();
		UseCase usecase = { no, inputs, outputs };
		useCases.push_back(usecase);
		cout << "用例生成完毕！" << endl << endl;
	}
	return useCases;
}

void UseCase_AutoGen::empty()
{
	// 清空对象内的内容
	this->ter_set.clear();
	this->nter_set.clear();
	this->NTER_to_ifEP.clear();
	this->product_set.clear();
	this->start = "";
	this->inputTable.clear();
	this->VnNum = 0;
	this->VtNum = 0;
}

// 适用于用例自动生成的输入
void UseCase_AutoGen::read()
{
	printf("输入形如A->abc的产生式（其中~表示空）输入空行停止输入： \n");
	int id_pro = 0;
	string temp;
	while (getline(cin, temp), temp != "")
	{
		Product ps;
		this->inputTable.push_back(temp + "\n");
		// 将产生式的左部提取出来赋值给ps的左部
		ps.setLeft(ps.recLeft(temp));
		if (temp == "")
		{
			throw exception("空右部应使用~表示\n");
		}
		ps.setRight(temp);
		ps.setId(id_pro);
		this->product_set.push_back(ps);
		id_pro++;
	}

	// 检测空输入
	if (this->product_set.size() == 0)
	{
		throw exception("空输入\n");
	}
}

void UseCase_AutoGen::print(const OutputTable outputTable) const
{
	for (string line : outputTable)
	{
		cout << line;
	}
	cout << endl;
}

InputTable UseCase_AutoGen::getInput() const
{
	return this->inputTable;
}

void UseCase_AutoGen::saveUseCases(const UseCases useCases, 
	const string file_name) const
{
	cout << "正在保存用例到" + file_name + FILE_TYPE + "文件中" << endl;
	ofstream ofs;
	ofs.open(file_name + FILE_TYPE, ios::out);
	if (!ofs)
	{
		string message = file_name + FILE_TYPE + "打开失败！";
		throw exception(message.data());
	}
	// 遍历每个用例，生成固定格式的用例文本输出到指定文件中
	UIterator uci = useCases.begin();
	UIterator end = useCases.end();
	for (UIterator uci = useCases.begin(), end = useCases.end();
		uci != end; uci++)
	{
		UseCase usecase = *uci;
		string inputString = string("");
		string outputString = string("");
		for_input(usecase) { inputString += *iti; }
		for_output(usecase) { outputString += *oti; }
		ofs << USECASE_TITLE(usecase.no) << USECASE_INPUT << inputString
			<< USECASE_OUTPUT << outputString << USECASE_ENDING;
	}
	ofs.close();
	cout << "用例保存完毕！" << endl;
}
#endif // _TEST_USECASE_AUTOGEN_

