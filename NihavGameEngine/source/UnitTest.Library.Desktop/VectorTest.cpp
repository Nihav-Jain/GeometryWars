#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;

namespace UnitTestLibraryDesktop
{

	class ReserveStrategy
	{
	public:
		std::uint32_t operator()(std::uint32_t size, std::uint32_t capacity) const
		{
			UNREFERENCED_PARAMETER(size);
			return capacity + 2;
		}
	};

	class BadReserveStrategy
	{
	public:
		std::uint32_t operator()(std::uint32_t size, std::uint32_t capacity) const
		{
			UNREFERENCED_PARAMETER(size);
			return capacity;
		}
	};

	TEST_CLASS(VectorTest)
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

#pragma region VectorTests

		TEST_METHOD(VectorTestCtor)
		{
			{
				Vector<int> vectorOfInts;
				Assert::IsTrue(vectorOfInts.IsEmpty());
				Assert::AreEqual(4U, vectorOfInts.Capacity());
			}

			{
				Vector<int*> vectorOfIntPtrs;
				Assert::IsTrue(vectorOfIntPtrs.IsEmpty());
				Assert::AreEqual(4U, vectorOfIntPtrs.Capacity());
			}

			{
				Vector<Foo> vectorOfFoos;
				Assert::IsTrue(vectorOfFoos.IsEmpty());
				Assert::AreEqual(4U, vectorOfFoos.Capacity());
			}

		}

		TEST_METHOD(VectorTestCopyCtor)
		{
			{
				Vector<int> vectorOfInts;
				vectorOfInts.PushBack(10);
				vectorOfInts.PushBack(20);

				Vector<int> anotherVectorOfInts(vectorOfInts);
				Assert::AreEqual(2, (int)anotherVectorOfInts.Size());
				Assert::AreEqual(10, anotherVectorOfInts[0]);
				Assert::AreEqual(20, anotherVectorOfInts[1]);
			}

			{
				int a = 10;
				int b = 20;
				Vector<int*> vectorOfIntPtrs;
				vectorOfIntPtrs.PushBack(&a);
				vectorOfIntPtrs.PushBack(&b);

				Vector<int*> anotherVectorOfIntPtrs(vectorOfIntPtrs);
				Assert::AreEqual(2, (int)anotherVectorOfIntPtrs.Size());
				Assert::AreEqual(&a, anotherVectorOfIntPtrs[0]);
				Assert::AreEqual(&b, anotherVectorOfIntPtrs[1]);
			}

			{
				Foo a(10);
				Foo b(20);
				Vector<Foo> vectorOfFoos;
				vectorOfFoos.PushBack(a);
				vectorOfFoos.PushBack(b);

				Vector<Foo> anotherVectorOfFoos(vectorOfFoos);
				Assert::AreEqual(2U, anotherVectorOfFoos.Size());
				Assert::IsTrue(a == anotherVectorOfFoos[0]);
				Assert::IsTrue(b == anotherVectorOfFoos[1]);
			}

		}

		TEST_METHOD(VectorTestMoveCtor)
		{
			{
				Vector<int> vectorOfInts;
				vectorOfInts.PushBack(10);
				vectorOfInts.PushBack(20);

				Vector<int> anotherVectorOfInts = std::move(vectorOfInts);
				Assert::IsTrue(vectorOfInts.IsEmpty());
				Assert::AreEqual(10, anotherVectorOfInts.Front());
				Assert::AreEqual(20, anotherVectorOfInts.Back());
			}

			{
				int a = 10;
				int b = 20;
				Vector<int*> vectorOfIntPtrs;
				vectorOfIntPtrs.PushBack(&a);
				vectorOfIntPtrs.PushBack(&b);

				Vector<int*> anotherVectorOfIntPtrs = std::move(vectorOfIntPtrs);
				Assert::IsTrue(vectorOfIntPtrs.IsEmpty());
				Assert::AreEqual(&a, anotherVectorOfIntPtrs.Front());
				Assert::AreEqual(&b, anotherVectorOfIntPtrs.Back());
			}

			{
				Foo a(10);
				Foo b(20);

				Vector<Foo> vectorOfFoos;
				vectorOfFoos.PushBack(a);
				vectorOfFoos.PushBack(b);

				Vector<Foo> anotherVectorOfFoos = std::move(vectorOfFoos);
				Assert::IsTrue(vectorOfFoos.IsEmpty());
				Assert::IsTrue(a == anotherVectorOfFoos.Front());
				Assert::IsTrue(b == anotherVectorOfFoos.Back());
			}
		}

		TEST_METHOD(VectorTestMoveAssignementOperator)
		{
			{
				Vector<int> vectorOfInts;
				vectorOfInts.PushBack(10);
				vectorOfInts.PushBack(20);

				Vector<int> anotherVectorOfInts;
				anotherVectorOfInts.PushBack(30);
				anotherVectorOfInts.PushBack(40);

				anotherVectorOfInts = std::move(vectorOfInts);
				Assert::IsTrue(vectorOfInts.IsEmpty());
				Assert::AreEqual(10, anotherVectorOfInts.Front());
				Assert::AreEqual(20, anotherVectorOfInts.Back());
			}

			{
				int a = 10;
				int b = 20;
				Vector<int*> vectorOfIntPtrs;
				vectorOfIntPtrs.PushBack(&a);
				vectorOfIntPtrs.PushBack(&b);

				int c = 30;
				int d = 40;
				Vector<int*> anotherVectorOfIntPtrs;
				anotherVectorOfIntPtrs.PushBack(&c);
				anotherVectorOfIntPtrs.PushBack(&d);

				anotherVectorOfIntPtrs = std::move(vectorOfIntPtrs);
				Assert::IsTrue(vectorOfIntPtrs.IsEmpty());
				Assert::AreEqual(&a, anotherVectorOfIntPtrs.Front());
				Assert::AreEqual(&b, anotherVectorOfIntPtrs.Back());
			}

			{
				Foo a(10);
				Foo b(20);

				Vector<Foo> vectorOfFoos;
				vectorOfFoos.PushBack(a);
				vectorOfFoos.PushBack(b);

				Foo c(30);
				Foo d(40);
				Vector<Foo> anotherVectorOfFoos;
				anotherVectorOfFoos.PushBack(c);
				anotherVectorOfFoos.PushBack(d);

				anotherVectorOfFoos = std::move(vectorOfFoos);
				Assert::IsTrue(vectorOfFoos.IsEmpty());
				Assert::IsTrue(a == anotherVectorOfFoos.Front());
				Assert::IsTrue(b == anotherVectorOfFoos.Back());
			}

		}

