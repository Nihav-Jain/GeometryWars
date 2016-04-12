#include "pch.h"
#include "CppUnitTest.h"
#include "glm\gtx\string_cast.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;

namespace UnitTestLibraryDesktop
{

	TEST_CLASS(DatumTest)
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

		TEST_METHOD(DatumTestCtor)
		{
			Datum datum;
			Assert::AreEqual(0U, datum.Size());
			Assert::AreEqual(0U, datum.Capacity());
			Assert::IsTrue(Datum::DatumType::UNKNOWN == datum.Type());
			Assert::IsTrue(Datum::DatumStorageType::UNKNOWN == datum.StorageType());
		}

		TEST_METHOD(DatumCopyCtor)
		{
			Datum intDatum;
			intDatum.Set(10);

			Datum copyDatum(intDatum);
			Assert::IsTrue(Datum::DatumType::INTEGER == copyDatum.Type());
			Assert::IsTrue(Datum::DatumStorageType::INTERNAL == copyDatum.StorageType());
			Assert::AreEqual(1U, copyDatum.Size());
			Assert::AreEqual(1U, copyDatum.Capacity());
			Assert::AreEqual(10, copyDatum.Get<std::int32_t>());

			copyDatum.Set(20);
			Assert::AreEqual(20, copyDatum.Get<std::int32_t>());
			Assert::AreEqual(10, intDatum.Get<std::int32_t>());
		}

		TEST_METHOD(DatumMoveCtor)
		{
			Datum intDatum;
			intDatum.Set(10);

			Datum copyDatum = std::move(intDatum);
			Assert::IsTrue(Datum::DatumType::INTEGER == copyDatum.Type());
			Assert::IsTrue(Datum::DatumStorageType::INTERNAL == copyDatum.StorageType());
			Assert::AreEqual(1U, copyDatum.Size());
			Assert::AreEqual(1U, copyDatum.Capacity());
			Assert::AreEqual(10, copyDatum.Get<std::int32_t>());
			copyDatum.Set(20);

			auto expression = [&] {intDatum.Get<std::int32_t>(); };
			Assert::IsTrue(Datum::DatumType::UNKNOWN == intDatum.Type());
			Assert::IsTrue(Datum::DatumStorageType::UNKNOWN == intDatum.StorageType());
			Assert::AreEqual(0U, intDatum.Size());
			Assert::AreEqual(0U, intDatum.Capacity());
			Assert::ExpectException<std::exception>(expression);
		}

