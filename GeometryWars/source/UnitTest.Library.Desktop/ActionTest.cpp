#include "pch.h"
#include "CppUnitTest.h"

#include "Game.h"
#include "Action.h"
#include "ActionList.h"
#include "ActionListSwitch.h"
#include "ActionExpression.h"
#include "ActionIfThenElse.h"
#include "ActionWhileLoop.h"

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
			Game game;

			Assert::IsTrue(game.ParseMaster().ParseFromFile("Content/config/xml_action_test.xml"));
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

			Datum* boolResult = entity->Find("boolResult");
			Assert::IsNotNull(boolResult);
			Assert::IsFalse(boolResult->Get<bool>());
			
			Datum* boolResult2 = entity->Find("boolResult2");
			Assert::IsNotNull(boolResult2);
			Assert::IsTrue(boolResult2->Get<bool>());

			Datum* someVector = sector->Find("someVector");
			Assert::IsNotNull(someVector);
			Assert::IsTrue(*someVector == glm::vec4(0, 0, 0, 0));

			Datum* worldResult = world.Find("worldResult");
			Assert::IsNotNull(worldResult);
			Assert::AreEqual(0, worldResult->Get<std::int32_t>());

			game.Update();

			Datum* result = entity->Find("result");
			Assert::IsNotNull(result);
			std::int32_t res = result->Get<std::int32_t>();
			Assert::IsTrue(res == 19);

			Datum* result2 = entity->Find("result2");
			Assert::IsNotNull(result2);
			Assert::IsTrue(*result2 == 15);

			Assert::IsTrue(boolResult->Get<bool>());
			Assert::IsFalse(boolResult2->Get<bool>());

			Assert::IsTrue(*someVector == glm::vec4(20, 40, 60, 80));
			Assert::AreEqual(3, worldResult->Get<std::int32_t>());
		}

		TEST_METHOD(ActionTestIfThenElse)
		{
			Game game;

			Assert::IsTrue(game.ParseMaster().ParseFromFile("Content/config/xml_if_else_test.xml"));
			game.Start();

			World& world = game.GetWorld();
			Sector* sector = world.FindSector("worldSector");
			Assert::IsNotNull(sector);
			Entity* entity = sector->FindEntity("actor");
			Assert::IsNotNull(entity);

			Datum* result = entity->Find("result");
			Assert::IsNotNull(result);
			Assert::AreEqual(0, result->Get<std::int32_t>());

			Datum* result2 = entity->Find("result2");
			Assert::IsNotNull(result2);
			Assert::AreEqual(0, result2->Get<std::int32_t>());

			Datum* result3 = entity->Find("result3");
			Assert::IsNotNull(result3);
			Assert::AreEqual(0, result3->Get<std::int32_t>());

			game.Update();

			Assert::AreEqual(10, result->Get<std::int32_t>());
			Assert::AreEqual(15, result2->Get<std::int32_t>());
			Assert::AreEqual(0, result3->Get<std::int32_t>());
		}

		TEST_METHOD(ActionTestWhile)
		{
			Game game;

			Assert::IsTrue(game.ParseMaster().ParseFromFile("Content/config/xml_while_test.xml"));
			game.Start();

			World& world = game.GetWorld();
			Sector* sector = world.FindSector("worldSector");
			Assert::IsNotNull(sector);
			Entity* entity = sector->FindEntity("actor");
			Assert::IsNotNull(entity);
			Assert::IsNotNull(entity);

			Datum* result = entity->Find("result");
			Assert::IsNotNull(result);
			Assert::AreEqual(0, result->Get<std::int32_t>());

			game.Update();

			Assert::AreEqual(10, result->Get<std::int32_t>());
		}

		TEST_METHOD(ActionTestExpressionFunctionCalls)
		{
			Game game;

			ActionExpression::AddFunction("min", ActionExpression::FunctionDefinition(2, [](const Vector<Datum>& params)
			{
				assert(params.Size() >= 2);
				Datum result;
				result = ((params[0] <= params[1]).Get<bool>()) ? params[0] : params[1];
				return result;
			}));

			Assert::IsTrue(game.ParseMaster().ParseFromFile("Content/config/xml_function_test.xml"));
			game.Start();

			World& world = game.GetWorld();
			Sector* sector = world.FindSector("worldSector");
			Assert::IsNotNull(sector);
			Entity* entity = sector->FindEntity("actor");
			Assert::IsNotNull(entity);

			Datum* result = entity->Find("result");
			Assert::IsNotNull(result);
			Assert::AreEqual(0, result->Get<std::int32_t>());

			Datum* minResult = entity->Find("minResult");
			Assert::IsNotNull(minResult);
			Assert::AreEqual(0, minResult->Get<std::int32_t>());

			game.Update();

			Assert::AreEqual(10, result->Get<std::int32_t>());
			Assert::AreEqual(1, minResult->Get<std::int32_t>());
		}

		TEST_METHOD(ActionTestBeginPlay)
		{
			Game game;

			Assert::IsTrue(game.ParseMaster().ParseFromFile("Content/config/xml_beginplay_test.xml"));

			World& world = game.GetWorld();
			Sector* sector = world.FindSector("worldSector");
			Assert::IsNotNull(sector);
			Entity* entity = sector->FindEntity("actor");
			Assert::IsNotNull(entity);

			Datum* worldResult = world.Find("worldInt1");
			Assert::IsNotNull(worldResult);
			Assert::AreEqual(2, worldResult->Get<std::int32_t>());

			Datum* sectorResult = sector->Find("sectorInt1");
			Assert::IsNotNull(sectorResult);
			Assert::AreEqual(2, sectorResult->Get<std::int32_t>());

			Datum* entityResult = entity->Find("entityInt1");
			Assert::IsNotNull(entityResult);
			Assert::AreEqual(2, entityResult->Get<std::int32_t>());

			game.Start();

			Assert::AreEqual(4, worldResult->Get<std::int32_t>());
			Assert::AreEqual(4, sectorResult->Get<std::int32_t>());
			Assert::AreEqual(4, entityResult->Get<std::int32_t>());

			Datum* result = entity->Find("result");
			Assert::IsNotNull(result);
			Assert::AreEqual(0, result->Get<std::int32_t>());

			game.Update();

			Assert::AreEqual(4, worldResult->Get<std::int32_t>());
			Assert::AreEqual(4, sectorResult->Get<std::int32_t>());
			Assert::AreEqual(4, entityResult->Get<std::int32_t>());

			Assert::AreEqual(10, result->Get<std::int32_t>());
		}

		TEST_METHOD(ActionTestExpressionLiteral)
		{
			Game game;

			Assert::IsTrue(game.ParseMaster().ParseFromFile("Content/config/xml_literal_test.xml"));

			World& world = game.GetWorld();
			Sector* sector = world.FindSector("worldSector");
			Assert::IsNotNull(sector);
			Entity* entity = sector->FindEntity("actor");
			Assert::IsNotNull(entity);

			game.Start();

			Datum* result = entity->Find("intResult");
			Assert::IsNotNull(result);
			Assert::AreEqual(0, result->Get<std::int32_t>());

			Datum* floatResult = entity->Find("floatResult");
			Assert::IsNotNull(floatResult);
			Assert::AreEqual(0.0f, floatResult->Get<std::float_t>());

			Datum* stringResult = entity->Find("strResult");
			Assert::IsNotNull(stringResult);
			Assert::IsTrue("asclkn" == stringResult->Get<std::string>());

			Datum* vecResult = entity->Find("vecResult");
			Assert::IsNotNull(vecResult);
			Assert::IsTrue(glm::vec4(0, 0, 0, 0) == vecResult->Get<glm::vec4>());

			Datum* matResult = entity->Find("matResult");
			Assert::IsNotNull(matResult);
			Assert::IsTrue(glm::mat4x4(0) == matResult->Get<glm::mat4>());

			Datum* intResult2 = entity->Find("intResult2");
			Assert::IsNotNull(intResult2);
			Assert::AreEqual(0, intResult2->Get<std::int32_t>());

			game.Update();

			Assert::AreEqual(100, result->Get<std::int32_t>());
			Assert::AreEqual(10.12f, floatResult->Get<std::float_t>());
			Assert::IsTrue("some string" == stringResult->Get<std::string>());
			Assert::IsTrue(glm::vec4(10, 20, 30, 40) == vecResult->Get<glm::vec4>());
			Assert::IsTrue(glm::mat4x4(10) == matResult->Get<glm::mat4>());
			Assert::AreEqual(135, intResult2->Get<std::int32_t>());

		}

#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState sStartMemState;
#endif
	};

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState ActionTest::sStartMemState;
#endif
}