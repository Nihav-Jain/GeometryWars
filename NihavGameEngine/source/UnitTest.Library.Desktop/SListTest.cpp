#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

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
		}

		/********************************* Tests for list of integers ************************************/

		/**
		 * Test the constructor for the list of integers
		 */
		TEST_METHOD(SListTestCtor)
		{
			// the constructor of SList must initialize an empty list
			Assert::IsTrue(listOfIntegers->isEmpty());
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
			Assert::AreEqual(1, listOfIntegers->Size());

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
			Assert::IsTrue(listOfIntegers->isEmpty());

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
			Assert::AreEqual(1, listOfIntegers->Size());

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
			Assert::AreEqual(3, listOfIntegers->Size());
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
			Assert::IsTrue(listOfIntegers->isEmpty());
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
			Assert::AreEqual(4, copyList.Size());
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
			Assert::AreEqual(3, listOfIntegers->Size());
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


		/********************************* Tests for list of pointers to integers ************************************/

		TEST_METHOD(SListIntegerPointerTestCtor)
		{
			Assert::IsTrue(listOfIntegerPtrs->isEmpty());
		}

		TEST_METHOD(SListIntegerPointerTestPushFront)
		{
			// test for list of pointers to integers
			int a = 10;
			listOfIntegerPtrs->PushFront(&a);
			Assert::AreEqual(&a, listOfIntegerPtrs->Front());
			Assert::AreEqual(&a, listOfIntegerPtrs->Back());
			Assert::AreEqual(1, listOfIntegerPtrs->Size());

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
			Assert::IsTrue(listOfIntegerPtrs->isEmpty());
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
			Assert::AreEqual(1, listOfIntegerPtrs->Size());

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
			Assert::AreEqual(3, listOfIntegerPtrs->Size());
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
			Assert::IsTrue(listOfIntegerPtrs->isEmpty());
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
			Assert::AreEqual(4, copyList.Size());
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
			Assert::AreEqual(3, listOfIntegerPtrs->Size());
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


		/********************************* Tests for list of Foos ****************************************************/

		TEST_METHOD(SListFooTestCtor)
		{
			Assert::IsTrue(listOfFoos->isEmpty());
		}

		TEST_METHOD(SListFooTestPushFront)
		{
			Foo a(10);
			listOfFoos->PushFront(a);

			Assert::IsTrue(a == listOfFoos->Front());
			Assert::IsTrue(a == listOfFoos->Back());
			Assert::AreEqual(1, listOfFoos->Size());

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
			Assert::IsTrue(listOfFoos->isEmpty());
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
			Assert::AreEqual(1, listOfFoos->Size());

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
			Assert::AreEqual(3, listOfFoos->Size());
		}

		TEST_METHOD(SListFooTestClear)
		{
			listOfFoos->PushBack(Foo(10));
			listOfFoos->PushBack(Foo(20));
			listOfFoos->PushBack(Foo(30));
			listOfFoos->Clear();
			Assert::IsTrue(listOfFoos->isEmpty());
		}

		TEST_METHOD(SListFooTestCopyCtor)
		{
			Foo a(1);
			Foo b(2);
			Foo c(3);
			Foo d(4);
			Assert::IsTrue(listOfFoos->isEmpty());
			listOfFoos->PushBack(a);
			listOfFoos->PushBack(b);
			listOfFoos->PushBack(c);
			listOfFoos->PushBack(d);

			SList<Foo> copyList(*listOfFoos);
			Assert::AreEqual(4, copyList.Size());
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
			Assert::AreEqual(3, listOfFoos->Size());

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

		SList<int>* listOfIntegers;
		SList<int*>* listOfIntegerPtrs;
		SList<Foo>* listOfFoos;
	};
}