		TEST_METHOD(VectorTestAssigOperator)
		{
			{
				Vector<int> vectorOfInts;
				vectorOfInts.PushBack(10);
				vectorOfInts.PushBack(20);

				Vector<int> anotherVectorOfInts;
				anotherVectorOfInts.PushBack(30);
				anotherVectorOfInts.PushBack(40);

				anotherVectorOfInts = vectorOfInts;
				Assert::AreEqual(2, (int)anotherVectorOfInts.Size());
				Assert::AreEqual(10, anotherVectorOfInts[0]);
				Assert::AreEqual(20, anotherVectorOfInts[1]);

				anotherVectorOfInts = anotherVectorOfInts;
				Assert::AreEqual(2, (int)anotherVectorOfInts.Size());
				Assert::AreEqual(10, anotherVectorOfInts[0]);
				Assert::AreEqual(20, anotherVectorOfInts[1]);
			}

			{
				int a = 10;
				int b = 20;
				Vector<int*> vectorOfIntPtrs;
				vectorOfIntPtrs.PushBack(&a);
				vectorOfIntPtrs.PushBack(&b);

				int c = 30;
				int d = 40;
				Vector<int*> anotherVectorOfIntPtrs;
				anotherVectorOfIntPtrs.PushBack(&c);
				anotherVectorOfIntPtrs.PushBack(&d);

				anotherVectorOfIntPtrs = vectorOfIntPtrs;
				Assert::AreEqual(2U, anotherVectorOfIntPtrs.Size());
				Assert::AreEqual(&a, anotherVectorOfIntPtrs[0]);
				Assert::AreEqual(&b, anotherVectorOfIntPtrs[1]);

				anotherVectorOfIntPtrs = anotherVectorOfIntPtrs;
				Assert::AreEqual(2U, anotherVectorOfIntPtrs.Size());
				Assert::AreEqual(&a, anotherVectorOfIntPtrs[0]);
				Assert::AreEqual(&b, anotherVectorOfIntPtrs[1]);
			}

			{
				Foo a(10);
				Foo b(20);
				Vector<Foo> vectorOfIntFoos;
				vectorOfIntFoos.PushBack(a);
				vectorOfIntFoos.PushBack(b);

				Foo c(30);
				Foo d(40);
				Vector<Foo> anotherVectorOfFoos;
				anotherVectorOfFoos.PushBack(c);
				anotherVectorOfFoos.PushBack(d);

				anotherVectorOfFoos = vectorOfIntFoos;
				Assert::AreEqual(2U, anotherVectorOfFoos.Size());
				Assert::IsTrue(a == anotherVectorOfFoos[0]);
				Assert::IsTrue(b == anotherVectorOfFoos[1]);

				anotherVectorOfFoos = anotherVectorOfFoos;
				Assert::AreEqual(2U, anotherVectorOfFoos.Size());
				Assert::IsTrue(a == anotherVectorOfFoos[0]);
				Assert::IsTrue(b == anotherVectorOfFoos[1]);
			}


		}

		TEST_METHOD(VectorTestSubcriptOperator)
		{
			{
				Vector<int> vectorOfInts;
				vectorOfInts.PushBack(10);
				vectorOfInts.PushBack(20);

				Assert::AreEqual(10, vectorOfInts[0]);
				Assert::AreEqual(20, vectorOfInts[1]);

				auto expression = [&] {int a = vectorOfInts[2]; UNREFERENCED_PARAMETER(a); };
				Assert::ExpectException<std::exception>(expression);

				vectorOfInts[1] = 30;
				Assert::AreEqual(30, vectorOfInts[1]);

				const Vector<int> constVectorOfInts = vectorOfInts;
				Assert::AreEqual(10, constVectorOfInts[0]);
				auto expression2 = [&] {int a = constVectorOfInts[2]; UNREFERENCED_PARAMETER(a); };
				Assert::ExpectException<std::exception>(expression2);
			}

			{
				int a = 10;
				int b = 20;
				Vector<int*> vectorOfIntPtrs;
				vectorOfIntPtrs.PushBack(&a);
				vectorOfIntPtrs.PushBack(&b);

				Assert::AreEqual(&a, vectorOfIntPtrs[0]);
				Assert::AreEqual(&b, vectorOfIntPtrs[1]);

				auto expression = [&] {int* a = vectorOfIntPtrs[2]; UNREFERENCED_PARAMETER(a); };
				Assert::ExpectException<std::exception>(expression);

				int c = 30;
				vectorOfIntPtrs[1] = &c;
				Assert::AreEqual(&c, vectorOfIntPtrs[1]);
				
				const Vector<int*> constVectorOfInts = vectorOfIntPtrs;
				Assert::AreEqual(&a, constVectorOfInts[0]);
				auto expression2 = [&] {int* a = constVectorOfInts[2]; UNREFERENCED_PARAMETER(a); };
				Assert::ExpectException<std::exception>(expression2);
			}

			{
				Foo a(10);
				Foo b(20);
				Vector<Foo> vectorOfFoos;
				vectorOfFoos.PushBack(a);
				vectorOfFoos.PushBack(b);

				Assert::IsTrue(a == vectorOfFoos[0]);
				Assert::IsTrue(b == vectorOfFoos[1]);

				auto expression = [&] {Foo a = vectorOfFoos[2]; UNREFERENCED_PARAMETER(a); };
				Assert::ExpectException<std::exception>(expression);

				Foo c(30);
				vectorOfFoos[1] = c;
				Assert::IsTrue(c == vectorOfFoos[1]);

				const Vector<Foo> constVectorOfInts = vectorOfFoos;
				Assert::IsTrue(a == constVectorOfInts[0]);
				auto expression2 = [&] {Foo a = constVectorOfInts[2]; UNREFERENCED_PARAMETER(a); };
				Assert::ExpectException<std::exception>(expression2);			
			}

		}

		TEST_METHOD(VectorTestPopBack)
		{
			{
				Vector<int> vectorOfInts;

				auto expression = [&] { vectorOfInts.PopBack(); };
				Assert::ExpectException<std::exception>(expression);

				vectorOfInts.PushBack(10);
				vectorOfInts.PushBack(20);

				vectorOfInts.PopBack();
				Assert::AreEqual(1U, vectorOfInts.Size());
				Assert::AreEqual(10, vectorOfInts.Back());
			}

			{
				Vector<int*> vectorOfIntPtrs;

				auto expression = [&] { vectorOfIntPtrs.PopBack(); };
				Assert::ExpectException<std::exception>(expression);

				int a = 10;
				int b = 20;
				vectorOfIntPtrs.PushBack(&a);
				vectorOfIntPtrs.PushBack(&b);

				vectorOfIntPtrs.PopBack();
				Assert::AreEqual(1U, vectorOfIntPtrs.Size());
				Assert::AreEqual(&a, vectorOfIntPtrs.Back());
			}

			{
				Vector<Foo> vectorOfFoos;

				auto expression = [&] { vectorOfFoos.PopBack(); };
				Assert::ExpectException<std::exception>(expression);

				Foo a(10);
				Foo b(20);
				vectorOfFoos.PushBack(a);
				vectorOfFoos.PushBack(b);

				vectorOfFoos.PopBack();
				Assert::AreEqual(1U, vectorOfFoos.Size());
				Assert::IsTrue(a == vectorOfFoos.Back());
			}

		}

