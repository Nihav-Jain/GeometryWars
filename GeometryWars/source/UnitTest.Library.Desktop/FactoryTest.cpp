#include "pch.h"
#include "CppUnitTest.h"
#include "Factory.h"
#include "RTTI.h"
#include "FooRTTI.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;

namespace UnitTestLibraryDesktop
{

	CONCRETE_FACTORY(FooRTTI, RTTI);

	TEST_CLASS(FactoryTest)
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

		TEST_METHOD(FactoryTestRTTI)
		{
			const Factory<RTTI>* const fact = Factory<RTTI>::Find("FooRTTI");
			Assert::IsNull(fact);

			RTTI* nullFooRtti = Factory<RTTI>::Create("FooRTTI");
			Assert::IsNull(nullFooRtti);

			{
				FooRTTIFactory fooFactory;
				const Factory<RTTI>* const fact2 = Factory<RTTI>::Find("FooRTTI");
				Assert::IsNotNull(fact2);
				RTTI* rtti = Factory<RTTI>::Create("FooRTTI");
				Assert::IsTrue(rtti->Is("FooRTTI"));
				FooRTTI* fooRtti = rtti->As<FooRTTI>();

				// begin, end, 
				Factory<RTTI>::Iterator itr = Factory<RTTI>::begin();
				Assert::IsTrue((*itr).first == "FooRTTI");
				++itr;
				Assert::IsTrue(itr == Factory<RTTI>::end());

				delete fooRtti;
			}

			const Factory<RTTI>* const fact3 = Factory<RTTI>::Find("FooRTTI");
			Assert::IsNull(fact3);
		}

#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState sStartMemState;
#endif
	};

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState FactoryTest::sStartMemState;
#endif
}