		TEST_METHOD(DatumTestAssigOperator)
		{
			Datum intDatum;
			intDatum = 10;
			Assert::AreEqual(10, intDatum.Get<std::int32_t>());

			auto exp = [&] {intDatum = "string"; };
			Assert::ExpectException<std::exception>(exp);

			intDatum.SetSize(3);
			intDatum.Set(20, 1);
			intDatum.Set(30, 2);

			Datum intAssignDatum;
			intAssignDatum.SetType(Datum::DatumType::STRING);

			// internal to internal
			intAssignDatum = intDatum;
			Assert::IsTrue(Datum::DatumType::INTEGER == intAssignDatum.Type());
			Assert::IsTrue(Datum::DatumStorageType::INTERNAL == intAssignDatum.StorageType());
			Assert::AreEqual(3U, intAssignDatum.Size());
			Assert::AreEqual(3U, intAssignDatum.Capacity());
			Assert::AreEqual(10, intAssignDatum.Get<std::int32_t>(0));
			Assert::AreEqual(20, intAssignDatum.Get<std::int32_t>(1));
			Assert::AreEqual(30, intAssignDatum.Get<std::int32_t>(2));

			std::int32_t intDataArray[] = { 1, 2, 3, 4 };
			intDatum.SetStorage(intDataArray, 4);

			// external to internal
			intAssignDatum = intDatum;
			Assert::IsTrue(Datum::DatumStorageType::EXTERNAL == intAssignDatum.StorageType());
			Assert::AreEqual(4U, intAssignDatum.Size());
			Assert::AreEqual(4U, intAssignDatum.Capacity());
			Assert::AreEqual(1, intAssignDatum.Get<std::int32_t>(0));
			Assert::AreEqual(2, intAssignDatum.Get<std::int32_t>(1));
			Assert::AreEqual(3, intAssignDatum.Get<std::int32_t>(2));

			std::int32_t anotherIntDataArray[] = { 2, 4, 8, 16, 32 };
			Datum anotherIntDatum;
			anotherIntDatum.SetStorage(anotherIntDataArray, 5);

			// external to external
			intAssignDatum = anotherIntDatum;
			Assert::IsTrue(Datum::DatumStorageType::EXTERNAL == intAssignDatum.StorageType());
			Assert::AreEqual(5U, intAssignDatum.Size());
			Assert::AreEqual(5U, intAssignDatum.Capacity());
			Assert::AreEqual(2, intAssignDatum.Get<std::int32_t>(0));
			Assert::AreEqual(4, intAssignDatum.Get<std::int32_t>(1));
			Assert::AreEqual(8, intAssignDatum.Get<std::int32_t>(2));

			Datum internalIntDatum;
			internalIntDatum = 100;

			// external to internal
			intAssignDatum = internalIntDatum;
			Assert::IsTrue(Datum::DatumStorageType::INTERNAL == intAssignDatum.StorageType());
			Assert::AreEqual(1U, intAssignDatum.Size());
			Assert::AreEqual(1U, intAssignDatum.Capacity());
			Assert::AreEqual(100, intAssignDatum.Get<std::int32_t>(0));

			// float testing
			Datum floatData;
			floatData = 10.01f;
			Assert::AreEqual(10.01f, floatData.Get<std::float_t>());

			floatData.SetSize(3);
			floatData.Set(20.01f, 1);
			floatData.Set(30.01f, 2);

			intAssignDatum = floatData;
			Assert::IsTrue(Datum::DatumType::FLOAT == intAssignDatum.Type());
			Assert::IsTrue(Datum::DatumStorageType::INTERNAL == intAssignDatum.StorageType());
			Assert::AreEqual(3U, intAssignDatum.Size());
			Assert::AreEqual(3U, intAssignDatum.Capacity());
			Assert::AreEqual(10.01f, intAssignDatum.Get<std::float_t>(0));
			Assert::AreEqual(20.01f, intAssignDatum.Get<std::float_t>(1));
			Assert::AreEqual(30.01f, intAssignDatum.Get<std::float_t>(2));

			// string testing
			std::string strValue = "string";
			Datum stringDatum;
			stringDatum = "string";
			intAssignDatum = stringDatum;
			Assert::IsTrue(Datum::DatumType::STRING == intAssignDatum.Type());
			Assert::IsTrue(Datum::DatumStorageType::INTERNAL == intAssignDatum.StorageType());
			Assert::AreEqual(1U, intAssignDatum.Size());
			Assert::AreEqual(1U, intAssignDatum.Capacity());
			Assert::AreEqual(strValue, intAssignDatum.Get<std::string>());

			// vec4 testing
			Datum vec4Datum;
			vec4Datum = glm::vec4(10, 20, 30, 40);
			intAssignDatum = vec4Datum;
			Assert::IsTrue(Datum::DatumType::VECTOR4 == intAssignDatum.Type());
			Assert::IsTrue(Datum::DatumStorageType::INTERNAL == intAssignDatum.StorageType());
			Assert::AreEqual(1U, intAssignDatum.Size());
			Assert::AreEqual(1U, intAssignDatum.Capacity());
			Assert::IsTrue(glm::vec4(10, 20, 30, 40) == intAssignDatum.Get<glm::vec4>());

			// mat4 testing
			Datum mat4Datum;
			mat4Datum = glm::mat4(10);
			intAssignDatum = mat4Datum;
			Assert::IsTrue(Datum::DatumType::MATRIX4x4 == intAssignDatum.Type());
			Assert::IsTrue(Datum::DatumStorageType::INTERNAL == intAssignDatum.StorageType());
			Assert::AreEqual(1U, intAssignDatum.Size());
			Assert::AreEqual(1U, intAssignDatum.Capacity());
			Assert::IsTrue(glm::mat4(10) == intAssignDatum.Get<glm::mat4>());

			FooRTTI fooVal(20);
			Datum fooDatum;
			fooDatum = (&fooVal);
			intAssignDatum = fooDatum;
			Assert::IsTrue(Datum::DatumType::POINTER == intAssignDatum.Type());
			Assert::IsTrue(Datum::DatumStorageType::INTERNAL == intAssignDatum.StorageType());
			Assert::AreEqual(1U, intAssignDatum.Size());
			Assert::AreEqual(1U, intAssignDatum.Capacity());
			Assert::IsTrue((*intAssignDatum.Get<RTTI*>()).Equals(&fooVal));

		}