		TEST_METHOD(VectorTestPushBack)
		{
			{
				Vector<int, BadReserveStrategy> vectorOfInts;
				vectorOfInts.PushBack(10);
				vectorOfInts.PushBack(20);

				Assert::AreEqual(10, vectorOfInts[0]);
				Assert::AreEqual(20, vectorOfInts[1]);

				vectorOfInts.PushBack(30);
				vectorOfInts.PushBack(40);

				auto expression = [&] {vectorOfInts.PushBack(50); };
				Assert::ExpectException<std::exception>(expression);
			}
			{

				Vector<int, ReserveStrategy> vectorOfInts;
				vectorOfInts.PushBack(10);
				vectorOfInts.PushBack(20);

				Assert::AreEqual(10, vectorOfInts[0]);
				Assert::AreEqual(20, vectorOfInts[1]);

				vectorOfInts.PushBack(30);
				vectorOfInts.PushBack(40);

				vectorOfInts.PushBack(50);

				Assert::AreEqual(6U, vectorOfInts.Capacity());
				Assert::AreEqual(50, vectorOfInts[4]);
				
				vectorOfInts.PushBack(60);
				vectorOfInts.PushBack(70);
				Assert::AreEqual(8U, vectorOfInts.Capacity());
				Assert::AreEqual(70, vectorOfInts.Back());
			}

			{
				int a = 10;
				int b = 20;
				Vector<int*, BadReserveStrategy> vectorOfIntPtrs;
				vectorOfIntPtrs.PushBack(&a);
				vectorOfIntPtrs.PushBack(&b);

				Assert::AreEqual(&a, vectorOfIntPtrs[0]);
				Assert::AreEqual(&b, vectorOfIntPtrs[1]);

				int c = 30;
				int d = 40;
				int e = 50;
				vectorOfIntPtrs.PushBack(&c);
				vectorOfIntPtrs.PushBack(&d);

				auto expression = [&] {vectorOfIntPtrs.PushBack(&e); };
				Assert::ExpectException<std::exception>(expression);
			}
			{

				int a = 10;
				int b = 20;
				Vector<int*, ReserveStrategy> vectorOfIntPtrs;
				vectorOfIntPtrs.PushBack(&a);
				vectorOfIntPtrs.PushBack(&b);

				Assert::AreEqual(&a, vectorOfIntPtrs[0]);
				Assert::AreEqual(&b, vectorOfIntPtrs[1]);

				int c = 30;
				int d = 40;
				int e = 50;
				vectorOfIntPtrs.PushBack(&c);
				vectorOfIntPtrs.PushBack(&d);

				vectorOfIntPtrs.PushBack(&e);

				Assert::AreEqual(6U, vectorOfIntPtrs.Capacity());
				Assert::AreEqual(&e, vectorOfIntPtrs[4]);

				int f = 60;
				int g = 70;
				vectorOfIntPtrs.PushBack(&f);
				vectorOfIntPtrs.PushBack(&g);
				Assert::AreEqual(8U, vectorOfIntPtrs.Capacity());
				Assert::AreEqual(&g, vectorOfIntPtrs.Back());
			}

			{
				Foo a(10);
				Foo b(20);
				Vector<Foo, BadReserveStrategy> vectorOfFoos;
				vectorOfFoos.PushBack(a);
				vectorOfFoos.PushBack(b);

				Assert::IsTrue(a == vectorOfFoos[0]);
				Assert::IsTrue(b == vectorOfFoos[1]);

				Foo c(30);
				Foo d(40);
				Foo e(50);
				vectorOfFoos.PushBack(c);
				vectorOfFoos.PushBack(d);

				auto expression = [&] {vectorOfFoos.PushBack(e); };
				Assert::ExpectException<std::exception>(expression);
			}
			{

				Foo a(10);
				Foo b(20);
				Vector<Foo, ReserveStrategy> vectorOfFoos;
				vectorOfFoos.PushBack(a);
				vectorOfFoos.PushBack(b);

				Assert::IsTrue(a == vectorOfFoos[0]);
				Assert::IsTrue(b == vectorOfFoos[1]);

				Foo c(30);
				Foo d(40);
				Foo e(50);
				vectorOfFoos.PushBack(c);
				vectorOfFoos.PushBack(d);

				vectorOfFoos.PushBack(e);

				Assert::AreEqual(6U, vectorOfFoos.Capacity());
				Assert::IsTrue(e == vectorOfFoos[4]);

				Foo f(60);
				Foo g(70);
				vectorOfFoos.PushBack(f);
				vectorOfFoos.PushBack(g);
				Assert::AreEqual(8U, vectorOfFoos.Capacity());
				Assert::IsTrue(g == vectorOfFoos.Back());
			}
		}

		TEST_METHOD(VectorTestFront)
		{
			{
				Vector<int> vectorOfInts;
				auto expression = [&] { vectorOfInts.Front(); };
				Assert::ExpectException<std::exception>(expression);

				vectorOfInts.PushBack(10);
				vectorOfInts.PushBack(20);
				Assert::AreEqual(10, vectorOfInts.Front());

				int& a = vectorOfInts.Front();
				a = 30;
				Assert::AreEqual(30, vectorOfInts.Front());

				const Vector<int> constVectorOfInts;
				auto expression2 = [&] { constVectorOfInts.Front(); };
				Assert::ExpectException<std::exception>(expression2);

				const Vector<int> constAnotherVectorOfInts = vectorOfInts;
				Assert::AreEqual(30, constAnotherVectorOfInts.Front());
			}

			{
				Vector<int*> vectorOfIntPtrs;
				auto expression = [&] { vectorOfIntPtrs.Front(); };
				Assert::ExpectException<std::exception>(expression);

				int a = 10;
				int b = 20;
				vectorOfIntPtrs.PushBack(&a);
				vectorOfIntPtrs.PushBack(&b);
				Assert::AreEqual(&a, vectorOfIntPtrs.Front());

				const Vector<int*> constVectorOfInts;
				auto expression2 = [&] { constVectorOfInts.Front(); };
				Assert::ExpectException<std::exception>(expression2);

				const Vector<int*> constAnotherVectorOfInts = vectorOfIntPtrs;
				Assert::AreEqual(&a, constAnotherVectorOfInts.Front());
			}

			{
				Vector<Foo> vectorOfFoo;
				auto expression = [&] { vectorOfFoo.Front(); };
				Assert::ExpectException<std::exception>(expression);

				Foo a(10);
				Foo b(20);
				vectorOfFoo.PushBack(a);
				vectorOfFoo.PushBack(b);
				Assert::IsTrue(a == vectorOfFoo.Front());

				const Vector<Foo> constVectorOfInts;
				auto expression2 = [&] { constVectorOfInts.Front(); };
				Assert::ExpectException<std::exception>(expression2);

				const Vector<Foo> constAnotherVectorOfInts = vectorOfFoo;
				Assert::IsTrue(a == constAnotherVectorOfInts.Front());
			}
		}

