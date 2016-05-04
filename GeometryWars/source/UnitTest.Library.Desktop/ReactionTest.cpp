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
			Attributed::ClearStaticMembers();

			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
		}
#endif

		TEST_METHOD(ReactionTestReaction)
		{
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

			ActionList* switchAction = entity->FindAction("switch2")->As<ActionList>();
			Assert::IsNotNull(switchAction);
			//ActionList* case3 = entity
			//Datum* switch2Datum = entityAction->Find("switch2");
			//Assert::IsNotNull(switch2Datum);

		}

		TEST_METHOD(ReactionTestMultipleSubtypes)
		{
			Game game;
			Assert::IsTrue(game.ParseMaster().ParseFromFile("Content/config/xml_reaction_test_subtype.xml"));
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

		TEST_METHOD(ReactionTestNotNotified)
		{
			Game game;
			Assert::IsTrue(game.ParseMaster().ParseFromFile("Content/config/xml_reaction_test_notnotified.xml"));
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
			Assert::AreEqual(0, a);

			Event<EventMessageAttributed>::UnsubscriberAll();
			world.GetEventQueue().Clear(*world.GetWorldState().mGameTime);
		}

		TEST_METHOD(ReactionTestArguments)
		{

			Game game;
			Assert::IsTrue(game.ParseMaster().ParseFromFile("Content/config/xml_reaction_test_arguments.xml"));
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
			Assert::AreEqual(60, a);

			Event<EventMessageAttributed>::UnsubscriberAll();
			world.GetEventQueue().Clear(*world.GetWorldState().mGameTime);
		}

		TEST_METHOD(ReactionTestEventMessageAttributed)
		{
			EventMessageAttributed msg;
			msg.SetSubtype("mousemove");
			Assert::IsTrue(msg.GetSubtype() == "mousemove");

			EventMessageAttributed msg2(msg);
			msg2.SetSubtype("mouseup");
			Assert::IsTrue(msg2[EventMessageAttributed::ATTRIBUTE_SUBTYPE].Get<std::string>() == "mouseup");
			Assert::IsTrue(msg[EventMessageAttributed::ATTRIBUTE_SUBTYPE].Get<std::string>() == "mousemove");

			EventMessageAttributed msg3;
			msg3 = msg;
			msg3.SetSubtype("mousedown");
			Assert::IsTrue(msg3[EventMessageAttributed::ATTRIBUTE_SUBTYPE].Get<std::string>() == "mousedown");
			Assert::IsTrue(msg[EventMessageAttributed::ATTRIBUTE_SUBTYPE].Get<std::string>() == "mousemove");

			EventMessageAttributed msg4(std::move(msg));
			Assert::IsTrue(msg4[EventMessageAttributed::ATTRIBUTE_SUBTYPE].Get<std::string>() == "mousemove");

			EventMessageAttributed msg5;
			msg5 = std::move(msg2);
			Assert::IsTrue(msg5[EventMessageAttributed::ATTRIBUTE_SUBTYPE].Get<std::string>() == "mouseup");

			Assert::IsTrue(msg3.Is(EventMessageAttributed::TypeIdClass()));
			Assert::IsTrue(msg3.Is(EventMessageAttributed::TypeName()));
			Assert::IsTrue(msg3.Is(msg4.TypeIdInstance()));

			Assert::IsNotNull(msg3.As<Attributed>());
			Assert::IsNotNull(msg3.AssertiveAs<Attributed>());

			Assert::IsTrue(msg3.ToString() == "Scope");
			Assert::IsTrue(msg3.Equals(&msg3));
			Assert::IsNotNull(msg3.QueryInterface(EventMessageAttributed::TypeIdClass()));
			Assert::IsNull(msg3.QueryInterface(Action::TypeIdClass()));
		}

		TEST_METHOD(ReactionTestActionEventRTTI)
		{
			ActionEvent actionEvent;
			Scope scope;

			Assert::IsTrue(actionEvent.Is(ActionEvent::TypeIdClass()));
			Assert::IsTrue(actionEvent.Is(ActionEvent::TypeName()));
			Assert::IsTrue(actionEvent.Is(scope.TypeIdInstance()));
			Assert::IsTrue(actionEvent.Is(actionEvent.TypeIdInstance()));

			Assert::IsNotNull(actionEvent.As<Attributed>());
			Assert::IsNotNull(actionEvent.AssertiveAs<Attributed>());

			Assert::IsTrue(actionEvent.ToString() == "Scope");
			Assert::IsTrue(actionEvent.Equals(&actionEvent));
			Assert::IsNotNull(actionEvent.QueryInterface(ActionEvent::TypeIdClass()));
			Assert::IsNotNull(actionEvent.QueryInterface(ActionList::TypeIdClass()));
		}

		TEST_METHOD(ReactionTestReactionAttrinbuteRTTI)
		{
			ReactionAttributed reaction;
			Scope scope;

			Assert::IsTrue(reaction.Is(Reaction::TypeIdClass()));
			Assert::IsTrue(reaction.Is(Reaction::TypeName()));
			Assert::IsTrue(reaction.Is(scope.TypeIdInstance()));
			Assert::IsTrue(reaction.Is(reaction.TypeIdInstance()));

			Assert::IsNotNull(reaction.As<Attributed>());
			Assert::IsNotNull(reaction.AssertiveAs<Attributed>());

			Assert::IsTrue(reaction.ToString() == "Scope");
			Assert::IsTrue(reaction.Equals(&reaction));
			Assert::IsNotNull(reaction.QueryInterface(Reaction::TypeIdClass()));
			Assert::IsNull(reaction.QueryInterface(ActionExpression::TypeIdClass()));

			Event<EventMessageAttributed>::UnsubscriberAll();
		}

#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState sStartMemState;
#endif
	};

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState ReactionTest::sStartMemState;
#endif
}