		TEST_METHOD(DatumTestMoveAssginOperator)
		{
			Datum intDatum;
			intDatum = 10;

			Datum anotherDatum;
			anotherDatum = std::move(intDatum);
			Assert::IsTrue(Datum::DatumType::INTEGER == anotherDatum.Type());
			Assert::IsTrue(Datum::DatumStorageType::INTERNAL == anotherDatum.StorageType());
			Assert::AreEqual(1U, anotherDatum.Size());
			Assert::AreEqual(1U, anotherDatum.Capacity());
			Assert::AreEqual(10, anotherDatum.Get<std::int32_t>(0));

			auto expression = [&] {intDatum.Get<std::int32_t>(); };
			Assert::IsTrue(Datum::DatumType::UNKNOWN == intDatum.Type());
			Assert::IsTrue(Datum::DatumStorageType::UNKNOWN == intDatum.StorageType());
			Assert::AreEqual(0U, intDatum.Size());
			Assert::AreEqual(0U, intDatum.Capacity());
			Assert::ExpectException<std::exception>(expression);
		}

		TEST_METHOD(DatumTestSetType)
		{
			Datum datum;
			datum.SetType(Datum::DatumType::INTEGER);
			Assert::IsTrue(Datum::DatumType::INTEGER == datum.Type());
			datum.SetType(Datum::DatumType::INTEGER);
			Assert::IsTrue(Datum::DatumType::INTEGER == datum.Type());

			auto expression = [&] {datum.SetType(Datum::DatumType::STRING); };
			Assert::ExpectException<std::exception>(expression);
		}

		TEST_METHOD(DatumTestSetSize)
		{
			Datum datum;
			auto exp1 = [&] {datum.SetSize(2); };
			Assert::ExpectException<std::exception>(exp1);

			std::int32_t anotherIntDataArray[] = { 2, 4, 8, 16, 32 };
			datum.SetStorage(anotherIntDataArray, 5);
			auto exp2 = [&] {datum.SetSize(2); };
			Assert::ExpectException<std::exception>(exp2);

			Datum intDatum;
			intDatum = 10;
			intDatum.SetSize(6);
			Assert::AreEqual(6U, intDatum.Size());
			Assert::AreEqual(6U, intDatum.Capacity());
			intDatum.SetSize(3);
			Assert::AreEqual(3U, intDatum.Size());
			Assert::AreEqual(6U, intDatum.Capacity());

			Datum floatDatum;
			floatDatum = 10.01f;
			floatDatum.SetSize(6);
			Assert::AreEqual(6U, floatDatum.Size());
			Assert::AreEqual(6U, floatDatum.Capacity());
			floatDatum.SetSize(3);
			Assert::AreEqual(3U, floatDatum.Size());
			Assert::AreEqual(6U, floatDatum.Capacity());

			Datum stringDatum;
			stringDatum = "stirng";
			stringDatum.SetSize(6);
			Assert::AreEqual(6U, stringDatum.Size());
			Assert::AreEqual(6U, stringDatum.Capacity());
			stringDatum.SetSize(3);
			Assert::AreEqual(3U, stringDatum.Size());
			Assert::AreEqual(6U, stringDatum.Capacity());

			Datum vecDatum;
			vecDatum = glm::vec4(10, 20, 30, 40);
			vecDatum.SetSize(6);
			Assert::AreEqual(6U, vecDatum.Size());
			Assert::AreEqual(6U, vecDatum.Capacity());
			vecDatum.SetSize(3);
			Assert::AreEqual(3U, vecDatum.Size());
			Assert::AreEqual(6U, vecDatum.Capacity());

			Datum matDatum;
			matDatum = glm::mat4(10);
			matDatum.SetSize(6);
			Assert::AreEqual(6U, matDatum.Size());
			Assert::AreEqual(6U, matDatum.Capacity());
			matDatum.SetSize(3);
			Assert::AreEqual(3U, matDatum.Size());
			Assert::AreEqual(6U, matDatum.Capacity());

			Datum rttiDatum;
			FooRTTI foo(10);
			rttiDatum = &foo;
			rttiDatum.SetSize(6);
			Assert::AreEqual(6U, rttiDatum.Size());
			Assert::AreEqual(6U, rttiDatum.Capacity());
			rttiDatum.SetSize(3);
			Assert::AreEqual(3U, rttiDatum.Size());
			Assert::AreEqual(6U, rttiDatum.Capacity());
		}