		TEST_METHOD(VectorTestBack)
		{
			{
				Vector<int> vectorOfInts;
				auto expression = [&] { vectorOfInts.Back(); };
				Assert::ExpectException<std::exception>(expression);

				vectorOfInts.PushBack(10);
				vectorOfInts.PushBack(20);
				Assert::AreEqual(20, vectorOfInts.Back());

				int& a = vectorOfInts.Back();
				a = 30;
				Assert::AreEqual(30, vectorOfInts.Back());

				const Vector<int> constVectorOfInts;
				auto expression2 = [&] { constVectorOfInts.Back(); };
				Assert::ExpectException<std::exception>(expression2);

				const Vector<int> constAnotherVectorOfInts = vectorOfInts;
				Assert::AreEqual(30, constAnotherVectorOfInts.Back());
			}

			{
				Vector<int*> vectorOfIntPtrs;
				auto expression = [&] { vectorOfIntPtrs.Back(); };
				Assert::ExpectException<std::exception>(expression);

				int a = 10;
				int b = 20;
				vectorOfIntPtrs.PushBack(&a);
				vectorOfIntPtrs.PushBack(&b);
				Assert::AreEqual(&b, vectorOfIntPtrs.Back());

				const Vector<int*> constVectorOfInts;
				auto expression2 = [&] { constVectorOfInts.Back(); };
				Assert::ExpectException<std::exception>(expression2);

				const Vector<int*> constAnotherVectorOfInts = vectorOfIntPtrs;
				Assert::AreEqual(&b, constAnotherVectorOfInts.Back());			
			}

			{
				Vector<Foo> vectorOfFoos;
				auto expression = [&] { vectorOfFoos.Back(); };
				Assert::ExpectException<std::exception>(expression);

				Foo a(10);
				Foo b(20);
				vectorOfFoos.PushBack(a);
				vectorOfFoos.PushBack(b);
				Assert::IsTrue(b == vectorOfFoos.Back());

				const Vector<Foo> constVectorOfInts;
				auto expression2 = [&] { constVectorOfInts.Back(); };
				Assert::ExpectException<std::exception>(expression2);

				const Vector<Foo> constAnotherVectorOfInts = vectorOfFoos;
				Assert::IsTrue(b == constAnotherVectorOfInts.Back());
			}

		}

		TEST_METHOD(VectorTestIsEmpty)
		{
			{
				Vector<int> vectorOfInts;
				Assert::IsTrue(vectorOfInts.IsEmpty());
				vectorOfInts.PushBack(10);
				Assert::IsFalse(vectorOfInts.IsEmpty());
			}

			{
				int a = 10;
				Vector<int*> vectorOfIntPtrs;
				Assert::IsTrue(vectorOfIntPtrs.IsEmpty());
				vectorOfIntPtrs.PushBack(&a);
				Assert::IsFalse(vectorOfIntPtrs.IsEmpty());
			}

			{
				Foo a(10);
				Vector<Foo> vectorOfFoos;
				Assert::IsTrue(vectorOfFoos.IsEmpty());
				vectorOfFoos.PushBack(a);
				Assert::IsFalse(vectorOfFoos.IsEmpty());
			}

		}

		TEST_METHOD(VectorTestAt)
		{
			{
				Vector<int> vectorOfInts;
				vectorOfInts.PushBack(10);
				vectorOfInts.PushBack(20);

				auto expression = [&] {int& a = vectorOfInts.At(vectorOfInts.Capacity() + 1); UNREFERENCED_PARAMETER(a); };
				Assert::ExpectException<std::exception>(expression);

				auto expression2 = [&] {int& a = vectorOfInts.At(vectorOfInts.Size()); UNREFERENCED_PARAMETER(a); };
				Assert::ExpectException<std::exception>(expression2);

				Assert::AreEqual(20, vectorOfInts.At(1));
			}

			{
				int a = 10;
				int b = 20;
				Vector<int*> vectorOfIntPtrs;
				vectorOfIntPtrs.PushBack(&a);
				vectorOfIntPtrs.PushBack(&b);

				auto expression = [&] {int* a = vectorOfIntPtrs.At(vectorOfIntPtrs.Capacity() + 1); UNREFERENCED_PARAMETER(a); };
				Assert::ExpectException<std::exception>(expression);

				auto expression2 = [&] {int* a = vectorOfIntPtrs.At(vectorOfIntPtrs.Size()); UNREFERENCED_PARAMETER(a); };
				Assert::ExpectException<std::exception>(expression2);

				Assert::AreEqual(&b, vectorOfIntPtrs.At(1));
			}

			{
				Foo a(10);
				Foo b(20);
				Vector<Foo> vectorOfFoos;
				vectorOfFoos.PushBack(a);
				vectorOfFoos.PushBack(b);

				auto expression = [&] {Foo a = vectorOfFoos.At(vectorOfFoos.Capacity() + 1); UNREFERENCED_PARAMETER(a); };
				Assert::ExpectException<std::exception>(expression);

				auto expression2 = [&] {Foo a = vectorOfFoos.At(vectorOfFoos.Size()); UNREFERENCED_PARAMETER(a); };
				Assert::ExpectException<std::exception>(expression2);

				Assert::IsTrue(b == vectorOfFoos.At(1));
			}

		}
		
		TEST_METHOD(VectorTestReserve)
		{
			{
				Vector<int> vectorOfInts;
				vectorOfInts.PushBack(10);
				vectorOfInts.PushBack(20);

				Assert::AreEqual(4, (int)vectorOfInts.Capacity());
				vectorOfInts.Reserve(10);
				Assert::AreEqual(10, (int)vectorOfInts.Capacity());

				Assert::AreEqual(10, vectorOfInts[0]);
				Assert::AreEqual(20, vectorOfInts[1]);
			}

			{
				int a = 10;
				int b = 20;
				Vector<int*> vectorOfIntPtrs;
				vectorOfIntPtrs.PushBack(&a);
				vectorOfIntPtrs.PushBack(&b);

				Assert::AreEqual(4U, vectorOfIntPtrs.Capacity());
				vectorOfIntPtrs.Reserve(10);
				Assert::AreEqual(10U, vectorOfIntPtrs.Capacity());

				Assert::AreEqual(&a, vectorOfIntPtrs[0]);
				Assert::AreEqual(&b, vectorOfIntPtrs[1]);
			}

			{
				Foo a(10);
				Foo b(20);
				Vector<Foo> vectorOfFoos;
				vectorOfFoos.PushBack(a);
				vectorOfFoos.PushBack(b);

				Assert::AreEqual(4U, vectorOfFoos.Capacity());
				vectorOfFoos.Reserve(10);
				Assert::AreEqual(10U, vectorOfFoos.Capacity());

				Assert::IsTrue(a == vectorOfFoos[0]);
				Assert::IsTrue(b == vectorOfFoos[1]);
			}

		}

		TEST_METHOD(VectorTestClear)
		{
			{
				Vector<int> vectorOfInts;
				vectorOfInts.PushBack(10);
				vectorOfInts.PushBack(20);
				vectorOfInts.Clear();
				Assert::IsTrue(vectorOfInts.IsEmpty());

				vectorOfInts.PushBack(40);
				Assert::AreEqual(1U, vectorOfInts.Size());
			}

			{
				int a = 10;
				int b = 20;
				Vector<int*> vectorOfIntPtrs;
				vectorOfIntPtrs.PushBack(&a);
				vectorOfIntPtrs.PushBack(&b);
				vectorOfIntPtrs.Clear();
				Assert::IsTrue(vectorOfIntPtrs.IsEmpty());
			}

			{
				Foo a(10);
				Foo b(20);
				Vector<Foo> vectorOfFoos;
				vectorOfFoos.PushBack(a);
				vectorOfFoos.PushBack(b);
				vectorOfFoos.Clear();
				Assert::IsTrue(vectorOfFoos.IsEmpty());
			}
		}

