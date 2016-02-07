#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;

namespace UnitTestLibraryDesktop
{

	TEST_CLASS(StackTest)
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
		TEST_METHOD(StackTestCtor)
		{
			{
				Stack<int> stackOfInts;
				Assert::IsTrue(stackOfInts.IsEmpty());
			}

			{
				Stack<int*> stackOfIntPtrs;
				Assert::IsTrue(stackOfIntPtrs.IsEmpty());
			}

			{
				Stack<Foo> stackOfFoos;
				Assert::IsTrue(stackOfFoos.IsEmpty());
			}
		}

		TEST_METHOD(StackTestCopyCtor)
		{
			{
				Stack<int> stackOfInts;
				stackOfInts.Push(10);
				
				Stack<int> anotherStackOfInts(stackOfInts);
				Assert::AreEqual(10, anotherStackOfInts.Top());
			}

			{
				int a = 10;
				Stack<int*> stackOfIntPtrs;
				stackOfIntPtrs.Push(&a);

				Stack<int*> anotherStackOfIntPtrs(stackOfIntPtrs);
				Assert::AreEqual(&a, anotherStackOfIntPtrs.Top());
			}

			{
				Foo a(10);
				Stack<Foo> stackOfFoos;
				stackOfFoos.Push(a);

				Stack<Foo> anotherStackOfFoos(stackOfFoos);
				Assert::IsTrue(a == anotherStackOfFoos.Top());
			}
		}

		TEST_METHOD(StackTestMoveCtor)
		{
			{
				Stack<int> stackOfInts;
				stackOfInts.Push(10);

				Stack<int> anotherStackOfInts(std::move(stackOfInts));
				Assert::AreEqual(10, anotherStackOfInts.Top());
				Assert::IsTrue(stackOfInts.IsEmpty());
			}

			{
				int a = 10;
				Stack<int*> stackOfIntPtrs;
				stackOfIntPtrs.Push(&a);

				Stack<int*> anotherStackOfIntPtrs(std::move(stackOfIntPtrs));
				Assert::AreEqual(&a, anotherStackOfIntPtrs.Top());
				Assert::IsTrue(stackOfIntPtrs.IsEmpty());
			}

			{
				Foo a(10);
				Stack<Foo> stackOfFoos;
				stackOfFoos.Push(a);

				Stack<Foo> anotherStackOfFoos(std::move(stackOfFoos));
				Assert::IsTrue(a == anotherStackOfFoos.Top());
				Assert::IsTrue(stackOfFoos.IsEmpty());
			}
		}

		TEST_METHOD(StackTestAssigOperator)
		{
			{
				Stack<int> stackOfInts;
				stackOfInts.Push(10);

				Stack<int> anotherStackOfInts;
				anotherStackOfInts.Push(20);
				anotherStackOfInts = stackOfInts;

				Assert::AreEqual(10, anotherStackOfInts.Top());
			}

			{
				int a = 10;
				int b = 20;
				Stack<int*> stackOfIntPtrs;
				stackOfIntPtrs.Push(&a);

				Stack<int*> anotherStackOfIntPtrs;
				anotherStackOfIntPtrs.Push(&b);
				anotherStackOfIntPtrs = stackOfIntPtrs;
				Assert::AreEqual(&a, anotherStackOfIntPtrs.Top());
			}

			{
				Foo a(10);
				Foo b(20);
				Stack<Foo> stackOfFoos;
				stackOfFoos.Push(a);

				Stack<Foo> anotherStackOfFoos;
				anotherStackOfFoos.Push(b);
				anotherStackOfFoos = stackOfFoos;
				Assert::IsTrue(a == anotherStackOfFoos.Top());
			}
		}
		
		TEST_METHOD(StackTestMoveAssignOperator)
		{
			{
				Stack<int> stackOfInts;
				stackOfInts.Push(10);

				Stack<int> anotherStackOfInts;
				anotherStackOfInts.Push(20);
				anotherStackOfInts = std::move(stackOfInts);

				Assert::AreEqual(10, anotherStackOfInts.Top());
				Assert::IsTrue(stackOfInts.IsEmpty());
			}

			{
				int a = 10;
				int b = 20;
				Stack<int*> stackOfIntPtrs;
				stackOfIntPtrs.Push(&a);

				Stack<int*> anotherStackOfIntPtrs;
				anotherStackOfIntPtrs.Push(&b);
				anotherStackOfIntPtrs = std::move(stackOfIntPtrs);
				
				Assert::AreEqual(&a, anotherStackOfIntPtrs.Top());
				Assert::IsTrue(stackOfIntPtrs.IsEmpty());
			}

			{
				Foo a(10);
				Foo b(20);
				Stack<Foo> stackOfFoos;
				stackOfFoos.Push(a);

				Stack<Foo> anotherStackOfFoos;
				anotherStackOfFoos.Push(b);
				anotherStackOfFoos = std::move(stackOfFoos);

				Assert::IsTrue(a == anotherStackOfFoos.Top());
				Assert::IsTrue(stackOfFoos.IsEmpty());
			}

		}

		TEST_METHOD(StackTestPush)
		{
			{
				Stack<int> stackOfInts;
				stackOfInts.Push(10);
				Assert::AreEqual(10, stackOfInts.Top());
				stackOfInts.Push(20);
				Assert::AreEqual(20, stackOfInts.Top());
			}

			{
				int a = 10;
				int b = 20;
				Stack<int*> stackOfIntPtrs;
				stackOfIntPtrs.Push(&a);
				Assert::AreEqual(&a, stackOfIntPtrs.Top());
				stackOfIntPtrs.Push(&b);
				Assert::AreEqual(&b, stackOfIntPtrs.Top());
			}

			{
				Foo a(10);
				Foo b(20);
				Stack<Foo> stackOfFoos;
				stackOfFoos.Push(a);
				Assert::IsTrue(a == stackOfFoos.Top());
				stackOfFoos.Push(b);
				Assert::IsTrue(b == stackOfFoos.Top());
			}

		}

