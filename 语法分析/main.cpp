#include "FollowSet_Generator.h"
#include "Follow_Graph.h"
#include "Printable_Stack.h"
#include "LR(0)_Analysis_AutoGen.h"
#include "LL(1)_Table_Gen.h"

#ifdef _UPTODATE_
	#ifdef _TEST_FIRSTSET_GEN_
	int main()
	{
		try
		{
			First_Gen ffg;
			// 文法的读取
			ffg.read();
			ffg.init();
			ffg.Parser::print(false);
			cout << endl << endl;
			// FIRST集的计算、输出
			ffg.compute();
			ffg.print(false, false);
		}
		catch (const std::exception &e)
		{
			cout << e.what();
		}
		return 0;
	}
	#endif

	#ifdef _TEST_FOLLOWSET_GEN_
	int main()
	{
		try
		{
			// FOLLOW集的计算、输出
			Follow_Gen fog;
			fog.read();
			fog.init();
			fog.compute();
			fog.print(false, false);
		}
		catch (const std::exception &e)
		{
			cout << e.what();
		}
		return 0;
	}
	#endif // _TEST_FOLLOWSET_GEN_


	#ifdef _TEST_FOLLOW_GRAPH_
	int main()
	{
		try
		{
			Follow_Graph FG = Follow_Graph();
			pair<TER, NTER> temp('#', string(""));
			pair<TER, NTER> temp2(NULL, string("12"));
			pair<TER, NTER> temp3(NULL, string("61"));
			temp.first = '#';
			temp.second = string("");
			FG.insertVertex(Follow, temp);
			FG.insertVertex(T, temp);
			FG.insertVertex(First, temp);
			FG.insertVertex(First, temp2);
			FG.insertVertex(Follow, temp2);
			auto head = FG.getVertex(0);
			auto node1 = FG.getVertex(1);
			auto node2 = FG.getVertex(2);
			auto node3 = FG.getVertex(3);
			auto node4 = FG.getVertex(Follow, temp2);
			auto node5 = FG.getVertex(First, temp2);
			auto node6 = FG.getVertex(First, temp3);
			FG.addArc(*head, *node1);
			FG.addArc(*node1, *node1);
			FG.addArc(node2, node1);
			FG.addArc(node2, node3);
		}
		catch (const std::exception &e)
		{
			cout << e.what();
		}

	}
	#endif

	#ifdef _TEST_LR0_ANALYSIS_
	int main()
	{
		try
		{
			// 非自动生成
			LR0_Analysis LR = LR0_Analysis();
			LR.read();
			LR.init();
			LR.compute();
			LR.print(false, true);
		}
		catch (const std::exception &e)
		{
			cout << e.what();
		}
	}
	#endif // _TEST_LR0_ANALYSIS_

	#ifdef _TEST_LR0_AUTO_
	int main()
	{
		try
		{
			LR0_Analysis_AutoGen LR = LR0_Analysis_AutoGen();
			LR.read();
			LR.init();
			LR.compute();
			LR.print(false, true);
		}
		catch (const std::exception &e)
		{
			cout << e.what();
		}
	}
	#endif // _TEST_LR0_AUTO_

	#ifdef _TEST_LL1_TABLE_GEN_
		int main()
		{
			try
			{
				LL1_Table_Gen ll1;
				ll1.read();
				ll1.init();
				ll1.compute();
				ll1.print(true, true);
			}
			catch (const std::exception& e)
			{
				cout << e.what();
			}
		}
	#endif // _TEST_LL1_TABLE_GEN_

	#ifdef _TEST_USECASE_AUTOGEN_
	int main()
	{
		try
		{
			//UseCase_AutoGen *UA = new First_Gen();
			//UseCase_AutoGen *UA = new Follow_Gen();
			//UseCase_AutoGen *UA = new LR0_Analysis();
			//UseCase_AutoGen *UA = new LR0_Analysis_AutoGen();
			UseCase_AutoGen *UA = new LL1_Table_Gen();

			UseCases useCases = UA->getUseCases();

			//UA->saveUseCases(useCases, "usecase_FIRST");
			//UA->saveUseCases(useCases, "usecase_FOLLOW");
			//UA->saveUseCases(useCases, "usecase_LR(0)Analysis");
			//UA->saveUseCases(useCases, "usecase_LR(0)Analysis_Auto");
			UA->saveUseCases(useCases, "usecase_PreAndAna_Table");

			//UA->read();
			//UA->init();
			//UA->compute();
			//OutputTable outputTable = UA->getOutput();
			return 0;
		}
		catch (const std::exception &e)
		{
			cout << e.what() << endl;
		}
	}
	#endif // _TEST_USECASE_AUTOGEN_

#endif // _UPTODATE_