		TEST_METHOD(VectorTestBegin)
		{
			{
				Vector<int> vectorOfInts;
				vectorOfInts.PushBack(10);

				Vector<int>::Iterator itr = vectorOfInts.begin();
				Assert::AreEqual(10, *itr);
			}

			{
				int a = 10;
				Vector<int*> vectorOfIntPtrs;
				vectorOfIntPtrs.PushBack(&a);

				Vector<int*>::Iterator itr = vectorOfIntPtrs.begin();
				Assert::AreEqual(&a, *itr);
			}

			{
				Foo a(10);
				Vector<Foo> vectorOfFoos;
				vectorOfFoos.PushBack(a);

				Vector<Foo>::Iterator itr = vectorOfFoos.begin();
				Assert::IsTrue(a == *itr);
			}
		}

		TEST_METHOD(VectorTestEnd)
		{
			{
				Vector<int> vectorOfInts;
				vectorOfInts.PushBack(10);

				Vector<int>::Iterator itr = vectorOfInts.end();
				auto expression = [&] { int a = *itr; UNREFERENCED_PARAMETER(a);  };
				Assert::ExpectException<std::exception>(expression);
			}

			{
				int a = 10;
				Vector<int*> vectorOfIntPtrs;
				vectorOfIntPtrs.PushBack(&a);

				Vector<int*>::Iterator itr = vectorOfIntPtrs.end();
				auto expression = [&] { int* b = *itr; UNREFERENCED_PARAMETER(b);  };
				Assert::ExpectException<std::exception>(expression);
			}

			{
				Foo a(10);
				Vector<Foo> vectorOfFoos;
				vectorOfFoos.PushBack(a);

				Vector<Foo>::Iterator itr = vectorOfFoos.end();
				auto expression = [&] { Foo b = *itr; UNREFERENCED_PARAMETER(b);  };
				Assert::ExpectException<std::exception>(expression);
			}

		}

		TEST_METHOD(VectorTestFind)
		{
			{
				Vector<int> vectorOfInts;
				vectorOfInts.PushBack(10);
				vectorOfInts.PushBack(20);
				vectorOfInts.PushBack(30);

				Vector<int>::Iterator itr = vectorOfInts.Find(20);
				Assert::AreEqual(20, *itr);

				itr = vectorOfInts.Find(40);
				Assert::IsTrue(itr == vectorOfInts.end());
			}

			{
				int a = 10;
				int b = 20;
				int c = 30;
				Vector<int*> vectorOfIntPtrs;
				vectorOfIntPtrs.PushBack(&a);
				vectorOfIntPtrs.PushBack(&b);
				vectorOfIntPtrs.PushBack(&c);

				Vector<int*>::Iterator itr = vectorOfIntPtrs.Find(&b);
				Assert::AreEqual(&b, *itr);

				int d = 40;
				itr = vectorOfIntPtrs.Find(&d);
				Assert::IsTrue(itr == vectorOfIntPtrs.end());
			}

			{
				Foo a(10);
				Foo b(20);
				Foo c(30);
				Vector<Foo> vectorOfFoos;
				vectorOfFoos.PushBack(a);
				vectorOfFoos.PushBack(b);
				vectorOfFoos.PushBack(c);

				Vector<Foo>::Iterator itr = vectorOfFoos.Find(b);
				Assert::IsTrue(b == *itr);

				Foo d(40);
				itr = vectorOfFoos.Find(d);
				Assert::IsTrue(itr == vectorOfFoos.end());
			}
		}

		TEST_METHOD(VectorTestRemove)
		{
			{
				// empty list
				Vector<int> vectorOfInts;
				vectorOfInts.Remove(10);
				Assert::IsTrue(vectorOfInts.IsEmpty());



				// last element
				vectorOfInts.PushBack(10);
				vectorOfInts.Remove(10);
				Assert::IsTrue(vectorOfInts.IsEmpty());

				// remove middle element
				vectorOfInts.PushBack(10);
				vectorOfInts.PushBack(20);
				vectorOfInts.PushBack(30);
				vectorOfInts.Remove(20);

				Assert::AreEqual(2, (int)vectorOfInts.Size());
				Assert::AreEqual(10, vectorOfInts.Front());
				Assert::AreEqual(30, vectorOfInts.Back());

				//Vector<int>::Iterator itr;
				//auto expression = [&] { vectorOfInts.Remove(itr); };
				//Assert::ExpectException<std::exception>(expression);

				// remove last element
				vectorOfInts.Remove(30);
				Assert::AreEqual(10, vectorOfInts.Back());
			}

			{
				int a = 10;
				int b = 20;
				int c = 30;

				// empty list
				Vector<int*> vectorOfIntPtrs;
				vectorOfIntPtrs.Remove(&a);
				Assert::IsTrue(vectorOfIntPtrs.IsEmpty());

				// last element
				vectorOfIntPtrs.PushBack(&a);
				vectorOfIntPtrs.Remove(&a);
				Assert::IsTrue(vectorOfIntPtrs.IsEmpty());

				// remove middle element
				vectorOfIntPtrs.PushBack(&a);
				vectorOfIntPtrs.PushBack(&b);
				vectorOfIntPtrs.PushBack(&c);
				vectorOfIntPtrs.Remove(&b);

				Assert::AreEqual(2U, vectorOfIntPtrs.Size());
				Assert::AreEqual(&a, vectorOfIntPtrs.Front());
				Assert::AreEqual(&c, vectorOfIntPtrs.Back());

				// remove last element
				vectorOfIntPtrs.Remove(&c);
				Assert::AreEqual(&a, vectorOfIntPtrs.Back());
			}

			{
				Foo a(10);
				Foo b(20);
				Foo c(30);

				// empty list
				Vector<Foo> vectorOfFoos;
				vectorOfFoos.Remove(a);
				Assert::IsTrue(vectorOfFoos.IsEmpty());

				// last element
				vectorOfFoos.PushBack(a);
				vectorOfFoos.Remove(a);
				Assert::IsTrue(vectorOfFoos.IsEmpty());

				// remove middle element
				vectorOfFoos.PushBack(a);
				vectorOfFoos.PushBack(b);
				vectorOfFoos.PushBack(c);
				vectorOfFoos.Remove(b);

				Assert::AreEqual(2U, vectorOfFoos.Size());
				Assert::IsTrue(a == vectorOfFoos.Front());
				Assert::IsTrue(c == vectorOfFoos.Back());

				// remove last element
				vectorOfFoos.Remove(c);
				Assert::IsTrue(a == vectorOfFoos.Back());
			}
		}

