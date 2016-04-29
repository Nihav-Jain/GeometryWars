#include "pch.h"
#include "CppUnitTest.h"

#include "AttributedInternalError.h"
#include "AttributedExternalError.h"
#include "EventMessageAttributed.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;

namespace UnitTestLibraryDesktop
{

	TEST_CLASS(AttributedTest)
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

		TEST_METHOD(AttributedTestCtor)
		{
			AttributedFoo foo;
			Assert::IsTrue(foo["this"].Get<RTTI*>()->Is("AttributedFoo"));

			AttributedFooChild fooChild;
			Assert::IsTrue(fooChild["this"].Get<RTTI*>()->Is("AttributedFooChild"));
		}

		TEST_METHOD(AttributedTestCopyCtor)
		{
			AttributedFoo foo;
			AttributedFooChild fooChild;

			Datum& fooAux1 = foo.AppendAuxiliaryAttribute("fooAux1");
			fooAux1 = 10;
			Datum& fooAux2 = foo.AppendAuxiliaryAttribute("fooAux2");
			fooAux2 = "i am an auxiliary attribute";

			Datum& fooChildAux1 = fooChild.AppendAuxiliaryAttribute("fooChildAux1");
			fooChildAux1 = 10;
			Datum& fooChildAux2 = fooChild.AppendAuxiliaryAttribute("fooChildAux2");
			fooChildAux2 = "i am an auxiliary attribute";

			AttributedFoo fooCopy(foo);
			Assert::IsTrue(fooCopy["this"].Get<RTTI*>()->Is("AttributedFoo"));
			Assert::AreEqual(14U, fooCopy.AuxiliaryBegin());

			Assert::AreEqual(10, foo["fooAux1"].Get<std::int32_t>());
			fooCopy["fooAux1"] = 20;
			Assert::AreEqual(10, foo["fooAux1"].Get<std::int32_t>());
			Assert::AreEqual(20, fooCopy["fooAux1"].Get<std::int32_t>());

			Assert::IsTrue(&foo == foo["this"].Get<RTTI*>());
			Assert::IsTrue(&fooCopy == fooCopy["this"].Get<RTTI*>());
			Assert::IsTrue(&foo != fooCopy["this"].Get<RTTI*>());


			AttributedFooChild fooChildCopy(fooChild);
			Assert::IsTrue(fooChildCopy["this"].Get<RTTI*>()->Is("AttributedFooChild"));
			Assert::AreEqual(20U, fooChildCopy.AuxiliaryBegin());

			Assert::AreEqual(10, fooChild["fooChildAux1"].Get<std::int32_t>());
			fooChildCopy["fooChildAux1"] = 20;
			Assert::AreEqual(10, fooChild["fooChildAux1"].Get<std::int32_t>());
			Assert::AreEqual(20, fooChildCopy["fooChildAux1"].Get<std::int32_t>());

			Assert::IsTrue(&fooChild == fooChild["this"].Get<RTTI*>());
			Assert::IsTrue(&fooChildCopy == fooChildCopy["this"].Get<RTTI*>());
			Assert::IsTrue(&fooChild != fooChildCopy["this"].Get<RTTI*>());

			AttributedNative nativeFoo;
			Assert::AreEqual(4U, nativeFoo.AuxiliaryBegin());

			AttributedNative nativeFooCopy(nativeFoo);
			Assert::AreEqual(4U, nativeFooCopy.AuxiliaryBegin());
			Assert::IsNotNull(nativeFooCopy.Find("mNativeInt"));
			
			nativeFooCopy["mNativeInt"] = 20;
			Assert::AreEqual(10, nativeFoo["mNativeInt"].Get<std::int32_t>());
			Assert::AreEqual(20, nativeFooCopy["mNativeInt"].Get<std::int32_t>());
			Assert::IsFalse(nativeFoo["this"] == nativeFooCopy["this"].Get<RTTI*>());
		}