		TEST_METHOD(DatumTestReserve)
		{
			Datum datum;
			auto exp1 = [&] {datum.Reserve(9); };
			Assert::ExpectException<std::exception>(exp1);

			datum.SetType(Datum::DatumType::INTEGER);
			datum.Reserve(10);

			//auto exp = [&] {datum.Reserve(8); };
			//Assert::ExpectException<std::exception>(exp);

			int arr[] = { 1, 2, 3 };
			datum.SetStorage(arr, 3);

			auto exp2 = [&] {datum.Reserve(8); };
			Assert::ExpectException<std::exception>(exp2);
		}

		TEST_METHOD(DatumTestSetStorage)
		{
			Datum datum;
			std::int32_t intDataArray[] = { 1, 2, 3 };
			datum.SetType(Datum::DatumType::STRING);
			auto exp2 = [&] {datum.SetStorage(intDataArray, 3); };
			Assert::ExpectException<std::exception>(exp2);

			Datum intDatum;
			intDatum.SetStorage(intDataArray, 3);
			Assert::AreEqual(3U, intDatum.Size());
			Assert::AreEqual(3U, intDatum.Capacity());
			Assert::AreEqual(3, intDatum.Get<std::int32_t>(2));

			std::float_t floatDataArray[] = { 1.1f, 2.2f, 3.3f };
			Datum floatDatum;
			floatDatum.SetStorage(floatDataArray, 3);
			Assert::AreEqual(3U, floatDatum.Size());
			Assert::AreEqual(3U, floatDatum.Capacity());
			Assert::AreEqual(3.3f, floatDatum.Get<std::float_t>(2));


			std::string stringDataArray[] = { "1", "2", "3" };
			Datum stringDatum;
			stringDatum.SetStorage(stringDataArray, 3);
			Assert::AreEqual(3U, stringDatum.Size());
			Assert::AreEqual(3U, stringDatum.Capacity());
			Assert::AreEqual(stringDataArray[2], stringDatum.Get<std::string>(2));

			glm::vec4 vecDataArray[] = { glm::vec4(1), glm::vec4(2), glm::vec4(3)};
			Datum vecDatum;
			vecDatum.SetStorage(vecDataArray, 3);
			Assert::AreEqual(3U, vecDatum.Size());
			Assert::AreEqual(3U, vecDatum.Capacity());
			Assert::IsTrue(glm::vec4(3) == vecDatum.Get<glm::vec4>(2));

			glm::mat4 matDataArray[] = { glm::mat4(1), glm::mat4(2), glm::mat4(3) };
			Datum matDatum;
			matDatum.SetStorage(matDataArray, 3);
			Assert::AreEqual(3U, matDatum.Size());
			Assert::AreEqual(3U, matDatum.Capacity());
			Assert::IsTrue(glm::mat4(3) == matDatum.Get<glm::mat4>(2));

			FooRTTI foo1(10);
			FooRTTI foo2(20);
			FooRTTI foo3(30);

			FooRTTI** rttiDataArray = new FooRTTI*[3];
			rttiDataArray[0] = &foo1;
			rttiDataArray[1] = &foo3;
			rttiDataArray[2] = &foo2;
			
			Datum rttiDatum;
			rttiDatum.SetStorage(reinterpret_cast<RTTI**>(rttiDataArray), 3);
			Assert::AreEqual(3U, rttiDatum.Size());
			Assert::AreEqual(3U, rttiDatum.Capacity());

			Assert::IsTrue((*rttiDatum.Get<RTTI*>(2)).Equals(rttiDataArray[2]));
			delete[] rttiDataArray;
		}