		TEST_METHOD(VectorTestRemoveRange)
		{
			{
				// empty list
				Vector<int> vectorOfInts;
				vectorOfInts.PushBack(10);
				vectorOfInts.PushBack(20);
				vectorOfInts.PushBack(30);
				vectorOfInts.PushBack(40);
				vectorOfInts.PushBack(50);

				vectorOfInts.Remove(vectorOfInts.begin(), vectorOfInts.end());
				Assert::IsTrue(vectorOfInts.IsEmpty());

				vectorOfInts.PushBack(10);
				Vector<int>::Iterator itrBegin = vectorOfInts.PushBack(20);
				vectorOfInts.PushBack(30);
				Vector<int>::Iterator itrEnd = vectorOfInts.PushBack(40);
				vectorOfInts.PushBack(50);
				
				vectorOfInts.Remove(itrBegin, itrEnd);
				Assert::AreEqual(3U, vectorOfInts.Size());
				Assert::AreEqual(40, vectorOfInts[1]);
			}

			{
				int a = 10;
				int b = 20;
				int c = 30;
				int d = 40;
				int e = 50;

				// empty list
				Vector<int*> vectorOfInts;
				vectorOfInts.PushBack(&a);
				vectorOfInts.PushBack(&b);
				vectorOfInts.PushBack(&c);
				vectorOfInts.PushBack(&d);
				vectorOfInts.PushBack(&e);

				vectorOfInts.Remove(vectorOfInts.begin(), vectorOfInts.end());
				Assert::IsTrue(vectorOfInts.IsEmpty());

				vectorOfInts.PushBack(&a);
				Vector<int*>::Iterator itrBegin = vectorOfInts.PushBack(&b);
				vectorOfInts.PushBack(&c);
				Vector<int*>::Iterator itrEnd = vectorOfInts.PushBack(&d);
				vectorOfInts.PushBack(&e);

				vectorOfInts.Remove(itrBegin, itrEnd);
				Assert::AreEqual(3U, vectorOfInts.Size());
				Assert::AreEqual(&d, vectorOfInts[1]);
			}

			{
				Foo a(10);
				Foo b(20);
				Foo c(30);
				Foo d(40);
				Foo e(50);

				// empty list
				Vector<Foo> vectorOfInts;
				vectorOfInts.PushBack(a);
				vectorOfInts.PushBack(b);
				vectorOfInts.PushBack(c);
				vectorOfInts.PushBack(d);
				vectorOfInts.PushBack(e);

				vectorOfInts.Remove(vectorOfInts.begin(), vectorOfInts.end());
				Assert::IsTrue(vectorOfInts.IsEmpty());

				vectorOfInts.PushBack(a);
				Vector<Foo>::Iterator itrBegin = vectorOfInts.PushBack(b);
				vectorOfInts.PushBack(c);
				Vector<Foo>::Iterator itrEnd = vectorOfInts.PushBack(d);
				vectorOfInts.PushBack(e);

				vectorOfInts.Remove(itrBegin, itrEnd);
				Assert::AreEqual(3U, vectorOfInts.Size());
				Assert::IsTrue(d == vectorOfInts[1]);
			}

		}

		TEST_METHOD(VectorTestConstFunctions)
		{
			{
				Vector<int> vectorOfInts;
				vectorOfInts.PushBack(10);
				vectorOfInts.PushBack(20);

				const Vector<int> constVectorOfInts = vectorOfInts;
				Assert::AreEqual(10, constVectorOfInts[0]);
				Assert::AreEqual(10, constVectorOfInts.Front());
				Assert::AreEqual(20, constVectorOfInts.Back());
			}

			{
				int a = 10;
				int b = 20;
				Vector<int*> vectorOfIntPtrs;
				vectorOfIntPtrs.PushBack(&a);
				vectorOfIntPtrs.PushBack(&b);

				const Vector<int*> constVectorOfIntPtrs = vectorOfIntPtrs;
				Assert::AreEqual(&a, constVectorOfIntPtrs[0]);
				Assert::AreEqual(&a, constVectorOfIntPtrs.Front());
				Assert::AreEqual(&b, constVectorOfIntPtrs.Back());
			}

			{
				Foo a(10);
				Foo b(20);
				Vector<Foo> vectorOfFoos;
				vectorOfFoos.PushBack(a);
				vectorOfFoos.PushBack(b);

				const Vector<Foo> constVectorOfFoos = vectorOfFoos;
				Assert::IsTrue(a == constVectorOfFoos[0]);
				Assert::IsTrue(a == constVectorOfFoos.Front());
				Assert::IsTrue(b == constVectorOfFoos.Back());
			}

		}

#pragma endregion

#pragma region IteratorTests

		TEST_METHOD(VectorIteratorTestCtor)
		{
			{
				Vector<int> vectorOfInts;
				Vector<int>::Iterator itr;

				// not attached to a host
				auto expression = [&] { int a = *itr; UNREFERENCED_PARAMETER(a);  };
				Assert::ExpectException<std::exception>(expression);

				// current index set to 0
				itr = vectorOfInts.begin();

				auto expression2 = [&] { int a = *itr; UNREFERENCED_PARAMETER(a);  };
				Assert::ExpectException<std::exception>(expression2);
			}

			{
				Vector<int*> vectorOfIntPtrs;
				Vector<int*>::Iterator itr;

				// not attached to a host
				auto expression = [&] { int* a = *itr; UNREFERENCED_PARAMETER(a);  };
				Assert::ExpectException<std::exception>(expression);

				// current index set to 0
				itr = vectorOfIntPtrs.begin();

				auto expression2 = [&] { int* a = *itr; UNREFERENCED_PARAMETER(a);  };
				Assert::ExpectException<std::exception>(expression2);
			}

			{
				Vector<Foo> vectorOfFoos;
				Vector<Foo>::Iterator itr;

				// not attached to a host
				auto expression = [&] { Foo a = *itr; UNREFERENCED_PARAMETER(a);  };
				Assert::ExpectException<std::exception>(expression);

				// current index set to 0
				itr = vectorOfFoos.begin();

				auto expression2 = [&] { Foo a = *itr; UNREFERENCED_PARAMETER(a);  };
				Assert::ExpectException<std::exception>(expression2);
			}
		}

		TEST_METHOD(VectorIteratorTestCopyCtor)
		{
			{
				Vector<int> vectorOfInts;
				vectorOfInts.PushBack(10);
				Vector<int>::Iterator itr = vectorOfInts.begin();
				Vector<int>::Iterator itr2(itr);

				Assert::IsTrue(itr == itr2);
			}

			{
				int a = 10;
				Vector<int*> vectorOfIntPtrs;
				vectorOfIntPtrs.PushBack(&a);
				Vector<int*>::Iterator itr = vectorOfIntPtrs.begin();
				Vector<int*>::Iterator itr2(itr);

				Assert::IsTrue(itr == itr2);
			}

			{
				Foo a(10);
				Vector<Foo> vectorOfIntPtrs;
				vectorOfIntPtrs.PushBack(a);
				Vector<Foo>::Iterator itr = vectorOfIntPtrs.begin();
				Vector<Foo>::Iterator itr2(itr);

				Assert::IsTrue(itr == itr2);
			}
		}

		TEST_METHOD(VectorIteratorTestMoveCtor)
		{
			{
				Vector<int> vectorOfInts;
				vectorOfInts.PushBack(10);
				Vector<int>::Iterator itr = vectorOfInts.begin();

				Vector<int>::Iterator anotherItr = std::move(itr);
				Assert::AreEqual(10, *anotherItr);

				auto expression = [&] {int a = *itr; UNREFERENCED_PARAMETER(a); };
				Assert::ExpectException<std::exception>(expression);
			}

			{
				int a = 10;
				Vector<int*> vectorOfIntPtrs;
				vectorOfIntPtrs.PushBack(&a);
				Vector<int*>::Iterator itr = vectorOfIntPtrs.begin();

				Vector<int*>::Iterator anotherItr = std::move(itr);
				Assert::AreEqual(&a, *anotherItr);

				auto expression = [&] {int* b = *itr; UNREFERENCED_PARAMETER(b); };
				Assert::ExpectException<std::exception>(expression);
			}

			{
				Foo a(10);
				Vector<Foo> vectorOfFoos;
				vectorOfFoos.PushBack(a);
				Vector<Foo>::Iterator itr = vectorOfFoos.begin();

				Vector<Foo>::Iterator anotherItr = std::move(itr);
				Assert::IsTrue(a == *anotherItr);

				auto expression = [&] { Foo b = *itr; UNREFERENCED_PARAMETER(b); };
				Assert::ExpectException<std::exception>(expression);
			}
		}