		TEST_METHOD(AttributedTestAssignmentOperator)
		{
			AttributedNative nativeFoo;
			nativeFoo["mNativeInt"] = 20;

			AttributedNative nativeFooCopy;
			nativeFooCopy = nativeFoo;

			Assert::IsNotNull(nativeFooCopy.Find("mNativeInt"));
			Assert::AreEqual(20, nativeFooCopy["mNativeInt"].Get<std::int32_t>());
			nativeFooCopy["mNativeInt"] = 30;
			Assert::AreEqual(20, nativeFoo["mNativeInt"].Get<std::int32_t>());
			Assert::AreEqual(30, nativeFooCopy["mNativeInt"].Get<std::int32_t>());


			AttributedFoo foo;
			Datum& fooAux1 = foo.AppendAuxiliaryAttribute("fooAux1");
			fooAux1 = 10;
			Datum& fooAux2 = foo.AppendAuxiliaryAttribute("fooAux2");
			fooAux2 = "i am an auxiliary attribute";

			AttributedFoo anotherFoo;
			anotherFoo = foo;

			Assert::IsNotNull(anotherFoo.Find("mInt"));
			Assert::IsFalse(foo["this"].Get<RTTI*>() == anotherFoo["this"].Get<RTTI*>());

			anotherFoo["mFloat"] = 20.02f;
			Assert::AreEqual(10.01f, foo["mFloat"].Get<std::float_t>());
			Assert::AreEqual(20.02f, anotherFoo["mFloat"].Get<std::float_t>());
		}

		TEST_METHOD(AttributedTestMoveCtor)
		{
			AttributedNative nativeFoo;

			AttributedNative nativeFooCopy(std::move(nativeFoo));
			Assert::AreEqual(4U, nativeFooCopy.AuxiliaryBegin());
			Assert::IsNotNull(nativeFooCopy.Find("mNativeInt"));

			// Test AttributedFoo and AttributedFooChild
			AttributedFoo foo;
			AttributedFoo fooMove(std::move(foo));
		}

		TEST_METHOD(AttributedTestMoveAssignmentOperator)
		{
			AttributedNative nativeFoo;
			nativeFoo["mNativeInt"] = 20;

			AttributedNative nativeFooCopy;
			nativeFooCopy = std::move(nativeFoo);

			Assert::IsNotNull(nativeFooCopy.Find("mNativeInt"));
			Assert::AreEqual(20, nativeFooCopy["mNativeInt"].Get<std::int32_t>());
			Assert::AreEqual(0U, nativeFoo.AuxiliaryBegin());

			AttributedFoo foo;
			AttributedFoo anotherFoo;
			anotherFoo = std::move(foo);
			Assert::AreEqual(0U, foo.AuxiliaryBegin());

		}

		TEST_METHOD(AttributedTestPopulate)
		{
			AttributedFoo foo;
			AttributedFooChild fooChild;

			Assert::IsNotNull(foo.Find("mScope"));
			Assert::IsNotNull(fooChild.Find("mScope"));
			Assert::IsNotNull(foo.Find("mInt"));
			Assert::IsNotNull(foo.Find("mExternalInt"));
			Assert::IsNotNull(fooChild.Find("mIntChild"));

			Assert::IsNull(foo.Find("mIntChild"));
			Assert::IsNull(fooChild.Find("random"));

			//auto expression = [&] {AttributedInternalError internalErrorFoo; };
			//Assert::ExpectException<std::exception>(expression);

			//AttributedExternalError* extErrorFoo;
			//auto expression1 = [&] {extErrorFoo = new AttributedExternalError(); };
			//Assert::ExpectException<std::exception>(expression1);
		}

