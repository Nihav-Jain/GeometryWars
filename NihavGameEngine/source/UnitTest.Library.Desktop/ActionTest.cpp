#include "pch.h"
#include "CppUnitTest.h"

#include "Game.h"
#include "Action.h"
#include "ActionList.h"
#include "ActionListSwitch.h"
#include "ActionExpression.h"

#include "XmlParseHelperActionSwitch.h"
#include "XmlParseHelperActionExpression.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;

namespace UnitTestLibraryDesktop
{

	TEST_CLASS(ActionTest)
	{
	public:

#if defined(DEBUG) | defined(_DEBUG)
		TEST_METHOD_INITIALIZE(Initialize)
		{
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
		}
#endif

		TEST_METHOD(ActionTestList)
		{
			EntityFactory entityFactory;
			ActionListFactory actionListFactory;
			ActionListSwitchFactory switchFactory;
			ActionListSwitch::ActionListSwitchCaseFactory switchCaseFactory;
			ActionExpressionFactory expFactory;

			XmlParseHelperActionSwitch switchParseHelper;
			XmlParseHelperActionSwitch::XmlParseHelperActionSwitchCase caseParseHelper;
			XmlParseHelperActionExpression expParseHelper;

			Game game;

			game.ParseMaster().AddHelper(switchParseHelper);
			game.ParseMaster().AddHelper(caseParseHelper);
			game.ParseMaster().AddHelper(expParseHelper);

			Assert::IsTrue(game.ParseMaster().ParseFromFile("Content/config/xml_action_test.xml"));
			game.Start();
			game.Update();

			World& world = game.GetWorld();
			Sector* sector = world.FindSector("worldSector");
			Assert::IsNotNull(sector);
			Entity* entity = sector->FindEntity("actor");
			Assert::IsNotNull(entity);
			Action* action = entity->FindAction("exp1");
			Assert::IsNotNull(action);
			ActionExpression* exp = action->As<ActionExpression>();
			Assert::IsNotNull(exp);

			SList<std::string>::Iterator itr = exp->mPostfixExpression->begin();
			Assert::IsTrue(*itr == "switchInteger");
			++itr;
			Assert::IsTrue(*itr == "abc");
			++itr;
			Assert::IsTrue(*itr == "+");
			++itr;
			Assert::IsTrue(*itr == "C");
			++itr;
			Assert::IsTrue(*itr == "-");
			++itr;
			Assert::IsTrue(itr == exp->mPostfixExpression->end());

			action = entity->FindAction("exp2");
			Assert::IsNotNull(action);
			exp = action->As<ActionExpression>();
			Assert::IsNotNull(exp);

			itr = exp->mPostfixExpression->begin();
			Assert::IsTrue(*itr == "switchInteger");
			++itr;
			Assert::IsTrue(*itr == "abc");
			++itr;
			Assert::IsTrue(*itr == "C");
			++itr;
			Assert::IsTrue(*itr == "-");
			++itr;
			Assert::IsTrue(*itr == "xyz");
			++itr;
			Assert::IsTrue(*itr == "/");
			++itr;
			Assert::IsTrue(*itr == "+");
			++itr;
			Assert::IsTrue(itr == exp->mPostfixExpression->end());


			action = entity->FindAction("exp3");
			Assert::IsNotNull(action);
			exp = action->As<ActionExpression>();
			Assert::IsNotNull(exp);

			itr = exp->mPostfixExpression->begin();
			Assert::IsTrue(*itr == "A");
			++itr;
			Assert::IsTrue(*itr == "B");
			++itr;
			Assert::IsTrue(*itr == "max");
			++itr;
			Assert::IsTrue(*itr == "C");
			++itr;
			Assert::IsTrue(*itr == "/");
			++itr;
			Assert::IsTrue(*itr == "D");
			++itr;
			Assert::IsTrue(*itr == "*");
			++itr;
			Assert::IsTrue(*itr == "sin");
			++itr;
			Assert::IsTrue(itr == exp->mPostfixExpression->end());
		}
		

#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState sStartMemState;
#endif
	};

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState ActionTest::sStartMemState;
#endif
}