		TEST_METHOD(VectorIteratorTestMoveAssigmentOperator)
		{
			{
				Vector<int> vectorOfInts;
				vectorOfInts.PushBack(10);
				Vector<int>::Iterator itr = vectorOfInts.begin();

				Vector<int> anotherVectorOfInts;
				anotherVectorOfInts.PushBack(20);
				Vector<int>::Iterator anotherItr = anotherVectorOfInts.begin();
				anotherItr = std::move(itr);

				Assert::AreEqual(10, *anotherItr);
				auto expression = [&] {int a = *itr; UNREFERENCED_PARAMETER(a); };
				Assert::ExpectException<std::exception>(expression);
			}

			{
				int a = 10;
				Vector<int*> vectorOfIntPtrs;
				vectorOfIntPtrs.PushBack(&a);
				Vector<int*>::Iterator itr = vectorOfIntPtrs.begin();

				int b = 20;
				Vector<int*> anotherVectorOfIntPtrs;
				anotherVectorOfIntPtrs.PushBack(&b);
				Vector<int*>::Iterator anotherItr = anotherVectorOfIntPtrs.begin();
				anotherItr = std::move(itr);
				Assert::AreEqual(&a, *anotherItr);

				auto expression = [&] {int* b = *itr; UNREFERENCED_PARAMETER(b); };
				Assert::ExpectException<std::exception>(expression);
			}

			{
				Foo a(10);
				Vector<Foo> vectorOfFoos;
				vectorOfFoos.PushBack(a);
				Vector<Foo>::Iterator itr = vectorOfFoos.begin();

				Foo b(20);
				Vector<Foo> anotherVectorOfFoos;
				anotherVectorOfFoos.PushBack(b);
				Vector<Foo>::Iterator anotherItr = anotherVectorOfFoos.begin();
				anotherItr = std::move(itr);
				Assert::IsTrue(a == *anotherItr);

				auto expression = [&] { Foo b = *itr; UNREFERENCED_PARAMETER(b); };
				Assert::ExpectException<std::exception>(expression);
			}
		}

		TEST_METHOD(VectorIteratorTestAssignOperator)
		{
			{
				Vector<int> vectorOfInts;
				vectorOfInts.PushBack(10);
				Vector<int>::Iterator itr;
				itr = vectorOfInts.begin();

				Assert::IsTrue(itr == vectorOfInts.begin());
			}

			{
				int a = 10;
				Vector<int*> vectorOfIntPtrs;
				vectorOfIntPtrs.PushBack(&a);
				Vector<int*>::Iterator itr;
				itr = vectorOfIntPtrs.begin();

				Assert::IsTrue(itr == vectorOfIntPtrs.begin());
			}

			{
				Foo a(10);
				Vector<Foo> vectorOfFoos;
				vectorOfFoos.PushBack(a);
				Vector<Foo>::Iterator itr;
				itr = vectorOfFoos.begin();

				Assert::IsTrue(itr == vectorOfFoos.begin());
			}
		}

		TEST_METHOD(VectorIteratorTestEqualityOperator)
		{
			{
				Vector<int> vectorOfInts;
				vectorOfInts.PushBack(10);
				vectorOfInts.PushBack(20);

				Vector<int>::Iterator itr;
				Vector<int>::Iterator itr2;
				Assert::IsTrue(itr == itr2);
				
				itr = vectorOfInts.begin();
				Assert::IsFalse(itr == itr2);

				Vector<int> anotherVectorOfInts;
				anotherVectorOfInts.PushBack(30);
				anotherVectorOfInts.PushBack(40);
				itr2 = anotherVectorOfInts.begin();
				Assert::IsFalse(itr == itr2);

				itr2 = vectorOfInts.begin();
				Assert::IsTrue(itr == itr2);
				Assert::IsFalse(itr != itr2);

				++itr2;
				Assert::IsFalse(itr == itr2);
			}

			{
				int a = 10;
				int b = 20;
				int c = 30;
				int d = 40;
				Vector<int*> vectorOfIntPtrs;
				vectorOfIntPtrs.PushBack(&a);
				vectorOfIntPtrs.PushBack(&b);

				Vector<int*>::Iterator itr;
				Vector<int*>::Iterator itr2;
				Assert::IsTrue(itr == itr2);

				itr = vectorOfIntPtrs.begin();
				Assert::IsFalse(itr == itr2);

				Vector<int*> anotherVectorOfIntPtrs;
				anotherVectorOfIntPtrs.PushBack(&c);
				anotherVectorOfIntPtrs.PushBack(&d);
				itr2 = anotherVectorOfIntPtrs.begin();
				Assert::IsFalse(itr == itr2);

				itr2 = vectorOfIntPtrs.begin();
				Assert::IsTrue(itr == itr2);
				Assert::IsFalse(itr != itr2);

				++itr2;
				Assert::IsFalse(itr == itr2);
			}

			{
				Foo a(10);
				Foo b(20);
				Foo c(30);
				Foo d(40);
				Vector<Foo> vectorOfFoos;
				vectorOfFoos.PushBack(a);
				vectorOfFoos.PushBack(b);

				Vector<Foo>::Iterator itr;
				Vector<Foo>::Iterator itr2;
				Assert::IsFalse(itr != itr2);

				itr = vectorOfFoos.begin();
				Assert::IsFalse(itr == itr2);

				Vector<Foo> anotherVectorOfFoos;
				anotherVectorOfFoos.PushBack(c);
				anotherVectorOfFoos.PushBack(d);
				itr2 = anotherVectorOfFoos.begin();
				Assert::IsFalse(itr == itr2);

				itr2 = vectorOfFoos.begin();
				Assert::IsTrue(itr == itr2);
				Assert::IsFalse(itr != itr2);

				++itr2;
				Assert::IsFalse(itr == itr2);
			}
		}