		TEST_METHOD(DatumTestEqualityOperator)
		{
			Datum intDatum;
			intDatum = 10;
			Assert::IsTrue(intDatum == 10);
			Assert::IsTrue(intDatum != 20);

			Datum anotherDatum;
			Assert::IsFalse(intDatum == anotherDatum);
			Assert::IsTrue(intDatum != anotherDatum);

			anotherDatum.SetType(Datum::DatumType::INTEGER);
			anotherDatum.SetSize(3);
			Assert::IsFalse(intDatum == anotherDatum);
			Assert::IsTrue(intDatum != anotherDatum);

			Datum anotherIntDatum;
			anotherIntDatum = 10;
			Assert::IsTrue(intDatum == anotherIntDatum);
			anotherIntDatum = 20;
			Assert::IsFalse(intDatum == anotherIntDatum);

			Datum floatDatum;
			floatDatum = 10.01f;
			Assert::IsTrue(floatDatum == 10.01f);
			Assert::IsTrue(floatDatum != 20.02f);
			
			Datum anotherFloatDatum;
			anotherFloatDatum = 10.01f;
			Assert::IsTrue(floatDatum == anotherFloatDatum);


			Datum stringDatum;
			stringDatum = "string";
			Assert::IsTrue(stringDatum == "string");
			Assert::IsTrue(stringDatum != "strong");

			Datum anotherStringDatum;
			anotherStringDatum = "string";
			Assert::IsTrue(anotherStringDatum == stringDatum);


			Datum vecDatum;
			vecDatum = glm::vec4(10);
			Assert::IsTrue(vecDatum == glm::vec4(10));
			Assert::IsTrue(vecDatum != glm::vec4(20));

			Datum anotherVecDatum;
			anotherVecDatum = glm::vec4(10);
			Assert::IsTrue(anotherVecDatum == vecDatum);


			Datum matDatum;
			matDatum = glm::mat4(10);
			Assert::IsTrue(matDatum == glm::mat4(10));
			Assert::IsTrue(matDatum != glm::mat4(20));

			Datum anotherMatDatum;
			anotherMatDatum = glm::mat4(10);
			Assert::IsTrue(anotherMatDatum == matDatum);

			FooRTTI foo1(10);
			FooRTTI foo2(10);
			Datum rttiDatum;
			rttiDatum = &foo1;
			Assert::IsTrue(rttiDatum == &foo1);
			Assert::IsTrue(rttiDatum != &foo2);

			Datum anotherRttiDatum;
			anotherRttiDatum = &foo1;
			Assert::IsTrue(anotherRttiDatum == rttiDatum);
		}


		TEST_METHOD(DatumTestSet)
		{
			Datum intDatum;
			intDatum.Set(10);
			Assert::IsTrue(Datum::DatumType::INTEGER == intDatum.Type());
			Assert::IsTrue(Datum::DatumStorageType::INTERNAL == intDatum.StorageType());
			Assert::AreEqual(1U, intDatum.Size());
			Assert::AreEqual(1U, intDatum.Capacity());
			Assert::AreEqual(10, intDatum.Get<std::int32_t>());

			auto expression = [&] {intDatum.Set(10.2f); };
			Assert::ExpectException<std::exception>(expression);

			std::int32_t intDataArray[] = { 1, 2, 3, 4 };
			intDatum.SetStorage(intDataArray, 4);

			auto expression2 = [&] {intDatum.Set(10, 4); };
			Assert::ExpectException<std::exception>(expression2);

			intDatum.Set(20, 1);
			Assert::AreEqual(20, intDatum.Get<std::int32_t>(1));

			Datum floatData;
			floatData.Set(10.01f);
			Assert::AreEqual(10.01f, floatData.Get<std::float_t>());

			std::string strValue = "string";
			Datum stringDatum;
			stringDatum.Set("string");
			Assert::AreEqual(strValue, stringDatum.Get<std::string>());

			Datum vec4Datum;
			vec4Datum.Set(glm::vec4(10, 20, 30, 40));
			Assert::IsTrue(glm::vec4(10, 20, 30, 40) == vec4Datum.Get<glm::vec4>());

			Datum mat4Datum;
			mat4Datum.Set(glm::mat4(10));
			Assert::IsTrue(glm::mat4(10) == mat4Datum.Get<glm::mat4>());

			FooRTTI fooVal(20);
			Datum fooDatum;
			fooDatum.Set(&fooVal);
			Assert::IsTrue((*fooDatum.Get<Library::RTTI*>()).Equals(&fooVal));

		}

