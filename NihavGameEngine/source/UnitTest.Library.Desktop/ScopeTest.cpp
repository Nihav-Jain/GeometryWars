#include "pch.h"
#include "CppUnitTest.h"
#include "glm\gtx\string_cast.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;

namespace UnitTestLibraryDesktop
{

	TEST_CLASS(ScopeTest)
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

		TEST_METHOD(ScopeTestCtor)
		{
			Scope scope(10);
			Assert::IsNull(scope.GetParent());
		}

		TEST_METHOD(ScopeTestAppend)
		{
			Scope scope;

			auto expression = [&] {scope.Append(""); };
			Assert::ExpectException<std::exception>(expression);

			Datum& datum1 = scope.Append("power");
			datum1 = 10;

			Assert::IsTrue(scope.Append("power") == datum1);
			Assert::IsFalse(scope.Append("health") == datum1);
		}

		TEST_METHOD(ScopeTestAppendScope)
		{
			Scope rootScope;
			Scope& powerScope = rootScope.AppendScope("power");
			Assert::IsTrue(powerScope.GetParent() == &rootScope);

			Scope& anotherPowerScope = rootScope.AppendScope("power");
			Assert::IsTrue(powerScope.GetParent() == &rootScope);
			Datum& powerDatum = rootScope.Append("power");
			Assert::AreEqual(2U, powerDatum.Size());

			Datum& healthDatum = rootScope.Append("health");
			healthDatum = 100;
			auto expression = [&] {rootScope.AppendScope("health"); };
			Assert::ExpectException<std::exception>(expression);

			UNREFERENCED_PARAMETER(anotherPowerScope);
		}

		TEST_METHOD(ScopeTestFind)
		{
			Scope rootScope;
			Assert::IsNull(rootScope.Find(""));

			Datum& datum1 = rootScope.Append("power");
			Assert::IsTrue(&datum1 == rootScope.Find("power"));
			Assert::IsNull(rootScope.Find("health"));
		}

		TEST_METHOD(ScopeTestSearch)
		{
			Scope rootScope;
			Datum& health = rootScope.Append("health");

			Scope& powerScope = rootScope.AppendScope("power");
			Scope& firePowerScope = powerScope.AppendScope("fire power");

			Scope* searchedScope;
			Assert::IsTrue(&health == firePowerScope.Search("health", &searchedScope));
			Assert::IsTrue(&rootScope == searchedScope);

			Assert::IsNull(firePowerScope.Search("gibberish"));
		}

		TEST_METHOD(ScopeTestOrphan)
		{
			Scope rootScope;
			Datum& healthDatum = rootScope.Append("health");
			healthDatum = 100;

			Scope& powerScope1 = rootScope.AppendScope("power");
			Datum& attackPowerDatum = powerScope1.Append("attackPower");
			attackPowerDatum = 1000;

			Scope& powerScope2 = rootScope.AppendScope("power");
			Datum& attackPower2Datum = powerScope2.Append("attackPower");
			attackPowerDatum = 2000;

			Scope& firePowerScope = powerScope1.AppendScope("firepower");
			
			Assert::AreEqual(2U, rootScope.Find("power")->Size());

			powerScope1.Orphan();
			Assert::IsNull(powerScope1.GetParent());
			Assert::AreEqual(1U, rootScope.Find("power")->Size());
			Assert::IsTrue(&powerScope2 == &rootScope.Find("power")->Get<Scope>());

			delete &powerScope1;

			UNREFERENCED_PARAMETER(firePowerScope);
			UNREFERENCED_PARAMETER(attackPower2Datum);
		}

		TEST_METHOD(ScopeTestAdopt)
		{
			Scope rootScope;
			Datum& healthDatum = rootScope.Append("health");
			healthDatum = 100;

			Scope& powerScope1 = rootScope.AppendScope("power");
			Datum& attackPowerDatum = powerScope1.Append("attackPower");
			attackPowerDatum = 1000;

			Scope powerScope2;
			Datum& attackPower2Datum = powerScope2.Append("attackPower");
			attackPowerDatum = 2000;

			rootScope.Adopt("power", powerScope2);
			Assert::IsTrue(powerScope2.GetParent() == &rootScope);
			Assert::AreEqual(2U, rootScope.Find("power")->Size());

			auto expression = [&] {powerScope2.Adopt("abcd", rootScope); };
			Assert::ExpectException<std::exception>(expression);
			UNREFERENCED_PARAMETER(attackPower2Datum);
		}

