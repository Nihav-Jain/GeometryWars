#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(SListTest)
	{
	public:

		/**
		 * Initialize the lists before starting the test
		 */
		TEST_METHOD_INITIALIZE(SListTestInit)
		{
#if defined(DEBUG) | defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
#endif
			listOfIntegers = new SList<int>();
			listOfIntegerPtrs = new SList<int*>();
			listOfFoos = new SList<Foo>();
		}

		/**
		 * delete the lists after the test ends
		 */
		TEST_METHOD_CLEANUP(SListTestCleanup)
		{
			delete listOfIntegers;
			delete listOfIntegerPtrs;
			delete listOfFoos;

#if defined(DEBUG) | defined(_DEBUG)
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

		/********************************* Tests for list of integers ************************************/

		/**
		 * Test the constructor for the list of integers
		 */
		TEST_METHOD(SListTestCtor)
		{
			// the constructor of SList must initialize an empty list
			Assert::IsTrue(listOfIntegers->IsEmpty());
		}

		/**
		 * Test function PushFront() for the list of integers
		 */
		TEST_METHOD(SListTestPushFront)
		{
			// for the first element, the front and back must be the same
			listOfIntegers->PushFront(10);
			Assert::AreEqual(10, listOfIntegers->Front());
			Assert::AreEqual(10, listOfIntegers->Back());

			// size must get updated on performing PushFront
			Assert::AreEqual(1U, listOfIntegers->Size());

			// the front of the list must get updated on another PushFront operation but the back must remain the same
			listOfIntegers->PushFront(20);
			Assert::AreEqual(20, listOfIntegers->Front());
			Assert::AreEqual(10, listOfIntegers->Back());
		}

		/**
		 * Test function PopFront() for the list of integers
		 */
		TEST_METHOD(SListTestPopFront)
		{
			// when the list is empty, an exception must be thrown
			try
			{
				listOfIntegers->PopFront();
				Assert::Fail();
			}
			catch (const std::exception& e)
			{
				UNREFERENCED_PARAMETER(e);
			}

			// push 1 elemennt and pop it back, results in an empty list
			listOfIntegers->PushFront(10);
			listOfIntegers->PopFront();
			Assert::IsTrue(listOfIntegers->IsEmpty());

			// Back should be reset to nullptr if PopFront causes list to become empty
			// Calling Back() on an empty list raises an exception
			try
			{
				int back = listOfIntegers->Back();
				UNREFERENCED_PARAMETER(back);
				Assert::Fail();
			}
			catch (const std::exception& e)
			{
				UNREFERENCED_PARAMETER(e);
			}

			// pop front causes change in the Front() element
			listOfIntegers->PushFront(10);
			listOfIntegers->PushFront(20);
			listOfIntegers->PopFront();
			Assert::AreEqual(10, listOfIntegers->Front());
		}

		/**
		 * Test function PushBack() for the list of integers
		 */
		TEST_METHOD(SListTestPushBack)
		{
			// for the first element, the front and back must be the same
			listOfIntegers->PushBack(10);
			Assert::AreEqual(10, listOfIntegers->Back());
			Assert::AreEqual(10, listOfIntegers->Front());

			// size must get updated on performing PushFront
			Assert::AreEqual(1U, listOfIntegers->Size());

			listOfIntegers->PushBack(20);
			Assert::AreEqual(20, listOfIntegers->Back());
			Assert::AreEqual(10, listOfIntegers->Front());
		}

		/**
		 * Test function Size() for the list of integers
		 */
		TEST_METHOD(SListTestSize)
		{
			listOfIntegers->PushBack(10);
			listOfIntegers->PushBack(20);
			listOfIntegers->PushBack(30);
			Assert::AreEqual(3U, listOfIntegers->Size());
		}

		/**
 		 * Test function Clear() for the list of integers
		 */
		TEST_METHOD(SListTestClear)
		{
			listOfIntegers->PushBack(10);
			listOfIntegers->PushBack(20);
			listOfIntegers->PushBack(30);
			listOfIntegers->Clear();
			Assert::IsTrue(listOfIntegers->IsEmpty());
		}

		/**
		 * Test the Copy Constructor for the list of integers
		 */
		TEST_METHOD(SListTestCopyCtor)
		{
			listOfIntegers->PushBack(1);
			listOfIntegers->PushBack(2);
			listOfIntegers->PushBack(3);
			listOfIntegers->PushBack(4);

			SList<int> copyList(*listOfIntegers);
			Assert::AreEqual(4U, copyList.Size());
			Assert::AreEqual(1, copyList.Front());
			copyList.PopFront();
			Assert::AreEqual(2, copyList.Front());
			copyList.PopFront();
			Assert::AreEqual(3, copyList.Front());
			copyList.PopFront();
			Assert::AreEqual(4, copyList.Front());
		}

		/**
		 * Test the Assignment Operator= for the list of integers
		 */
		TEST_METHOD(SListTestAssignOperator)
		{
			SList<int> copyList;
			copyList.PushBack(10);
			copyList.PushBack(20);
			copyList.PushBack(30);

			*listOfIntegers = copyList;
			Assert::AreEqual(3U, listOfIntegers->Size());
			Assert::AreEqual(10, listOfIntegers->Front());
			listOfIntegers->PopFront();
			Assert::AreEqual(20, listOfIntegers->Front());
			listOfIntegers->PopFront();
			Assert::AreEqual(30, listOfIntegers->Front());
		}

		/**
		 * Test function Front() for the list of integers
		 */
		TEST_METHOD(SListTestFront)
		{
			try
			{
				int front = listOfIntegers->Front();
				UNREFERENCED_PARAMETER(front);
				Assert::Fail();
			}
			catch (const std::exception& e)
			{
				UNREFERENCED_PARAMETER(e);
			}

			listOfIntegers->PushFront(10);
			listOfIntegers->PushFront(20);
			Assert::AreEqual(20, listOfIntegers->Front());

			listOfIntegers->PushBack(30);
			Assert::AreEqual(20, listOfIntegers->Front());
		}

		/**
		* Test function Back() for the list of integers
		*/
		TEST_METHOD(SListTestBack)
		{
			try
			{
				int back = listOfIntegers->Back();
				UNREFERENCED_PARAMETER(back);
				Assert::Fail();
			}
			catch (const std::exception& e)
			{
				UNREFERENCED_PARAMETER(e);
			}

			listOfIntegers->PushFront(10);
			listOfIntegers->PushFront(20);
			Assert::AreEqual(10, listOfIntegers->Back());

			listOfIntegers->PushBack(30);
			Assert::AreEqual(30, listOfIntegers->Back());
		}

		TEST_METHOD(SListIntTestBegin)
		{
			listOfIntegers->PushBack(10);

			SList<int>::Iterator itr = listOfIntegers->begin();
			Assert::AreEqual(10, *itr);
		}

		TEST_METHOD(SListIntTestEnd)
		{
			listOfIntegers->PushBack(10);

			SList<int>::Iterator itr = listOfIntegers->end();
			auto expression = [&] { int a = *itr; UNREFERENCED_PARAMETER(a);  };
			Assert::ExpectException<std::exception>(expression);
		}

		TEST_METHOD(SListIntTestInsertAfter)
		{
			SList<int> anotherIntList;

			listOfIntegers->PushBack(10);
			listOfIntegers->PushBack(20);

			{
				// sending iterator belonging to another list 
				SList<int>::Iterator itr = anotherIntList.begin();
				auto expression = [&] { listOfIntegers->InsertAfter(10, itr); };
				Assert::ExpectException<std::exception>(expression);
			}

			{
				// InsertAfter on an empty list
				SList<int>::Iterator itr = anotherIntList.begin();
				anotherIntList.InsertAfter(10, itr);
				Assert::AreEqual(10, anotherIntList.Front());
			}

			{
				// InsertAfter on iterator pointing to end of list
				SList<int>::Iterator itr = listOfIntegers->end();
				listOfIntegers->InsertAfter(25, itr);
				Assert::AreEqual(25, listOfIntegers->Back());
			}

			SList<int>::Iterator itr = listOfIntegers->begin();
			listOfIntegers->InsertAfter(15, itr);
			
			int value = 10;
			for (; itr != listOfIntegers->end(); ++itr)
			{
				Assert::AreEqual(value, *itr);
				value += 5;
			}

		}

		TEST_METHOD(SListIntTestFind)
		{
			listOfIntegers->PushBack(10);
			listOfIntegers->PushBack(20);
			listOfIntegers->PushBack(30);

			SList<int>::Iterator itr = listOfIntegers->Find(20);
			Assert::AreEqual(20, *itr);

			itr = listOfIntegers->Find(40);
			Assert::IsTrue(itr == listOfIntegers->end());
		}

		TEST_METHOD(SListIntTestRemove)
		{
			// empty list
			listOfIntegers->Remove(10);
			Assert::IsTrue(listOfIntegers->IsEmpty());

			// last element
			listOfIntegers->PushBack(10);
			listOfIntegers->Remove(10);
			Assert::IsTrue(listOfIntegers->IsEmpty());

			// remove middle element
			listOfIntegers->PushBack(10);
			listOfIntegers->PushBack(20);
			listOfIntegers->PushBack(30);
			listOfIntegers->Remove(20);

			Assert::AreEqual(2U, listOfIntegers->Size());
			Assert::AreEqual(10, listOfIntegers->Front());
			Assert::AreEqual(30, listOfIntegers->Back());

			// remove last element
			listOfIntegers->Remove(30);
			Assert::AreEqual(10, listOfIntegers->Back());
		}

		TEST_METHOD(SListTestMoveCtor)
		{
			{
				listOfIntegers->PushBack(10);
				listOfIntegers->PushBack(10);
				listOfIntegers->PushBack(10);

				SList<int> list = std::move(*listOfIntegers);
				Assert::IsTrue(listOfIntegers->IsEmpty());
				Assert::AreEqual(3U, list.Size());
			}

			{
				int a = 10;
				listOfIntegerPtrs->PushBack(&a);
				listOfIntegerPtrs->PushBack(&a);
				listOfIntegerPtrs->PushBack(&a);

				SList<int*> list = std::move(*listOfIntegerPtrs);
				Assert::IsTrue(listOfIntegerPtrs->IsEmpty());
				Assert::AreEqual(3U, list.Size());
			}

			{
				Foo a(10);
				listOfFoos->PushBack(a);
				listOfFoos->PushBack(a);
				listOfFoos->PushBack(a);

				SList<Foo> list = std::move(*listOfFoos);
				Assert::IsTrue(listOfFoos->IsEmpty());
				Assert::AreEqual(3U, list.Size());
			}

		}

		TEST_METHOD(SListTestMoveAssignOperator)
		{
			{
				listOfIntegers->PushBack(10);
				listOfIntegers->PushBack(10);
				listOfIntegers->PushBack(10);

				SList<int> list;
				list.PushBack(20);

				list = std::move(*listOfIntegers);
				Assert::IsTrue(listOfIntegers->IsEmpty());
				Assert::AreEqual(3U, list.Size());
			}

			{
				int a = 10;
				int b = 20;
				listOfIntegerPtrs->PushBack(&a);
				listOfIntegerPtrs->PushBack(&a);
				listOfIntegerPtrs->PushBack(&a);

				SList<int*> list;
				list.PushBack(&b);
				
				list = std::move(*listOfIntegerPtrs);
				Assert::IsTrue(listOfIntegerPtrs->IsEmpty());
				Assert::AreEqual(3U, list.Size());
			}

			{
				Foo a(10);
				Foo b(20);
				listOfFoos->PushBack(a);
				listOfFoos->PushBack(a);
				listOfFoos->PushBack(a);

				SList<Foo> list;
				list.PushBack(b);
				
				list = std::move(*listOfFoos);
				Assert::IsTrue(listOfFoos->IsEmpty());
				Assert::AreEqual(3U, list.Size());
			}
		}

		/********************************* Tests for Iterators ************************************/

		TEST_METHOD(SListTestIteratorCtor)
		{
			{
				SList<int>::Iterator itr;

				// not attached to a host
				auto expression = [&] { int a = *itr; UNREFERENCED_PARAMETER(a);  };
				Assert::ExpectException<std::exception>(expression);

				// current node set to nullptr
				itr = listOfIntegers->begin();

				auto expression2 = [&] { int a = *itr; UNREFERENCED_PARAMETER(a);  };
				Assert::ExpectException<std::exception>(expression2);
			}

			{
				SList<int*>::Iterator itr;

				// not attached to a host
				auto expression = [&] { int* a = *itr; UNREFERENCED_PARAMETER(a);  };
				Assert::ExpectException<std::exception>(expression);

				// current node set to nullptr
				itr = listOfIntegerPtrs->begin();

				auto expression2 = [&] { int* a = *itr; UNREFERENCED_PARAMETER(a);  };
				Assert::ExpectException<std::exception>(expression2);
			}

			{
				SList<Foo>::Iterator itr;

				// not attached to a host
				auto expression = [&] { Foo a = *itr; UNREFERENCED_PARAMETER(a);  };
				Assert::ExpectException<std::exception>(expression);

				// current node set to nullptr
				itr = listOfFoos->begin();

				auto expression2 = [&] { Foo a = *itr; UNREFERENCED_PARAMETER(a);  };
				Assert::ExpectException<std::exception>(expression2);
			}
		}

		TEST_METHOD(SListTestIteratorCopyCtor)
		{
			{
				listOfIntegers->PushBack(10);
				SList<int>::Iterator itr = listOfIntegers->begin();
				SList<int>::Iterator itr2(itr);

				Assert::IsTrue(itr == itr2);
			}

			{
				int a = 10;
				listOfIntegerPtrs->PushBack(&a);
				SList<int*>::Iterator itr = listOfIntegerPtrs->begin();
				SList<int*>::Iterator itr2(itr);

				Assert::IsTrue(itr == itr2);
			}

			{
				Foo a(10);
				listOfFoos->PushBack(a);
				SList<Foo>::Iterator itr = listOfFoos->begin();
				SList<Foo>::Iterator itr2(itr);

				Assert::IsTrue(itr == itr2);
			}

		}

		TEST_METHOD(SListTestIteratorAssignOperator)
		{
			{
				listOfIntegers->PushBack(10);
				SList<int>::Iterator itr;
				itr = listOfIntegers->begin();

				Assert::IsTrue(itr == listOfIntegers->begin());
			}

			{
				int a = 10;
				listOfIntegerPtrs->PushBack(&a);
				SList<int*>::Iterator itr;
				itr = listOfIntegerPtrs->begin();

				Assert::IsTrue(itr == listOfIntegerPtrs->begin());
			}

			{
				Foo a(10);
				listOfFoos->PushBack(a);
				SList<Foo>::Iterator itr;
				itr = listOfFoos->begin();

				Assert::IsTrue(itr == listOfFoos->begin());
			}
		}

		TEST_METHOD(SListTestIteratorEqualityOperator)
		{
			{
				listOfIntegers->PushBack(10);
				listOfIntegers->PushBack(20);

				SList<int>::Iterator itr;
				SList<int>::Iterator itr2;

				Assert::IsTrue(itr == itr2);

				itr = listOfIntegers->begin();
				Assert::IsFalse(itr == itr2);

				itr2 = listOfIntegers->begin();
				++itr2;
				Assert::IsFalse(itr == itr2);

				itr2 = listOfIntegers->begin();
				Assert::IsTrue(itr == itr2);
				Assert::IsFalse(itr != itr2);
			}

			{
				int a = 10;
				int b = 20;
				listOfIntegerPtrs->PushBack(&a);
				listOfIntegerPtrs->PushBack(&b);

				SList<int*>::Iterator itr;
				SList<int*>::Iterator itr2;

				Assert::IsTrue(itr == itr2);

				itr = listOfIntegerPtrs->begin();
				Assert::IsFalse(itr == itr2);

				itr2 = listOfIntegerPtrs->begin();
				++itr2;
				Assert::IsFalse(itr == itr2);

				itr2 = listOfIntegerPtrs->begin();
				Assert::IsTrue(itr == itr2);
				Assert::IsFalse(itr != itr2);
			}

			{
				Foo a(10);
				Foo b(20);
				listOfFoos->PushBack(a);
				listOfFoos->PushBack(b);

				SList<Foo>::Iterator itr;
				SList<Foo>::Iterator itr2;

				Assert::IsTrue(itr == itr2);

				itr = listOfFoos->begin();
				Assert::IsFalse(itr == itr2);

				itr2 = listOfFoos->begin();
				++itr2;
				Assert::IsFalse(itr == itr2);

				itr2 = listOfFoos->begin();
				Assert::IsTrue(itr == itr2);
				Assert::IsFalse(itr != itr2);

			}

		}

		TEST_METHOD(SListTestIteratorPrefixIncrement)
		{
			{
				SList<int>::Iterator itr;

				{
					auto expression = [&] {int a = *itr; UNREFERENCED_PARAMETER(a); };
					Assert::ExpectException<std::exception>(expression);
				}

				itr = listOfIntegers->end();

				{
					auto expression = [&] {int a = *itr; UNREFERENCED_PARAMETER(a); };
					Assert::ExpectException<std::exception>(expression);
				}

				listOfIntegers->PushBack(10);
				listOfIntegers->PushBack(20);

				itr = listOfIntegers->begin();
				++itr;
				Assert::AreEqual(20, *itr);

				++itr;
				{
					auto expression = [&] { ++itr; };
					Assert::ExpectException<std::exception>(expression);
				}
			}

			{
				SList<int*>::Iterator itr;

				{
					auto expression = [&] {int* a = *itr; UNREFERENCED_PARAMETER(a); };
					Assert::ExpectException<std::exception>(expression);
				}

				itr = listOfIntegerPtrs->end();

				{
					auto expression = [&] {int* a = *itr; UNREFERENCED_PARAMETER(a); };
					Assert::ExpectException<std::exception>(expression);
				}

				int a = 10;
				int b = 20;
				listOfIntegerPtrs->PushBack(&a);
				listOfIntegerPtrs->PushBack(&b);

				itr = listOfIntegerPtrs->begin();
				++itr;
				Assert::AreEqual(&b, *itr);

				++itr;
				{
					auto expression = [&] { ++itr; };
					Assert::ExpectException<std::exception>(expression);
				}
			}

			{
				SList<Foo>::Iterator itr;

				{
					auto expression = [&] {Foo a = *itr; UNREFERENCED_PARAMETER(a); };
					Assert::ExpectException<std::exception>(expression);
				}

				itr = listOfFoos->end();

				{
					auto expression = [&] {Foo a = *itr; UNREFERENCED_PARAMETER(a); };
					Assert::ExpectException<std::exception>(expression);
				}

				Foo a(10);
				Foo b(20);
				listOfFoos->PushBack(a);
				listOfFoos->PushBack(b);

				itr = listOfFoos->begin();
				++itr;
				Assert::IsTrue(b == *itr);

				++itr;
				{
					auto expression = [&] { ++itr; };
					Assert::ExpectException<std::exception>(expression);
				}
			}
		}

		TEST_METHOD(SListTestIteratorPostfixIncrement)
		{
			{
				SList<int>::Iterator itr;

				{
					auto expression = [&] {int a = *itr; UNREFERENCED_PARAMETER(a); };
					Assert::ExpectException<std::exception>(expression);
				}

				itr = listOfIntegers->end();

				{
					auto expression = [&] {int a = *itr; UNREFERENCED_PARAMETER(a); };
					Assert::ExpectException<std::exception>(expression);
				}

				listOfIntegers->PushBack(10);
				listOfIntegers->PushBack(20);

				itr = listOfIntegers->begin();
				int a = *(itr++);
				Assert::AreEqual(10, a);
				Assert::AreEqual(20, *itr);

				itr++;
				{
					auto expression = [&] { itr++; };
					Assert::ExpectException<std::exception>(expression);
				}
			}

			{
				SList<int*>::Iterator itr;

				{
					auto expression = [&] {int* a = *itr; UNREFERENCED_PARAMETER(a); };
					Assert::ExpectException<std::exception>(expression);
				}

				itr = listOfIntegerPtrs->end();

				{
					auto expression = [&] {int* a = *itr; UNREFERENCED_PARAMETER(a); };
					Assert::ExpectException<std::exception>(expression);
				}

				int a = 10;
				int b = 20;
				listOfIntegerPtrs->PushBack(&a);
				listOfIntegerPtrs->PushBack(&b);

				itr = listOfIntegerPtrs->begin();
				int* c = *(itr++);
				Assert::AreEqual(&a, c);
				Assert::AreEqual(&b, *itr);

				itr++;
				{
					auto expression = [&] { itr++; };
					Assert::ExpectException<std::exception>(expression);
				}
			}

			{
				SList<Foo>::Iterator itr;

				{
					auto expression = [&] {Foo a = *itr; UNREFERENCED_PARAMETER(a); };
					Assert::ExpectException<std::exception>(expression);
				}

				itr = listOfFoos->end();

				{
					auto expression = [&] {Foo a = *itr; UNREFERENCED_PARAMETER(a); };
					Assert::ExpectException<std::exception>(expression);
				}

				Foo a(10);
				Foo b(20);
				listOfFoos->PushBack(a);
				listOfFoos->PushBack(b);

				itr = listOfFoos->begin();
				Foo c = *(itr++);
				Assert::IsTrue(a == c);
				Assert::IsTrue(b == *itr);

				itr++;
				{
					auto expression = [&] { itr++; };
					Assert::ExpectException<std::exception>(expression);
				}
			}

		}

		/********************************* Tests for list of pointers to integers ************************************/

		TEST_METHOD(SListIntegerPointerTestCtor)
		{
			Assert::IsTrue(listOfIntegerPtrs->IsEmpty());
		}

		TEST_METHOD(SListIntegerPointerTestPushFront)
		{
			// test for list of pointers to integers
			int a = 10;
			listOfIntegerPtrs->PushFront(&a);
			Assert::AreEqual(&a, listOfIntegerPtrs->Front());
			Assert::AreEqual(&a, listOfIntegerPtrs->Back());
			Assert::AreEqual(1U, listOfIntegerPtrs->Size());

			int b = 20;
			listOfIntegerPtrs->PushFront(&b);
			Assert::AreEqual(&b, listOfIntegerPtrs->Front());
			Assert::AreEqual(&a, listOfIntegerPtrs->Back());
		}

		TEST_METHOD(SListIntegerPointerTestPopFront)
		{
			// test for list of pointers to integers
			try
			{
				listOfIntegerPtrs->PopFront();
				Assert::Fail();
			}
			catch (const std::exception& e)
			{
				UNREFERENCED_PARAMETER(e);
			}

			int a = 10;
			listOfIntegerPtrs->PushFront(&a);
			listOfIntegerPtrs->PopFront();
			Assert::IsTrue(listOfIntegerPtrs->IsEmpty());
			try
			{
				int* back = listOfIntegerPtrs->Back();
				UNREFERENCED_PARAMETER(back);
				Assert::Fail();
			}
			catch (const std::exception& e)
			{
				UNREFERENCED_PARAMETER(e);
			}

			int b = 20;
			listOfIntegerPtrs->PushFront(&a);
			listOfIntegerPtrs->PushFront(&b);
			listOfIntegerPtrs->PopFront();
			Assert::AreEqual(&a, listOfIntegerPtrs->Front());
		}

		TEST_METHOD(SListIntegerPointerTestPushBack)
		{
			// test for list of pointers to integers
			int a = 10;
			int b = 20;
			int c = 30;

			listOfIntegerPtrs->PushBack(&a);
			Assert::AreEqual(&a, listOfIntegerPtrs->Back());
			Assert::AreEqual(&a, listOfIntegerPtrs->Front());
			Assert::AreEqual(1U, listOfIntegerPtrs->Size());

			listOfIntegerPtrs->PushBack(&b);
			Assert::AreEqual(&b, listOfIntegerPtrs->Back());
			Assert::AreEqual(&a, listOfIntegerPtrs->Front());

			listOfIntegerPtrs->PushFront(&c);
			Assert::AreEqual(&b, listOfIntegerPtrs->Back());
		}

		TEST_METHOD(SListIntegerPointerTestSize)
		{
			int a = 10;
			int b = 20;
			int c = 30;
			listOfIntegerPtrs->PushBack(&a);
			listOfIntegerPtrs->PushBack(&b);
			listOfIntegerPtrs->PushBack(&c);
			Assert::AreEqual(3U, listOfIntegerPtrs->Size());
		}

		TEST_METHOD(SListIntegerPointerTestClear)
		{
			int a = 10;
			int b = 20;
			int c = 30;
			listOfIntegerPtrs->PushBack(&a);
			listOfIntegerPtrs->PushBack(&b);
			listOfIntegerPtrs->PushBack(&c);
			listOfIntegerPtrs->Clear();
			Assert::IsTrue(listOfIntegerPtrs->IsEmpty());
		}

		TEST_METHOD(SListIntegerPointerTestCopyCtor)
		{
			int a = 1;
			int b = 2;
			int c = 3;
			int d = 4;
			listOfIntegerPtrs->PushBack(&a);
			listOfIntegerPtrs->PushBack(&b);
			listOfIntegerPtrs->PushBack(&c);
			listOfIntegerPtrs->PushBack(&d);

			SList<int*> copyList(*listOfIntegerPtrs);
			Assert::AreEqual(4U, copyList.Size());
			Assert::AreEqual(&a, copyList.Front());
			copyList.PopFront();
			Assert::AreEqual(&b, copyList.Front());
			copyList.PopFront();
			Assert::AreEqual(&c, copyList.Front());
			copyList.PopFront();
			Assert::AreEqual(&d, copyList.Front());
		}

		TEST_METHOD(SListIntegerPointerTestAssignOperator)
		{
			SList<int*> copyList;
			int a = 10;
			int b = 20;
			int c = 30;
			copyList.PushBack(&a);
			copyList.PushBack(&b);
			copyList.PushBack(&c);

			*listOfIntegerPtrs = copyList;
			Assert::AreEqual(3U, listOfIntegerPtrs->Size());
			Assert::AreEqual(&a, listOfIntegerPtrs->Front());
			listOfIntegerPtrs->PopFront();
			Assert::AreEqual(&b, listOfIntegerPtrs->Front());
			listOfIntegerPtrs->PopFront();
			Assert::AreEqual(&c, listOfIntegerPtrs->Front());

		}

		/**
		* Test function Front() for the list of poitners to integers
		*/
		TEST_METHOD(SListIntegerPointerTestFront)
		{
			try
			{
				int* front = listOfIntegerPtrs->Front();
				UNREFERENCED_PARAMETER(front);
				Assert::Fail();
			}
			catch (const std::exception& e)
			{
				UNREFERENCED_PARAMETER(e);
			}

			int a = 10;
			int b = 20;
			int c = 30;
			listOfIntegerPtrs->PushFront(&a);
			listOfIntegerPtrs->PushFront(&b);
			Assert::AreEqual(&b, listOfIntegerPtrs->Front());

			listOfIntegerPtrs->PushBack(&c);
			Assert::AreEqual(&b, listOfIntegerPtrs->Front());
		}

		/**
		* Test function Back() for the list of pointers to integers
		*/
		TEST_METHOD(SListIntegerPointerTestBack)
		{
			try
			{
				int* back = listOfIntegerPtrs->Back();
				UNREFERENCED_PARAMETER(back);
				Assert::Fail();
			}
			catch (const std::exception& e)
			{
				UNREFERENCED_PARAMETER(e);
			}

			int a = 10;
			int b = 20;
			int c = 30; 
			listOfIntegerPtrs->PushFront(&a);
			listOfIntegerPtrs->PushFront(&b);
			Assert::AreEqual(&a, listOfIntegerPtrs->Back());

			listOfIntegerPtrs->PushBack(&c);
			Assert::AreEqual(&c, listOfIntegerPtrs->Back());
		}


		TEST_METHOD(SListIntegerPointerTestBegin)
		{
			int a = 10;
			listOfIntegerPtrs->PushBack(&a);

			SList<int*>::Iterator itr = listOfIntegerPtrs->begin();
			Assert::AreEqual(&a, *itr);
		}

		TEST_METHOD(SListIntegerPointerTestEnd)
		{
			int a = 10;
			listOfIntegerPtrs->PushBack(&a);

			SList<int*>::Iterator itr = listOfIntegerPtrs->end();
			auto expression = [&] { int* b = *itr; UNREFERENCED_PARAMETER(b);  };
			Assert::ExpectException<std::exception>(expression);
		}

		TEST_METHOD(SListIntegerPointerTestInsertAfter)
		{
			SList<int*> anotherIntList;

			int a = 10;
			int b = 20;
			int d = 25;
			listOfIntegerPtrs->PushBack(&a);
			listOfIntegerPtrs->PushBack(&b);

			{
				// sending iterator belonging to another list 
				SList<int*>::Iterator itr = anotherIntList.begin();
				auto expression = [&] { listOfIntegerPtrs->InsertAfter(&a, itr); };
				Assert::ExpectException<std::exception>(expression);
			}

			{
				// InsertAfter on an empty list
				SList<int*>::Iterator itr = anotherIntList.begin();
				anotherIntList.InsertAfter(&a, itr);
				Assert::AreEqual(&a, anotherIntList.Front());
			}

			{
				// InsertAfter on iterator pointing to end of list
				SList<int*>::Iterator itr = listOfIntegerPtrs->end();
				listOfIntegerPtrs->InsertAfter(&d, itr);
				Assert::AreEqual(&d, listOfIntegerPtrs->Back());
			}

			SList<int*>::Iterator itr = listOfIntegerPtrs->begin();
			int c = 15;
			listOfIntegerPtrs->InsertAfter(&c, itr);


			int value = 10;
			for (; itr != listOfIntegerPtrs->end(); ++itr)
			{
				Assert::AreEqual(value, **itr);
				value += 5;
			}

		}

		TEST_METHOD(SListIntegerPointerTestFind)
		{
			int a = 10;
			int b = 20;
			int c = 30;
			listOfIntegerPtrs->PushBack(&a);
			listOfIntegerPtrs->PushBack(&b);
			listOfIntegerPtrs->PushBack(&c);

			SList<int*>::Iterator itr = listOfIntegerPtrs->Find(&b);
			Assert::AreEqual(&b, *itr);

			int d = 40;
			itr = listOfIntegerPtrs->Find(&d);
			Assert::IsTrue(itr == listOfIntegerPtrs->end());
		}

		TEST_METHOD(SListIntegerPointerTestRemove)
		{
			int a = 10;
			int b = 20;
			int c = 30;

			// empty list
			listOfIntegerPtrs->Remove(&a);
			Assert::IsTrue(listOfIntegerPtrs->IsEmpty());

			// last element
			listOfIntegerPtrs->PushBack(&a);
			listOfIntegerPtrs->Remove(&a);
			Assert::IsTrue(listOfIntegerPtrs->IsEmpty());

			// remove middle element
			listOfIntegerPtrs->PushBack(&a);
			listOfIntegerPtrs->PushBack(&b);
			listOfIntegerPtrs->PushBack(&c);
			listOfIntegerPtrs->Remove(&b);

			Assert::AreEqual(2U, listOfIntegerPtrs->Size());
			Assert::AreEqual(&a, listOfIntegerPtrs->Front());
			Assert::AreEqual(&c, listOfIntegerPtrs->Back());

			// remove last element
			listOfIntegerPtrs->Remove(&c);
			Assert::AreEqual(&a, listOfIntegerPtrs->Back());
		}

		/********************************* Tests for list of Foos ****************************************************/

		TEST_METHOD(SListFooTestCtor)
		{
			Assert::IsTrue(listOfFoos->IsEmpty());
		}

		TEST_METHOD(SListFooTestPushFront)
		{
			Foo a(10);
			listOfFoos->PushFront(a);

			Assert::IsTrue(a == listOfFoos->Front());
			Assert::IsTrue(a == listOfFoos->Back());
			Assert::AreEqual(1U, listOfFoos->Size());

			Foo b(20);
			listOfFoos->PushFront(b);
			Assert::IsTrue(b == listOfFoos->Front());

			Assert::IsTrue(a == listOfFoos->Back());
		}

		TEST_METHOD(SListFooTestPopFront)
		{
			try
			{
				listOfFoos->PopFront();
				Assert::Fail();
			}
			catch (const std::exception& e)
			{
				e.what();
			}

			Foo a(10);
			listOfFoos->PushFront(a);
			listOfFoos->PopFront();
			Assert::IsTrue(listOfFoos->IsEmpty());
			try
			{
				const Foo& back = listOfFoos->Back();
				UNREFERENCED_PARAMETER(back);
				Assert::Fail();
			}
			catch (const std::exception& e)
			{
				e.what();
			}

			Foo b(20);
			listOfFoos->PushFront(a);
			listOfFoos->PushFront(b);
			listOfFoos->PopFront();
			Assert::IsTrue(a == listOfFoos->Front());
		}

		TEST_METHOD(SListFooTestPushBack)
		{
			Foo a(10);
			listOfFoos->PushBack(a);
			Assert::IsTrue(a == listOfFoos->Back());
			Assert::IsTrue(a == listOfFoos->Front());
			Assert::AreEqual(1U, listOfFoos->Size());

			Foo b(20);
			listOfFoos->PushBack(b);
			Assert::IsTrue(b == listOfFoos->Back());
			Assert::IsTrue(a == listOfFoos->Front());

			Foo c(30);
			listOfFoos->PushFront(c);
			Assert::IsTrue(b == listOfFoos->Back());
		}

		TEST_METHOD(SListFooTestSize)
		{
			listOfFoos->PushBack(Foo(10));
			listOfFoos->PushBack(Foo(20));
			listOfFoos->PushBack(Foo(30));
			Assert::AreEqual(3U, listOfFoos->Size());
		}

		TEST_METHOD(SListFooTestClear)
		{
			listOfFoos->PushBack(Foo(10));
			listOfFoos->PushBack(Foo(20));
			listOfFoos->PushBack(Foo(30));
			listOfFoos->Clear();
			Assert::IsTrue(listOfFoos->IsEmpty());
		}

		TEST_METHOD(SListFooTestCopyCtor)
		{
			Foo a(1);
			Foo b(2);
			Foo c(3);
			Foo d(4);
			Assert::IsTrue(listOfFoos->IsEmpty());
			listOfFoos->PushBack(a);
			listOfFoos->PushBack(b);
			listOfFoos->PushBack(c);
			listOfFoos->PushBack(d);

			SList<Foo> copyList(*listOfFoos);
			Assert::AreEqual(4U, copyList.Size());
			Assert::IsTrue(a == copyList.Front());

			copyList.PopFront();
			Assert::IsTrue(b == copyList.Front());

			copyList.PopFront();
			Assert::IsTrue(c == copyList.Front());
	
			copyList.PopFront();
			Assert::IsTrue(d == copyList.Front());
		}
		
		TEST_METHOD(SListFooTestAssignOperator)
		{
			Foo a(10);
			Foo b(20);
			Foo c(30);
			SList<Foo> copyList;
			copyList.PushBack(a);
			copyList.PushBack(b);
			copyList.PushBack(c);

			*listOfFoos = copyList;
			Assert::AreEqual(3U, listOfFoos->Size());

			Assert::IsTrue(a == listOfFoos->Front());

			listOfFoos->PopFront();
			Assert::IsTrue(b == listOfFoos->Front());

			listOfFoos->PopFront();
			Assert::IsTrue(c == listOfFoos->Front());

		}

		/**
		* Test function Front() for the list of poitners to integers
		*/
		TEST_METHOD(SListFooTestFront)
		{
			try
			{
				const Foo& front = listOfFoos->Front();
				UNREFERENCED_PARAMETER(front);
				Assert::Fail();
			}
			catch (const std::exception& e)
			{
				UNREFERENCED_PARAMETER(e);
			}

			Foo a(10);
			Foo b(20);
			Foo c(30);
			listOfFoos->PushFront(a);
			listOfFoos->PushFront(b);
			Assert::IsTrue(b == listOfFoos->Front());

			listOfFoos->PushBack(c);
			Assert::IsTrue(b == listOfFoos->Front());
		}

		/**
		* Test function Back() for the list of pointers to integers
		*/
		TEST_METHOD(SListFooTestBack)
		{
			try
			{
			    const Foo& back = listOfFoos->Back();
				UNREFERENCED_PARAMETER(back);
				Assert::Fail();
			}
			catch (const std::exception& e)
			{
				UNREFERENCED_PARAMETER(e);
			}

			Foo a(10);
			Foo b(20);
			Foo c(30);
			listOfFoos->PushFront(a);
			listOfFoos->PushFront(b);
			Assert::IsTrue(a == listOfFoos->Back());

			listOfFoos->PushBack(c);
			Assert::IsTrue(c == listOfFoos->Back());
		}


		TEST_METHOD(SListFooTestBegin)
		{
			Foo a(10);
			listOfFoos->PushBack(a);

			SList<Foo>::Iterator itr = listOfFoos->begin();
			Assert::IsTrue(a == *itr);
		}

		TEST_METHOD(SListFooTestEnd)
		{
			Foo a(10);
			listOfFoos->PushBack(a);

			SList<Foo>::Iterator itr = listOfFoos->end();
			auto expression = [&] { Foo b = *itr; UNREFERENCED_PARAMETER(b);  };
			Assert::ExpectException<std::exception>(expression);
		}

		TEST_METHOD(SListFooTestInsertAfter)
		{
			SList<Foo> anotherIntList;

			Foo a(10);
			Foo b(20);
			Foo d(25);
			listOfFoos->PushBack(a);
			listOfFoos->PushBack(b);

			{
				// sending iterator belonging to another list 
				SList<Foo>::Iterator itr = anotherIntList.begin();
				auto expression = [&] { listOfFoos->InsertAfter(a, itr); };
				Assert::ExpectException<std::exception>(expression);
			}

			{
				// InsertAfter on an empty list
				SList<Foo>::Iterator itr = anotherIntList.begin();
				anotherIntList.InsertAfter(a, itr);
				Assert::IsTrue(a == anotherIntList.Front());
			}

			{
				// InsertAfter on iterator pointing to end of list
				SList<Foo>::Iterator itr = listOfFoos->end();
				listOfFoos->InsertAfter(d, itr);
				Assert::IsTrue(d == listOfFoos->Back());
			}

			SList<Foo>::Iterator itr = listOfFoos->begin();
			Foo c(15);
			listOfFoos->InsertAfter(c, itr);
			
			itr = listOfFoos->begin();
			Assert::IsTrue(a == *itr);
			++itr;
			Assert::IsTrue(c == *itr);
			++itr;
			Assert::IsTrue(b == *itr);

		}

		TEST_METHOD(SListFooTestFind)
		{
			Foo a(10);
			Foo b(20);
			Foo c(30);
			listOfFoos->PushBack(a);
			listOfFoos->PushBack(b);
			listOfFoos->PushBack(c);

			SList<Foo>::Iterator itr = listOfFoos->Find(b);
			Assert::IsTrue(b == *itr);

			Foo d(40);
			itr = listOfFoos->Find(d);
			Assert::IsTrue(itr == listOfFoos->end());
		}

		TEST_METHOD(SListFooTestRemove)
		{
			Foo a(10);
			Foo b(20);
			Foo c(30);

			// empty list
			listOfFoos->Remove(a);
			Assert::IsTrue(listOfFoos->IsEmpty());

			// last element
			listOfFoos->PushBack(a);
			listOfFoos->Remove(a);
			Assert::IsTrue(listOfFoos->IsEmpty());

			// remove middle element
			listOfFoos->PushBack(a);
			listOfFoos->PushBack(b);
			listOfFoos->PushBack(c);
			listOfFoos->Remove(b);

			Assert::AreEqual(2U, listOfFoos->Size());
			Assert::IsTrue(a == listOfFoos->Front());
			Assert::IsTrue(c == listOfFoos->Back());

			// remove last element
			listOfFoos->Remove(c);
			Assert::IsTrue(a == listOfFoos->Back());
		}


		SList<int>* listOfIntegers;
		SList<int*>* listOfIntegerPtrs;
		SList<Foo>* listOfFoos;

#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState sStartMemState;
#endif
	};

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState SListTest::sStartMemState;
#endif
}