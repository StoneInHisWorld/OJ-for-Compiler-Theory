#include "UseCase_AutoGen.h"

#ifdef _TEST_USECASE_AUTOGEN_
UseCase_AutoGen::UseCase_AutoGen()
{
}

UseCases UseCase_AutoGen::getUseCases()
{
	list<UseCase> useCases;
	//cout << "����س��Կ�ʼ����������ϣ�����ctrl+Z�Խ���" << endl;
	cout << "��������ϣ�����ctrl+Z�Խ���" << endl;
	for (int no = 1; true; no++)
	{
		InputTable inputs;
		OutputTable outputs;
		try
		{
			this->empty();
			this->read();
			cout << "����������..." << endl;
			this->init();
			this->compute();
		}
		catch (const std::exception& e)
		{
			inputs = this->getInput();
			// ����ǿ�������˵��������������ϣ�����ѭ��
			if (inputs.size() == 0)
			{
				break;
			}
			// ���쳣��Ϣ���뵽�������
			cout << "�����쳣�����쳣��Ϣ���浽������" << endl;
			outputs.push_back(e.what());
			UseCase usecase = { no, inputs, outputs };
			useCases.push_back(usecase);
			cout << "����������ϣ�" << endl << endl;
			continue;
		}
		inputs = this->getInput();
		outputs = this->getOutput();
		UseCase usecase = { no, inputs, outputs };
		useCases.push_back(usecase);
		cout << "����������ϣ�" << endl << endl;
	}
	return useCases;
}

void UseCase_AutoGen::empty()
{
	// ��ն����ڵ�����
	this->ter_set.clear();
	this->nter_set.clear();
	this->NTER_to_ifEP.clear();
	this->product_set.clear();
	this->start = "";
	this->inputTable.clear();
	this->VnNum = 0;
	this->VtNum = 0;
}

// �����������Զ����ɵ�����
void UseCase_AutoGen::read()
{
	printf("��������A->abc�Ĳ���ʽ������~��ʾ�գ��������ֹͣ���룺 \n");
	int id_pro = 0;
	string temp;
	while (getline(cin, temp), temp != "")
	{
		Product ps;
		this->inputTable.push_back(temp + "\n");
		// ������ʽ������ȡ������ֵ��ps����
		ps.setLeft(ps.recLeft(temp));
		if (temp == "")
		{
			throw exception("���Ҳ�Ӧʹ��~��ʾ\n");
		}
		ps.setRight(temp);
		ps.setId(id_pro);
		this->product_set.push_back(ps);
		id_pro++;
	}

	// ��������
	if (this->product_set.size() == 0)
	{
		throw exception("������\n");
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
	cout << "���ڱ���������" + file_name + FILE_TYPE + "�ļ���" << endl;
	ofstream ofs;
	ofs.open(file_name + FILE_TYPE, ios::out);
	if (!ofs)
	{
		string message = file_name + FILE_TYPE + "��ʧ�ܣ�";
		throw exception(message.data());
	}
	// ����ÿ�����������ɹ̶���ʽ�������ı������ָ���ļ���
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
	cout << "����������ϣ�" << endl;
}
#endif // _TEST_USECASE_AUTOGEN_