		TEST_METHOD(ScopeTestIndexOperator)
		{
			Scope rootScope;
			Datum& healthDatum = rootScope.Append("health");
			healthDatum = 100;

			Scope& powerScope1 = rootScope.AppendScope("power");
			Datum& attackPowerDatum = powerScope1.Append("attackPower");
			attackPowerDatum = 1000;

			Assert::AreEqual(100, rootScope["health"].Get <std::int32_t>());
			Assert::IsTrue(rootScope[1].Type() == Datum::DatumType::TABLE);

			rootScope["walkSpeed"] = 10;
			Datum* walkSpeed = rootScope.Find("walkSpeed");
			Assert::IsNotNull(walkSpeed);
			Assert::IsTrue(walkSpeed->Get<std::int32_t>() == 10);
		}

		TEST_METHOD(ScopeTestEqualityOperator)
		{
			Scope rootScope;
			Datum& healthDatum = rootScope.Append("health");
			healthDatum = 100;

			Scope& powerScope = rootScope.AppendScope("power");
			Datum& attackPowerDatum = powerScope.Append("attackPower");
			attackPowerDatum = 1000;

			Scope rootScope2;
			Datum& healthDatum2 = rootScope2.Append("health");
			healthDatum2 = 100;

			Scope& powerScope2 = rootScope2.AppendScope("power");
			Datum& attackPowerDatum2 = powerScope2.Append("attackPower");
			attackPowerDatum2 = 1000;

			Assert::IsTrue(rootScope == rootScope2);

			Scope rootScope3;
			Scope& powerScope3 = rootScope3.AppendScope("power");
			Datum& attackPowerDatum3 = powerScope3.Append("attackPower");
			attackPowerDatum3 = 1000;

			Datum& healthDatum3 = rootScope3.Append("health");
			healthDatum3 = 100;

			Assert::IsTrue(rootScope != rootScope3);

			Scope rootScope4;
			Datum& healthDatum4 = rootScope4.Append("health");
			healthDatum4 = 100;

			Scope& powerScope4 = rootScope4.AppendScope("power2");
			Datum& attackPowerDatum4 = powerScope4.Append("attackPower");
			attackPowerDatum4 = 1000;

			Assert::IsFalse(rootScope == rootScope4);
		}

		TEST_METHOD(ScopeTestFindName)
		{
			Scope rootScope;
			Datum& healthDatum = rootScope.Append("health");
			healthDatum = 100;

			Scope& powerScope = rootScope.AppendScope("power");
			Datum& attackPowerDatum = powerScope.Append("attackPower");
			attackPowerDatum = 1000;

			Scope& powerScope2 = rootScope.AppendScope("power");

			Assert::IsTrue(rootScope.FindName(powerScope2) == "power");

			Scope randomScope;
			Assert::IsTrue(rootScope.FindName(randomScope).empty());
		}

		TEST_METHOD(ScopeTestClear)
		{
			Scope rootScope;
			Datum& healthDatum = rootScope.Append("health");
			healthDatum = 100;

			Scope& powerScope1 = rootScope.AppendScope("power");
			Datum& attackPowerDatum = powerScope1.Append("attackPower");
			attackPowerDatum = 1000;

			Scope& firePowerScope = powerScope1.AppendScope("firePower");
			Datum& fireAttackPower = firePowerScope.Append("attackPower");
			fireAttackPower = 10.2f;

			powerScope1.Clear();
			Assert::IsNull(powerScope1.GetParent());
			Assert::IsTrue(rootScope.FindName(powerScope1).empty());

			delete &powerScope1;
		}

		TEST_METHOD(ScopeTestCopyCtor)
		{
			Scope rootScope;
			Datum& healthDatum = rootScope.Append("health");
			healthDatum = 100;

			Scope& powerScope1 = rootScope.AppendScope("power");
			Datum& attackPowerDatum = powerScope1.Append("attackPower");
			attackPowerDatum = 1000;

			Scope& firePowerScope = powerScope1.AppendScope("firePower");
			Datum& fireAttackPower = firePowerScope.Append("attackPower");
			fireAttackPower = 10.2f;
			
			Scope rootScope2(rootScope);

			Assert::IsTrue(rootScope == rootScope2);
		}