		TEST_METHOD(DatumTestGet)
		{
			Datum intDatum;
			intDatum = 10;
			auto exp1 = [&] {intDatum.Get<std::int32_t>(2); };
			auto exp2 = [&] {intDatum.Get<std::float_t>(); };
			Assert::ExpectException<std::exception>(exp1);
			Assert::ExpectException<std::exception>(exp2);

			Datum floatDatum;
			floatDatum = 10.01f;
			auto exp3 = [&] {floatDatum.Get<std::float_t>(2); };
			Assert::ExpectException<std::exception>(exp3);

			Datum stringDatum;
			stringDatum = "string";
			auto exp4 = [&] {stringDatum.Get<std::string>(2); };
			Assert::ExpectException<std::exception>(exp4);

			Datum vecDatum;
			vecDatum.SetType(Datum::DatumType::VECTOR4);
			auto exp5 = [&] {vecDatum.Get<glm::vec4>(2); };
			Assert::ExpectException<std::exception>(exp5);

			Datum matDatum;
			matDatum.SetType(Datum::DatumType::MATRIX4x4);
			auto exp6 = [&] {matDatum.Get<glm::mat4>(2); };
			Assert::ExpectException<std::exception>(exp6);

			Datum rttiDatum;
			rttiDatum.SetType(Datum::DatumType::POINTER);
			auto exp7 = [&] {rttiDatum.Get<glm::mat4>(2); };
			Assert::ExpectException<std::exception>(exp7);
		}

		TEST_METHOD(DatumTestSetFromString)
		{
			Datum intDatum;
			auto exp1 = [&] {intDatum.SetFromString("10"); };
			Assert::ExpectException<std::exception>(exp1);

			intDatum.SetType(Datum::DatumType::INTEGER);
			intDatum.SetFromString("10");
			Assert::AreEqual(10, intDatum.Get<std::int32_t>());

			Datum floatDatum;
			floatDatum.SetType(Datum::DatumType::FLOAT);
			floatDatum.SetFromString("10.01");
			Assert::AreEqual(10.01f, floatDatum.Get<std::float_t>());

			Datum stringDatum;
			stringDatum.SetType(Datum::DatumType::STRING);
			stringDatum.SetFromString("string");
			Assert::IsTrue(stringDatum == "string");

			Datum vecDatum;
			vecDatum.SetType(Datum::DatumType::VECTOR4);
			vecDatum.SetFromString("vec4(10, 20, 30, 40)");
			Assert::IsTrue(glm::vec4(10, 20, 30, 40) == vecDatum.Get<glm::vec4>());

			Datum matDatum;
			matDatum.SetType(Datum::DatumType::MATRIX4x4);
			matDatum.SetFromString("mat4x4((10, 20, 30, 40), (10, 20, 30, 40), (10, 20, 30, 40), (10, 20, 30, 40))");
			Assert::IsTrue(glm::mat4(10, 20, 30, 40, 10, 20, 30, 40, 10, 20, 30, 40, 10, 20, 30, 40) == matDatum.Get<glm::mat4>());

			Datum rttiDatum;
			rttiDatum.SetType(Datum::DatumType::POINTER);
			auto exp2 = [&] {rttiDatum.SetFromString("RTTI"); };
			Assert::ExpectException<std::exception>(exp2);
		}

