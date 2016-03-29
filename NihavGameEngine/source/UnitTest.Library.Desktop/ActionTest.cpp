#include "pch.h"
#include "CppUnitTest.h"

#include "Game.h"
#include "Action.h"
#include "ActionList.h"
#include "ActionListSwitch.h"

#include "XmlParseHelperActionSwitch.h"

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
			delete &Game::Instance();

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


			XmlParseHelperActionSwitch switchParseHelper;
			XmlParseHelperActionSwitch::XmlParseHelperActionSwitchCase caseParseHelper;

			Game::Instance().AddXmlParseHelper(switchParseHelper);
			Game::Instance().AddXmlParseHelper(caseParseHelper);

			Assert::IsTrue(Game::Instance().ParseFromFile("Content/config/xml_action_test.xml"));
			Game::Instance().Start();
			Game::Instance().Update();
		}
		

#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState sStartMemState;
#endif
	};

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState ActionTest::sStartMemState;
#endif
}