		TEST_METHOD(ScopeTestAssignmentOperator)
		{
			Scope rootScope;
			Datum& healthDatum = rootScope.Append("health");
			healthDatum = 100;

			Scope& powerScope1 = rootScope.AppendScope("power");
			Datum& attackPowerDatum = powerScope1.Append("attackPower");
			attackPowerDatum = 1000;

			Scope& firePowerScope = powerScope1.AppendScope("firePower");
			Datum& fireAttackPower = firePowerScope.Append("attackPower");
			fireAttackPower = 10.2f;

			Scope rootScope2;
			Scope& powerScope2 = rootScope2.AppendScope("power");
			Datum& attackPowerDatum2 = powerScope2.Append("attackPower");
			attackPowerDatum2 = 2000;

			Scope& firePowerScope2 = powerScope2.AppendScope("firePower");
			Datum& fireAttackPower2 = firePowerScope2.Append("attackPower");
			fireAttackPower2 = 20.2f;

			Assert::IsTrue(rootScope != rootScope2);
			rootScope2 = rootScope;
			Assert::IsTrue(rootScope == rootScope2);
		}

		TEST_METHOD(ScopeTestMoveCtor)
		{
			Scope rootScope;
			Datum& healthDatum = rootScope.Append("health");
			healthDatum = 100;

			Scope& powerScope1 = rootScope.AppendScope("power");
			Datum& attackPowerDatum = powerScope1.Append("attackPower");
			attackPowerDatum = 1000;

			Scope& firePowerScope = powerScope1.AppendScope("firePower");
			Datum& fireAttackPower = firePowerScope.Append("attackPower");
			fireAttackPower = 10.2f;

			Scope rootScope2 = std::move(rootScope);
			Assert::IsNull(rootScope2.GetParent());
			Assert::IsNotNull(rootScope2.Find("health"));
			Assert::IsTrue(&rootScope2.Find("power")->Get<Scope>() == &powerScope1);

			Scope powerScope2 = std::move(powerScope1);

			delete &powerScope1;
		}

		TEST_METHOD(ScopeTestMoveAssignmentOperator)
		{
			Scope rootScope;
			Datum& healthDatum = rootScope.Append("health");
			healthDatum = 100;

			Scope& powerScope1 = rootScope.AppendScope("power");
			Datum& attackPowerDatum = powerScope1.Append("attackPower");
			attackPowerDatum = 1000;

			Scope& firePowerScope = powerScope1.AppendScope("firePower");
			Datum& fireAttackPower = firePowerScope.Append("attackPower");
			fireAttackPower = 10.2f;

			Scope rootScope2;
			Scope& powerScope2 = rootScope2.AppendScope("power");
			Datum& attackPowerDatum2 = powerScope2.Append("attackPower");
			attackPowerDatum2 = 2000;

			Scope& firePowerScope2 = powerScope2.AppendScope("firePower");
			Datum& fireAttackPower2 = firePowerScope2.Append("attackPower");
			fireAttackPower2 = 20.2f;

			Scope rootScope3;
			Scope& powerScope3 = rootScope3.AppendScope("power");
			Datum& attackPowerDatum3 = powerScope3.Append("attackPower");
			attackPowerDatum3 = 2000;

			Scope& firePowerScope3 = powerScope3.AppendScope("firePower");
			Datum& fireAttackPower3 = firePowerScope3.Append("attackPower");
			fireAttackPower3 = 20.2f;

			Assert::IsTrue(rootScope2 == rootScope3);
			rootScope2 = std::move(rootScope);
			Assert::IsTrue(rootScope2 != rootScope3);
		}

		TEST_METHOD(ScopeTestToString)
		{
			Scope rootScope;
			Assert::AreEqual(std::string("Scope"), rootScope.ToString());

			RTTI* scopePtr = &rootScope;
			Assert::AreEqual(std::string("Scope"), scopePtr->ToString());
		}

		TEST_METHOD(ScopeTestEquals)
		{
			Scope rootScope;
			Datum& healthDatum = rootScope.Append("health");
			healthDatum = 100;

			Scope& powerScope = rootScope.AppendScope("power");
			Datum& attackPowerDatum = powerScope.Append("attackPower");
			attackPowerDatum = 1000;

			Scope rootScope2;
			Datum& healthDatum2 = rootScope2.Append("health");
			healthDatum2 = 100;

			Scope& powerScope2 = rootScope2.AppendScope("power");
			Datum& attackPowerDatum2 = powerScope2.Append("attackPower");
			attackPowerDatum2 = 1000;

			Assert::IsTrue(rootScope.Equals(&rootScope2));
			FooRTTI foo(10);
			Assert::IsFalse(rootScope.Equals(&foo));
		}

		TEST_METHOD(ScopeTestRTTIMethods)
		{
			Scope rootScope;
			FooRTTI foo;

			Assert::AreEqual(std::string("Scope"), rootScope.TypeName());
			Assert::IsTrue(foo.TypeIdInstance() != rootScope.TypeIdInstance());
		}

#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState sStartMemState;
#endif

	};

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState ScopeTest::sStartMemState;
#endif
}