		TEST_METHOD(StackTestPop)
		{
			{
				Stack<int> stackOfInts;
				stackOfInts.Push(10);
				stackOfInts.Push(20);
				stackOfInts.Pop();
				Assert::AreEqual(10, stackOfInts.Top());
				Assert::AreEqual(1U, stackOfInts.Size());
			}

			{
				int a = 10;
				int b = 20;
				Stack<int*> stackOfIntPtrs;
				stackOfIntPtrs.Push(&a);
				stackOfIntPtrs.Push(&b);
				stackOfIntPtrs.Pop();
				Assert::AreEqual(&a, stackOfIntPtrs.Top());
				Assert::AreEqual(1U, stackOfIntPtrs.Size());
			}

			{
				Foo a(10);
				Foo b(20);
				Stack<Foo> stackOfFoos;
				stackOfFoos.Push(a);
				stackOfFoos.Push(b);
				stackOfFoos.Pop();
				Assert::IsTrue(a == stackOfFoos.Top());
				Assert::AreEqual(1U, stackOfFoos.Size());
			}
		}

		TEST_METHOD(StackTestTop)
		{
			{
				Stack<int> stackOfInts;
				stackOfInts.Push(10);
				Assert::AreEqual(10, stackOfInts.Top());
				stackOfInts.Push(20);
				Assert::AreEqual(20, stackOfInts.Top());

				const Stack<int> constStackOfInts = stackOfInts;
				Assert::AreEqual(20, constStackOfInts.Top());

			}

			{
				int a = 10;
				int b = 20;
				Stack<int*> stackOfIntPtrs;
				stackOfIntPtrs.Push(&a);
				Assert::AreEqual(&a, stackOfIntPtrs.Top());
				stackOfIntPtrs.Push(&b);
				Assert::AreEqual(&b, stackOfIntPtrs.Top());

				const Stack<int*> constStackOfIntPtrs = stackOfIntPtrs;
				Assert::AreEqual(&b, constStackOfIntPtrs.Top());
			}

			{
				Foo a(10);
				Foo b(20);
				Stack<Foo> stackOfFoos;
				stackOfFoos.Push(a);
				Assert::IsTrue(a == stackOfFoos.Top());
				stackOfFoos.Push(b);
				Assert::IsTrue(b == stackOfFoos.Top());

				const Stack<Foo> constStackOfFoos = stackOfFoos;
				Assert::IsTrue(b == constStackOfFoos.Top());
			}
		}

		TEST_METHOD(StackTestSize)
		{
			{
				Stack<int> stackOfInts;
				Assert::AreEqual(0U, stackOfInts.Size());
				stackOfInts.Push(10);
				Assert::AreEqual(1U, stackOfInts.Size());
				stackOfInts.Push(20);
				Assert::AreEqual(2U, stackOfInts.Size());
			}

			{
				int a = 10;
				int b = 20;
				Stack<int*> stackOfIntPtrs;
				Assert::AreEqual(0U, stackOfIntPtrs.Size());
				stackOfIntPtrs.Push(&a);
				Assert::AreEqual(1U, stackOfIntPtrs.Size());
				stackOfIntPtrs.Push(&b);
				Assert::AreEqual(2U, stackOfIntPtrs.Size());
			}

			{
				Foo a(10);
				Foo b(20);
				Stack<Foo> stackOfFoos;
				Assert::AreEqual(0U, stackOfFoos.Size());
				stackOfFoos.Push(a);
				Assert::AreEqual(1U, stackOfFoos.Size());
				stackOfFoos.Push(b);
				Assert::AreEqual(2U, stackOfFoos.Size());
			}
		}

		TEST_METHOD(StackTestIsEmpty)
		{
			{
				Stack<int> stackOfInts;
				Assert::IsTrue(stackOfInts.IsEmpty());
				stackOfInts.Push(10);
				Assert::IsFalse(stackOfInts.IsEmpty());
			}

			{
				int a = 10;
				Stack<int*> stackOfIntPtrs;
				Assert::IsTrue(stackOfIntPtrs.IsEmpty());
				stackOfIntPtrs.Push(&a);
				Assert::IsFalse(stackOfIntPtrs.IsEmpty());
			}

			{
				Foo a(10);
				Stack<Foo> stackOfFoos;
				Assert::IsTrue(stackOfFoos.IsEmpty());
				stackOfFoos.Push(a);
				Assert::IsFalse(stackOfFoos.IsEmpty());
			}
		}

		TEST_METHOD(StackTestClear)
		{
			{
				Stack<int> stackOfInts;
				stackOfInts.Push(10);
				stackOfInts.Clear();
				Assert::IsTrue(stackOfInts.IsEmpty());
			}

			{
				int a = 10;
				Stack<int*> stackOfIntPtrs;
				stackOfIntPtrs.Push(&a);
				stackOfIntPtrs.Clear();
				Assert::IsTrue(stackOfIntPtrs.IsEmpty());
			}

			{
				Foo a(10);
				Stack<Foo> stackOfFoos;
				stackOfFoos.Push(a);
				stackOfFoos.Clear();
				Assert::IsTrue(stackOfFoos.IsEmpty());
			}
		}
#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState sStartMemState;
#endif
	};

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState StackTest::sStartMemState;
#endif
}