		TEST_METHOD(AttributedTestIsAttribute)
		{
			AttributedFoo foo;
			AttributedFooChild fooChild;

			Datum& fooAux1 = foo.AppendAuxiliaryAttribute("fooAux1");
			fooAux1 = 10;
			Datum& fooAux2 = foo.AppendAuxiliaryAttribute("fooAux2");
			fooAux2 = "i am an auxiliary attribute";

			Datum& fooChildAux1 = fooChild.AppendAuxiliaryAttribute("fooChildAux1");
			fooChildAux1 = 10;
			Datum& fooChildAux2 = fooChild.AppendAuxiliaryAttribute("fooChildAux2");
			fooChildAux2 = "i am an auxiliary attribute";

			Assert::IsTrue(foo.IsAttribute("this"));
			Assert::IsTrue(foo.IsAttribute("fooAux2"));
			Assert::IsFalse(foo.IsAttribute("qwerty"));

			Assert::IsTrue(foo.IsPrescribedAttribute("mInt"));
			Assert::IsTrue(foo.IsPrescribedAttribute("mExternalInt"));
			Assert::IsFalse(foo.IsPrescribedAttribute("fooAux1"));

			Assert::IsTrue(foo.IsAuxiliaryAttribute("fooAux1"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("mInt"));


			Assert::IsTrue(fooChild.IsAttribute("this"));
			Assert::IsTrue(fooChild.IsAttribute("fooChildAux2"));
			Assert::IsFalse(fooChild.IsAttribute("qwerty"));

			Assert::IsTrue(fooChild.IsPrescribedAttribute("mInt"));
			Assert::IsTrue(fooChild.IsPrescribedAttribute("mExternalInt"));
			Assert::IsTrue(fooChild.IsPrescribedAttribute("mIntChild"));
			Assert::IsFalse(fooChild.IsPrescribedAttribute("fooChildAux1"));

			Assert::IsTrue(fooChild.IsAuxiliaryAttribute("fooChildAux1"));
			Assert::IsFalse(fooChild.IsAuxiliaryAttribute("mFloatChild"));

		}

		TEST_METHOD(AttributedTestAppendAuxiliary)
		{
			AttributedFoo foo;
			AttributedFooChild fooChild;

			Datum& fooAux1 = foo.AppendAuxiliaryAttribute("fooAux1");
			fooAux1 = 10;
			Datum& fooAux2 = foo.AppendAuxiliaryAttribute("fooAux2");
			fooAux2 = "i am an auxiliary attribute";

			Datum& fooChildAux1 = fooChild.AppendAuxiliaryAttribute("fooChildAux1");
			fooChildAux1 = 10;
			Datum& fooChildAux2 = fooChild.AppendAuxiliaryAttribute("fooChildAux2");
			fooChildAux2 = "i am an auxiliary attribute";

			Assert::IsTrue(foo.IsAuxiliaryAttribute("fooAux1"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("mInt"));

			Assert::IsTrue(fooChild.IsAuxiliaryAttribute("fooChildAux1"));
			Assert::IsFalse(fooChild.IsAuxiliaryAttribute("mFloatChild"));

			auto expression = [&] {foo.AppendAuxiliaryAttribute("this"); };
			Assert::ExpectException<std::exception>(expression);
			auto expression1 = [&] {foo.AppendAuxiliaryAttribute("mExternalInt"); };
			Assert::ExpectException<std::exception>(expression1);

			auto expression2 = [&] {fooChild.AppendAuxiliaryAttribute("mInt"); };
			Assert::ExpectException<std::exception>(expression1);
		}

		TEST_METHOD(AttributedTestAuxiliaryBegin)
		{
			AttributedFoo foo;
			Assert::AreEqual(14U, foo.AuxiliaryBegin());

			AttributedFooChild fooChild;
			Assert::AreEqual(20U, fooChild.AuxiliaryBegin());

			Datum& fooAux1 = foo.AppendAuxiliaryAttribute("fooAux1");
			fooAux1 = 10;
			Datum& fooAux2 = foo.AppendAuxiliaryAttribute("fooAux2");
			fooAux2 = "i am an auxiliary attribute";

			Datum& fooChildAux1 = fooChild.AppendAuxiliaryAttribute("fooChildAux1");
			fooChildAux1 = 10;
			Datum& fooChildAux2 = fooChild.AppendAuxiliaryAttribute("fooChildAux2");
			fooChildAux2 = "i am an auxiliary attribute";

			Assert::IsTrue(fooAux1 == foo[14]);
			Assert::IsTrue(fooChildAux2 == fooChild[21]);
		}

#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState sStartMemState;
#endif
	};

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState AttributedTest::sStartMemState;
#endif
}