		TEST_METHOD(DatumTestToString)
		{
			Datum intDatum;
			auto exp1 = [&] {intDatum.ToString(); };
			Assert::ExpectException<std::exception>(exp1);
			
			intDatum = 10;
			auto exp2 = [&] {intDatum.ToString(2); };
			Assert::ExpectException<std::exception>(exp2);

			Assert::IsTrue(intDatum.ToString().compare("10") == 0);

			Datum floatDatum;
			floatDatum = 10.01f;
			Assert::IsTrue(floatDatum.ToString().compare(std::to_string(floatDatum.Get<std::float_t>())) == 0);

			Datum stringDatum;
			stringDatum = "string";
			Assert::IsTrue(stringDatum.ToString().compare("string") == 0);

			Datum vecDatum;
			glm::vec4 vecData = glm::vec4(10, 20, 30, 40);
			vecDatum = vecData;
			Assert::IsTrue(vecDatum.ToString().compare(glm::to_string(vecData)) == 0);

			glm::mat4 mat(10);
			Datum matDatum;
			matDatum = mat;
			Assert::IsTrue(matDatum.ToString().compare(glm::to_string(mat)) == 0);


			FooRTTI foo(10);
			Datum rttiDatum;
			rttiDatum = &foo;
			Assert::IsTrue(rttiDatum.ToString().compare(foo.ToString()) == 0);

		}

		TEST_METHOD(DatumTestArithmetic)
		{
			Datum a;
			a = 10;
			Datum b;
			
			auto expression = [&] {Datum d = a + b; };
			Assert::ExpectException<std::exception>(expression);

			b = 20;
			Datum c = a + b;
			Assert::IsTrue(c == 30);

			Datum fa;
			fa = 10.10f;
			Datum fres = a + fa;
			Assert::IsTrue(fres == 20.10f);
			Datum fb;
			fb = 20.25f;
			fres = fb + fa;
			Assert::IsTrue(fres == 30.35f);

			Datum sa;
			sa = "Hello ";
			Datum sb;
			sb = "World!";
			sa = sa + sb;
			Assert::IsTrue(sa == "Hello World!");

			Datum va;
			va = glm::vec4(10, 20, 10, 20);
			Datum vb;
			vb = glm::vec4(20, 10, 20, 10);
			vb = vb + va;
			Assert::IsTrue(vb == glm::vec4(30, 30, 30, 30));

			Datum ma;
			ma = glm::mat4(10);
			Datum mb;
			mb = glm::mat4(20);
			mb = ma + mb;
			Assert::IsTrue(mb == glm::mat4(30));

			Datum suba = a - b;
			Assert::IsTrue(suba == -10);
			Datum subf = fb - fa;
			Assert::IsTrue(subf == (20.25f - 10.10f));

			Datum subv = vb - va;
			Assert::IsTrue(subv == glm::vec4(20, 10, 20, 10));

			Datum subm = mb - ma;
			Assert::IsTrue(subm == glm::mat4(20));

			Datum mula = a * b;
			Assert::IsTrue(mula == 200);
			Datum mulf = fa * fb;
			Assert::IsTrue(mulf == (20.25f * 10.10f));
			Datum mulav = a * va;
			Assert::IsTrue(mulav == glm::vec4(100, 200, 100, 200));
			Datum mulam = a * ma;
			Assert::IsTrue(mulam == glm::mat4(100));

		}

		TEST_METHOD(DatumTestComparisonOps)
		{
			Datum a;
			a = true;
			Datum b;
			b = false;

			Datum result;
			result = a && b;
			Assert::IsFalse(result.Get<bool>());
			
			result = a || b;
			Assert::IsTrue(result.Get<bool>());

			Datum inta;
			inta = 10;
			Datum intb;
			intb = 20;
			
			result = inta > intb;
			Assert::IsFalse(result.Get<bool>());

			result = inta < intb;
			Assert::IsTrue(result.Get<bool>());

			inta = 20;
			result = false;
			
			result = inta >= intb;
			Assert::IsTrue(result.Get<bool>());

			result = false;

			result = inta <= intb;
			Assert::IsTrue(result.Get<bool>());
		}

#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState sStartMemState;
#endif

	};

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState DatumTest::sStartMemState;
#endif
}