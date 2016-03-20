#include "pch.h"
#include "CppUnitTest.h"

#include "XmlParseMaster.h"
#include "SharedDataTable.h"
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
			SharedDataTable::ClearStateGraph();
			XmlParseHelperPrimitives::ClearStaticMembers();

			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
		}
#endif


		TEST_METHOD(XmlParseTable)
		{
			SharedDataTable sharedData;
			XmlParseMaster master(sharedData);
			XmlParseHelperTable tableParser;
			XmlParseHelperPrimitives primitivesParser;
			XmlParseHelperNameValue nameValueParser;

			master.AddHelper(tableParser);
			master.AddHelper(primitivesParser);
			master.AddHelper(nameValueParser);

			Assert::IsTrue(master.ParseFromFile("Content/config/xmltabletest.xml"));
			
			Datum* rootScopeDatum = sharedData.RootScope().Find("rootscope");
			Assert::IsTrue(rootScopeDatum != nullptr);
			Assert::IsTrue(sharedData.RootScope().Find("assac") == nullptr);
			Datum* firstChildScopeDatum = rootScopeDatum->Get<Scope*>()->Find("firstchildscope");
			Assert::IsTrue(firstChildScopeDatum != nullptr);

			Scope& firstChildScope = *firstChildScopeDatum->Get<Scope*>();

			// int
			Datum* intvarDatum = firstChildScope.Find("intvar");
			Assert::IsTrue(intvarDatum != nullptr);
			Assert::IsTrue(*intvarDatum == 10);

			Datum* intvar2Datum = firstChildScope.Find("intvar2");
			Assert::IsTrue(intvar2Datum != nullptr);
			Assert::IsTrue(*intvar2Datum == 20);

			Datum* intvar3Datum = firstChildScope.Find("intvar3");
			Assert::IsTrue(intvar3Datum != nullptr);
			Assert::IsTrue(*intvar3Datum == 30);

			// float
			Datum* floatvarDatum = firstChildScope.Find("floatvar");
			Assert::IsTrue(floatvarDatum != nullptr);
			Assert::IsTrue(*floatvarDatum == 10.10f);

			Datum* floatvar2Datum = firstChildScope.Find("floatvar2");
			Assert::IsTrue(floatvar2Datum != nullptr);
			Assert::IsTrue(*floatvar2Datum == 20.20f);

			Datum* floatvar3Datum = firstChildScope.Find("floatvar3");
			Assert::IsTrue(floatvar3Datum != nullptr);
			Assert::IsTrue(*floatvar3Datum == 30.30f);

			// string
			Datum* strvarDatum = firstChildScope.Find("stringvar");
			Assert::IsTrue(strvarDatum != nullptr);
			Assert::IsTrue(*strvarDatum == "stringvalue");

			Datum* strvar2Datum = firstChildScope.Find("stringvar2");
			Assert::IsTrue(strvar2Datum != nullptr);
			Assert::IsTrue(*strvar2Datum == "stringvalue2");

			Datum* strvar3Datum = firstChildScope.Find("stringvar3");
			Assert::IsTrue(strvar3Datum != nullptr);
			Assert::IsTrue(*strvar3Datum == "stringvalue3");

			// vector
			Datum* vecvarDatum = firstChildScope.Find("position");
			Assert::IsTrue(vecvarDatum != nullptr);
			Assert::IsTrue(*vecvarDatum == glm::vec4(10, 20, 30, 1));

			Datum* enemyPositionDatum = firstChildScope.Find("enemy_position");
			Assert::IsTrue(enemyPositionDatum != nullptr);
			Assert::IsTrue(*enemyPositionDatum == glm::vec4(100, 200, 300, 10));

			Datum* targetPositionDatum = firstChildScope.Find("target_position");
			Assert::IsTrue(targetPositionDatum != nullptr);
			Assert::IsTrue(*targetPositionDatum == glm::vec4(1, 2, 3, 4));

			// matrix
			Datum* matDatum = firstChildScope.Find("transform");
			Assert::IsTrue(matDatum != nullptr);
			Assert::IsTrue(*matDatum == glm::mat4(10));

			Datum* enemyMatDatum = firstChildScope.Find("enemy_transform");
			Assert::IsTrue(enemyMatDatum != nullptr);
			Assert::IsTrue(*enemyMatDatum == glm::mat4(100));

			Datum* targetMatDatum = firstChildScope.Find("target_transform");
			Assert::IsTrue(targetMatDatum != nullptr);
			Assert::IsTrue(*targetMatDatum == glm::mat4(2));

			XmlParseMaster* clonedMaster = master.Clone();
			auto expression = [&] {clonedMaster->ParseFromFile("Content/config/xml_invalid_table.xml"); };
			Assert::ExpectException<std::exception>(expression);
			
			auto expression2 = [&] {clonedMaster->ParseFromFile("Content/config/xml_invalid_table_state.xml"); };
			Assert::ExpectException<std::exception>(expression2);

			delete clonedMaster;
		}

#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState sStartMemState;
#endif
	};

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState XmlParseHelperTest::sStartMemState;
#endif
}