#include "pch.h"
#include "CppUnitTest.h"

#include "XmlParseMaster.h"
#include "XmlParseHelperTable.h"
#include "XmlParseHelperPrimitives.h"
#include "XmlParseHelperNameValue.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;

namespace UnitTestLibraryDesktop
{

	TEST_CLASS(XmlParseHelperTest)
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
			XmlParseHelperTable::SharedDataTable::ClearStateGraph();

			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				//Assert::Fail(L"Memory Leaks!");
			}
		}
#endif


		TEST_METHOD(XmlParseTable)
		{
			XmlParseHelperTable::SharedDataTable sharedData;
			XmlParseMaster master(sharedData);
			XmlParseHelperTable tableParser;
			XmlParseHelperPrimitives primitivesParser;
			XmlParseHelperNameValue nameValueParser;
			master.AddHelper(tableParser);
			master.AddHelper(primitivesParser);
			master.AddHelper(nameValueParser);

			Assert::IsTrue(master.ParseFromFile("Content/config/xmltabletest.xml"));
			
			Datum* rootScopeDatum = sharedData.RootScope.Find("rootscope");
			Assert::IsTrue(rootScopeDatum != nullptr);
			Assert::IsTrue(sharedData.RootScope.Find("assac") == nullptr);
			Datum* firstChildScopeDatum = rootScopeDatum->Get<Scope*>()->Find("firstchildscope");
			Assert::IsTrue(firstChildScopeDatum != nullptr);

			Scope& firstChildScope = *firstChildScopeDatum->Get<Scope*>();
			Datum* intvarDatum = firstChildScope.Find("intvar");
			Assert::IsTrue(intvarDatum != nullptr);
			Assert::IsTrue(*intvarDatum == 10);

			Datum* intvar2Datum = firstChildScope.Find("intvar2");
			Assert::IsTrue(intvar2Datum != nullptr);
			Assert::IsTrue(*intvar2Datum == 20);

			Datum* intvar3Datum = firstChildScope.Find("intvar3");
			Assert::IsTrue(intvar3Datum != nullptr);
			Assert::IsTrue(*intvar3Datum == 30);
		}

#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState sStartMemState;
#endif
	};

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState XmlParseHelperTest::sStartMemState;
#endif
}