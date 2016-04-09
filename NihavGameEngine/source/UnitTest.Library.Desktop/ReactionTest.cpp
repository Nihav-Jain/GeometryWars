#include "pch.h"
#include "CppUnitTest.h"
#include <Windows.h>

#include "Game.h"
#include "Action.h"
#include "ActionList.h"
#include "ActionListSwitch.h"
#include "ActionExpression.h"

#include "EventMessageAttributed.h"
#include "ReactionAttributed.h"
#include "ActionEvent.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;
using namespace std::chrono;

namespace UnitTestLibraryDesktop
{

	TEST_CLASS(ReactionTest)
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
				//Assert::Fail(L"Memory Leaks!");
			}
		}
#endif

		TEST_METHOD(ReactionTestReaction)
		{
			EntityFactory entityFactory;
			ActionListFactory actionListFactory;
			ActionListSwitchFactory switchFactory;
			ActionListSwitch::ActionListSwitchCaseFactory switchCaseFactory;
			ActionExpressionFactory expFactory;
			ReactionAttributedFactory reactionAttributedFactory;
			ActionEventFactory actionEventFactory;

			Game game;
			Assert::IsTrue(game.ParseMaster().ParseFromFile("Content/config/xml_reaction_test.xml"));
			game.Start();

			World& world = game.GetWorld();
			Sector* sector = world.FindSector("worldSector");
			Assert::IsNotNull(sector);
			Entity* entity = sector->FindEntity("actor");
			Assert::IsNotNull(entity);
			Action* action = entity->FindAction("exp1");
			Assert::IsNotNull(action);
			ActionExpression* exp = action->As<ActionExpression>();
			Assert::IsNotNull(exp);

			game.Update();

			Datum* result = entity->Find("result");
			Assert::IsNotNull(result);
			std::int32_t res = result->Get<std::int32_t>();
			Assert::IsTrue(res == 19);

			Datum* result2 = entity->Find("result2");
			Assert::IsNotNull(result2);
			Assert::IsTrue(*result2 == 15);

			Datum* result3 = entity->Find("something");
			Assert::IsNotNull(result3);
			std::int32_t a = result3->Get<std::int32_t>();
			Assert::AreEqual(0, a);
			Sleep(100);
			game.Update();

			a = result3->Get<std::int32_t>();
			Assert::AreEqual(150, a);

			Event<EventMessageAttributed>::UnsubscriberAll();
			world.GetEventQueue().Clear(*world.GetWorldState().mGameTime);
		}


#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState sStartMemState;
#endif
	};

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState ReactionTest::sStartMemState;
#endif
}