		TEST_METHOD(VectorIteratorTestPrefixIncrementOperator)
		{
			{
				Vector<int> vectorOfInts;
				Vector<int>::Iterator itr;

				auto expression = [&] {++itr; };
				Assert::ExpectException<std::exception>(expression);

				itr = vectorOfInts.begin();
				auto expression2 = [&] {++itr; };
				Assert::ExpectException<std::exception>(expression2);

				itr = vectorOfInts.end();
				auto expression3 = [&] {++itr; };
				Assert::ExpectException<std::exception>(expression3);

				vectorOfInts.PushBack(10);
				vectorOfInts.PushBack(20);
				itr = vectorOfInts.begin();
				++itr;
				Assert::AreEqual(20, (int)vectorOfInts[1]);
			}

			{
				Vector<int*> vectorOfIntPtrs;
				Vector<int*>::Iterator itr;

				auto expression = [&] {++itr; };
				Assert::ExpectException<std::exception>(expression);

				itr = vectorOfIntPtrs.begin();
				auto expression2 = [&] {++itr; };
				Assert::ExpectException<std::exception>(expression2);

				itr = vectorOfIntPtrs.end();
				auto expression3 = [&] {++itr; };
				Assert::ExpectException<std::exception>(expression3);

				int a = 10;
				int b = 20;
				vectorOfIntPtrs.PushBack(&a);
				vectorOfIntPtrs.PushBack(&b);
				itr = vectorOfIntPtrs.begin();
				++itr;
				Assert::AreEqual(&b, vectorOfIntPtrs[1]);
			}

			{
				Vector<Foo> vectorOfFoos;
				Vector<Foo>::Iterator itr;

				auto expression = [&] {++itr; };
				Assert::ExpectException<std::exception>(expression);

				itr = vectorOfFoos.begin();
				auto expression2 = [&] {++itr; };
				Assert::ExpectException<std::exception>(expression2);

				itr = vectorOfFoos.end();
				auto expression3 = [&] {++itr; };
				Assert::ExpectException<std::exception>(expression3);

				Foo a(10);
				Foo b(20);
				vectorOfFoos.PushBack(a);
				vectorOfFoos.PushBack(b);
				itr = vectorOfFoos.begin();
				++itr;
				Assert::IsTrue(b == vectorOfFoos[1]);
			}
		}

		TEST_METHOD(VectorIteratorTestPostfixIncrementOperator)
		{
			{
				Vector<int> vectorOfInts;
				Vector<int>::Iterator itr;

				auto expression = [&] {itr++; };
				Assert::ExpectException<std::exception>(expression);

				itr = vectorOfInts.begin();
				auto expression2 = [&] {itr++; };
				Assert::ExpectException<std::exception>(expression2);

				itr = vectorOfInts.end();
				auto expression3 = [&] {itr++; };
				Assert::ExpectException<std::exception>(expression3);

				vectorOfInts.PushBack(10);
				vectorOfInts.PushBack(20);
				itr = vectorOfInts.begin();

				int a = *(itr++);
				Assert::AreEqual(10, a);
				Assert::AreEqual(20, *itr);
			}

			{
				Vector<int*> vectorOfIntPtrs;
				Vector<int*>::Iterator itr;

				auto expression = [&] {itr++; };
				Assert::ExpectException<std::exception>(expression);

				itr = vectorOfIntPtrs.begin();
				auto expression2 = [&] {itr++; };
				Assert::ExpectException<std::exception>(expression2);

				itr = vectorOfIntPtrs.end();
				auto expression3 = [&] {itr++; };
				Assert::ExpectException<std::exception>(expression3);

				int a = 10;
				int b = 20;
				vectorOfIntPtrs.PushBack(&a);
				vectorOfIntPtrs.PushBack(&b);
				itr = vectorOfIntPtrs.begin();

				int* c = *(itr++);
				Assert::AreEqual(&a, c);
				Assert::AreEqual(&b, *itr);
			}
			
			{
				Vector<Foo> vectorOfFoos;
				Vector<Foo>::Iterator itr;

				auto expression = [&] {itr++; };
				Assert::ExpectException<std::exception>(expression);

				itr = vectorOfFoos.begin();
				auto expression2 = [&] {itr++; };
				Assert::ExpectException<std::exception>(expression2);

				itr = vectorOfFoos.end();
				auto expression3 = [&] {itr++; };
				Assert::ExpectException<std::exception>(expression3);

				Foo a(10);
				Foo b(20);
				vectorOfFoos.PushBack(a);
				vectorOfFoos.PushBack(b);
				itr = vectorOfFoos.begin();

				Foo c = *(itr++);
				Assert::IsTrue(a == c);
				Assert::IsTrue(b == *itr);
			}

		}

		TEST_METHOD(VectorIteratorTestDereferenceOperator)
		{
			{
				Vector<int> vectorOfInts;
				vectorOfInts.PushBack(10);
				vectorOfInts.PushBack(20);

				Vector<int>::Iterator itr;
				auto expression = [&] {int a = *itr; UNREFERENCED_PARAMETER(a); };
				Assert::ExpectException<std::exception>(expression);

				itr = vectorOfInts.end();
				auto expression2 = [&] {int a = *itr;  UNREFERENCED_PARAMETER(a); };
				Assert::ExpectException<std::exception>(expression2);

				const Vector<int>::Iterator itr2;
				auto expression3 = [&] {const int a = *itr2;  UNREFERENCED_PARAMETER(a); };
				Assert::ExpectException<std::exception>(expression3);

				const Vector<int>::Iterator itr3 = vectorOfInts.end();
				auto expression4 = [&] {const int a = *itr3;  UNREFERENCED_PARAMETER(a); };
				Assert::ExpectException<std::exception>(expression4);
			}

			{
				int a = 10;
				int b = 20;
				Vector<int*> vectorOfInts;
				vectorOfInts.PushBack(&a);
				vectorOfInts.PushBack(&b);

				Vector<int*>::Iterator itr;
				auto expression = [&] {int* a = *itr; UNREFERENCED_PARAMETER(a); };
				Assert::ExpectException<std::exception>(expression);

				itr = vectorOfInts.end();
				auto expression2 = [&] {int* a = *itr;  UNREFERENCED_PARAMETER(a); };
				Assert::ExpectException<std::exception>(expression2);

				const Vector<int*>::Iterator itr2;
				auto expression3 = [&] {const int* a = *itr2;  UNREFERENCED_PARAMETER(a); };
				Assert::ExpectException<std::exception>(expression3);

				const Vector<int*>::Iterator itr3 = vectorOfInts.end();
				auto expression4 = [&] {const int* a = *itr3;  UNREFERENCED_PARAMETER(a); };
				Assert::ExpectException<std::exception>(expression4);
			}

			{
				Foo a(10);
				Foo b(20);
				Vector<Foo> vectorOfInts;
				vectorOfInts.PushBack(a);
				vectorOfInts.PushBack(b);

				Vector<Foo>::Iterator itr;
				auto expression = [&] {Foo a = *itr; UNREFERENCED_PARAMETER(a); };
				Assert::ExpectException<std::exception>(expression);

				itr = vectorOfInts.end();
				auto expression2 = [&] {Foo a = *itr;  UNREFERENCED_PARAMETER(a); };
				Assert::ExpectException<std::exception>(expression2);

				const Vector<Foo>::Iterator itr2;
				auto expression3 = [&] {const Foo a = *itr2;  UNREFERENCED_PARAMETER(a); };
				Assert::ExpectException<std::exception>(expression3);

				const Vector<Foo>::Iterator itr3 = vectorOfInts.end();
				auto expression4 = [&] {const Foo a = *itr3;  UNREFERENCED_PARAMETER(a); };
				Assert::ExpectException<std::exception>(expression4);
			}
		}

		TEST_METHOD(VectorTestInitList)
		{
			Vector<std::int32_t> vector = { 1, 2, 3 };
			Assert::AreEqual(3U, vector.Size());
			Assert::AreEqual(1, vector[0]);
			Assert::AreEqual(2, vector[1]);
			Assert::AreEqual(3, vector[2]);
		}

#pragma endregion

#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState sStartMemState;
#endif
	};

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState VectorTest::sStartMemState;
#endif
}


