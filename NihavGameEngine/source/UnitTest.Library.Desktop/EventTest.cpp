#include "pch.h"
#include "CppUnitTest.h"

#include "IEventSubscriber.h"
#include "Event.h"
#include "EventQueue.h"

#include "Foo.h"
#include "FooSubscriber.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;

namespace UnitTestLibraryDesktop
{

	TEST_CLASS(EventTest)
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

		TEST_METHOD(EventTestPublisher)
		{
			FooSubscriber subscriber;
			Foo foo(20);
			
			Event<Foo> fooEvent(foo);
			fooEvent.Deliver();

			Assert::AreEqual(0, subscriber.data);
			Assert::IsFalse(subscriber.wasNotified);

			fooEvent.Subscribe(subscriber);
			fooEvent.Deliver();

			Assert::IsTrue(subscriber.wasNotified);
			Assert::AreEqual(20, subscriber.data);

			subscriber.wasNotified = false;
			subscriber.data = 0;

			fooEvent.Unsubscribe(subscriber);
			fooEvent.Deliver();

			Assert::AreEqual(0, subscriber.data);
			Assert::IsFalse(subscriber.wasNotified);


			fooEvent.Subscribe(subscriber);
			fooEvent.Deliver();

			Assert::IsTrue(subscriber.wasNotified);
			Assert::AreEqual(20, subscriber.data);

			subscriber.wasNotified = false;
			subscriber.data = 0;

			fooEvent.UnsubscriberAll();
			fooEvent.Deliver();

			Assert::AreEqual(0, subscriber.data);
			Assert::IsFalse(subscriber.wasNotified);

		}


#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState sStartMemState;
#endif
	};

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState EventTest::sStartMemState;
#endif
}