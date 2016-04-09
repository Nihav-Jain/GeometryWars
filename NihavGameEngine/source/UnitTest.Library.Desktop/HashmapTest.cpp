#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;

namespace UnitTestLibraryDesktop
{
	class FooKeyHashFunctor
	{
	public:
		std::uint32_t operator()(const Foo& index) const
		{
			std::uint32_t hash = static_cast<std::uint32_t>(index.GetData() + *(index.GetPointerData()));
			return hash;
		}
	};


	TEST_CLASS(HashmapTest)
	{
	public:
		
		typedef Hashmap<std::string, int*> HashStringIntPtr;
		typedef Hashmap<std::string, Foo> HashStringFoo;
		
		typedef Hashmap<int, int> HashIntInt;
		typedef Hashmap<int*, int> HashIntPtrInt;
		typedef Hashmap<std::string, int> HashStringInt;
		typedef Hashmap<char*, int> HashCharPtrInt;
		typedef Hashmap<Foo, int> HashFooInt;

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

		TEST_METHOD(HashmapTestCtor)
		{
			{
				HashStringInt mapOfInts(10);
				Assert::AreEqual(0U, mapOfInts.Size());
			}

			{
				HashStringIntPtr mapOfIntPtr(10);
				Assert::AreEqual(0U, mapOfIntPtr.Size());
			}

			{
				HashStringFoo mapOfFoo(10);
				Assert::AreEqual(0U, mapOfFoo.Size());
			}

			{
				HashIntInt mapOfInts(10);
				Assert::AreEqual(0U, mapOfInts.Size());
			}

			{
				HashIntPtrInt mapOfIntPtr(10);
				Assert::AreEqual(0U, mapOfIntPtr.Size());
			}

			{
				HashCharPtrInt mapOfFoo(10);
				Assert::AreEqual(0U, mapOfFoo.Size());
			}

			{
				HashFooInt mapOfFoo(10);
				Assert::AreEqual(0U, mapOfFoo.Size());
			}
		}

		TEST_METHOD(HashmapTestCopyCtor)
		{
			{
				HashStringInt mapOfInts(10);
				HashStringInt::Iterator itr = mapOfInts.Insert(HashStringInt::PairType("ten", 10));
				itr = mapOfInts.Insert(HashStringInt::PairType("twenty", 20));

				HashStringInt anotherMapOfInts(mapOfInts);
				Assert::AreEqual(2U, anotherMapOfInts.Size());
				Assert::AreEqual(10, anotherMapOfInts["ten"]);
				Assert::AreEqual(20, anotherMapOfInts["twenty"]);
			}

			{
				int a = 10;
				int b = 20;
				HashStringIntPtr mapOfIntPtrs(10);
				HashStringIntPtr::Iterator itr = mapOfIntPtrs.Insert(HashStringIntPtr::PairType("ten", &a));
				itr = mapOfIntPtrs.Insert(HashStringIntPtr::PairType("twenty", &b));

				HashStringIntPtr anotherMapOfIntPtrs(mapOfIntPtrs);
				Assert::AreEqual(2U, anotherMapOfIntPtrs.Size());
				Assert::AreEqual(&a, anotherMapOfIntPtrs["ten"]);
				Assert::AreEqual(&b, anotherMapOfIntPtrs["twenty"]);
			}

			{
				Foo a(10);
				Foo b(20);
				HashStringFoo mapOfFoos(10);
				HashStringFoo::Iterator itr = mapOfFoos.Insert(HashStringFoo::PairType("ten", a));
				itr = mapOfFoos.Insert(HashStringFoo::PairType("twenty", b));

				HashStringFoo anotherMapOfFoos(mapOfFoos);
				Assert::AreEqual(2U, anotherMapOfFoos.Size());
				Assert::IsTrue(a == anotherMapOfFoos["ten"]);
				Assert::IsTrue(b == anotherMapOfFoos["twenty"]);
			}

			{
				int a = 10;
				int b = 20;
				HashIntInt mapOfIntPtrs(10);
				HashIntInt::Iterator itr = mapOfIntPtrs.Insert(HashIntInt::PairType(a, a));
				itr = mapOfIntPtrs.Insert(HashIntInt::PairType(b, b));

				HashIntInt anotherMapOfIntPtrs(mapOfIntPtrs);
				Assert::AreEqual(2U, anotherMapOfIntPtrs.Size());
				Assert::AreEqual(a, anotherMapOfIntPtrs[a]);
				Assert::AreEqual(b, anotherMapOfIntPtrs[b]);
			}

			{
				int a = 10;
				int b = 20;
				HashIntPtrInt mapOfIntPtrs(10);
				HashIntPtrInt::Iterator itr = mapOfIntPtrs.Insert(HashIntPtrInt::PairType(&a, a));
				itr = mapOfIntPtrs.Insert(HashIntPtrInt::PairType(&b, b));

				HashIntPtrInt anotherMapOfIntPtrs(mapOfIntPtrs);
				Assert::AreEqual(2U, anotherMapOfIntPtrs.Size());
				Assert::AreEqual(a, anotherMapOfIntPtrs[&a]);
				Assert::AreEqual(b, anotherMapOfIntPtrs[&b]);
			}

			{
				int a = 10;
				int b = 20;
				char* key1 = "ten";
				char* key2 = "twenty";
				HashCharPtrInt mapOfIntPtrs(10);
				HashCharPtrInt::Iterator itr = mapOfIntPtrs.Insert(HashCharPtrInt::PairType(key1, a));
				itr = mapOfIntPtrs.Insert(HashCharPtrInt::PairType(key2, b));

				HashCharPtrInt anotherMapOfIntPtrs(mapOfIntPtrs);
				Assert::AreEqual(2U, anotherMapOfIntPtrs.Size());
				Assert::AreEqual(a, anotherMapOfIntPtrs[key1]);
				Assert::AreEqual(b, anotherMapOfIntPtrs[key2]);
			}

			{
				int a = 10;
				int b = 20;
				Foo key1(10);
				Foo key2(20);
				HashFooInt mapOfIntPtrs(10);
				HashFooInt::Iterator itr = mapOfIntPtrs.Insert(HashFooInt::PairType(key1, a));
				itr = mapOfIntPtrs.Insert(HashFooInt::PairType(key2, b));

				HashFooInt anotherMapOfIntPtrs(mapOfIntPtrs);
				Assert::AreEqual(2U, anotherMapOfIntPtrs.Size());
				Assert::AreEqual(a, anotherMapOfIntPtrs[key1]);
				Assert::AreEqual(b, anotherMapOfIntPtrs[key2]);
			}
		}

		TEST_METHOD(HashmapTestAssignOperator)
		{
			{
				HashStringInt mapOfInts(10);
				HashStringInt::Iterator itr = mapOfInts.Insert(HashStringInt::PairType("ten", 10));
				itr = mapOfInts.Insert(HashStringInt::PairType("twenty", 20));

				HashStringInt anotherMapOfInts(20);
				anotherMapOfInts = mapOfInts;
				Assert::AreEqual(2U, anotherMapOfInts.Size());
				Assert::AreEqual(10, anotherMapOfInts["ten"]);
				Assert::AreEqual(20, anotherMapOfInts["twenty"]);
			}

			{
				int a = 10;
				int b = 20;
				HashStringIntPtr mapOfIntPtrs(10);
				HashStringIntPtr::Iterator itr = mapOfIntPtrs.Insert(HashStringIntPtr::PairType("ten", &a));
				itr = mapOfIntPtrs.Insert(HashStringIntPtr::PairType("twenty", &b));

				HashStringIntPtr anotherMapOfIntPtrs(20);
				anotherMapOfIntPtrs = (mapOfIntPtrs);
				Assert::AreEqual(2U, anotherMapOfIntPtrs.Size());
				Assert::AreEqual(&a, anotherMapOfIntPtrs["ten"]);
				Assert::AreEqual(&b, anotherMapOfIntPtrs["twenty"]);
			}

			{
				Foo a(10);
				Foo b(20);
				HashStringFoo mapOfFoos(10);
				HashStringFoo::Iterator itr = mapOfFoos.Insert(HashStringFoo::PairType("ten", a));
				itr = mapOfFoos.Insert(HashStringFoo::PairType("twenty", b));

				HashStringFoo anotherMapOfFoos(20);
				anotherMapOfFoos = (mapOfFoos);
				Assert::AreEqual(2U, anotherMapOfFoos.Size());
				Assert::IsTrue(a == anotherMapOfFoos["ten"]);
				Assert::IsTrue(b == anotherMapOfFoos["twenty"]);
			}

			{
				int a = 10;
				int b = 20;
				HashIntInt mapOfIntPtrs(10);
				HashIntInt::Iterator itr = mapOfIntPtrs.Insert(HashIntInt::PairType(a, a));
				itr = mapOfIntPtrs.Insert(HashIntInt::PairType(b, b));

				HashIntInt anotherMapOfIntPtrs(20);
				anotherMapOfIntPtrs = (mapOfIntPtrs);
				Assert::AreEqual(2U, anotherMapOfIntPtrs.Size());
				Assert::AreEqual(a, anotherMapOfIntPtrs[a]);
				Assert::AreEqual(b, anotherMapOfIntPtrs[b]);
			}

			{
				int a = 10;
				int b = 20;
				HashIntPtrInt mapOfIntPtrs(10);
				HashIntPtrInt::Iterator itr = mapOfIntPtrs.Insert(HashIntPtrInt::PairType(&a, a));
				itr = mapOfIntPtrs.Insert(HashIntPtrInt::PairType(&b, b));

				HashIntPtrInt anotherMapOfIntPtrs(20);
				anotherMapOfIntPtrs = (mapOfIntPtrs);
				Assert::AreEqual(2U, anotherMapOfIntPtrs.Size());
				Assert::AreEqual(a, anotherMapOfIntPtrs[&a]);
				Assert::AreEqual(b, anotherMapOfIntPtrs[&b]);
			}

			{
				int a = 10;
				int b = 20;
				char* key1 = "ten";
				char* key2 = "twenty";
				HashCharPtrInt mapOfIntPtrs(10);
				HashCharPtrInt::Iterator itr = mapOfIntPtrs.Insert(HashCharPtrInt::PairType(key1, a));
				itr = mapOfIntPtrs.Insert(HashCharPtrInt::PairType(key2, b));

				HashCharPtrInt anotherMapOfIntPtrs(20);
				anotherMapOfIntPtrs = (mapOfIntPtrs);
				Assert::AreEqual(2U, anotherMapOfIntPtrs.Size());
				Assert::AreEqual(a, anotherMapOfIntPtrs[key1]);
				Assert::AreEqual(b, anotherMapOfIntPtrs[key2]);
			}

			{
				int a = 10;
				int b = 20;
				Foo key1(10);
				Foo key2(20);
				HashFooInt mapOfIntPtrs(10);
				HashFooInt::Iterator itr = mapOfIntPtrs.Insert(HashFooInt::PairType(key1, a));
				itr = mapOfIntPtrs.Insert(HashFooInt::PairType(key2, b));

				HashFooInt anotherMapOfIntPtrs(10);
				anotherMapOfIntPtrs = (mapOfIntPtrs);
				Assert::AreEqual(2U, anotherMapOfIntPtrs.Size());
				Assert::AreEqual(a, anotherMapOfIntPtrs[key1]);
				Assert::AreEqual(b, anotherMapOfIntPtrs[key2]);
			}
		}

		TEST_METHOD(HashmapTestMoveCtor)
		{
			{
				HashStringInt mapOfInts(10);
				HashStringInt::Iterator itr = mapOfInts.Insert(HashStringInt::PairType("ten", 10));
				itr = mapOfInts.Insert(HashStringInt::PairType("twenty", 20));

				HashStringInt anotherMapOfInts(std::move(mapOfInts));
				Assert::AreEqual(2U, anotherMapOfInts.Size());
				Assert::AreEqual(10, anotherMapOfInts["ten"]);
				Assert::AreEqual(20, anotherMapOfInts["twenty"]);

				Assert::AreEqual(0U, mapOfInts.Size());
			}

			{
				int a = 10;
				int b = 20;
				HashStringIntPtr mapOfIntPtrs(10);
				HashStringIntPtr::Iterator itr = mapOfIntPtrs.Insert(HashStringIntPtr::PairType("ten", &a));
				itr = mapOfIntPtrs.Insert(HashStringIntPtr::PairType("twenty", &b));

				HashStringIntPtr anotherMapOfIntPtrs(std::move(mapOfIntPtrs));
				Assert::AreEqual(2U, anotherMapOfIntPtrs.Size());
				Assert::AreEqual(&a, anotherMapOfIntPtrs["ten"]);
				Assert::AreEqual(&b, anotherMapOfIntPtrs["twenty"]);

				Assert::AreEqual(0U, mapOfIntPtrs.Size());
			}

			{
				Foo a(10);
				Foo b(20);
				HashStringFoo mapOfFoos(10);
				HashStringFoo::Iterator itr = mapOfFoos.Insert(HashStringFoo::PairType("ten", a));
				itr = mapOfFoos.Insert(HashStringFoo::PairType("twenty", b));

				HashStringFoo anotherMapOfFoos(std::move(mapOfFoos));
				Assert::AreEqual(2U, anotherMapOfFoos.Size());
				Assert::IsTrue(a == anotherMapOfFoos["ten"]);
				Assert::IsTrue(b == anotherMapOfFoos["twenty"]);

				Assert::AreEqual(0U, mapOfFoos.Size());
			}

			{
				HashIntInt mapOfInts(10);
				HashIntInt::Iterator itr = mapOfInts.Insert(HashIntInt::PairType(100, 10));
				itr = mapOfInts.Insert(HashIntInt::PairType(200, 20));

				HashIntInt anotherMapOfInts(std::move(mapOfInts));
				Assert::AreEqual(2U, anotherMapOfInts.Size());
				Assert::AreEqual(10, anotherMapOfInts[100]);
				Assert::AreEqual(20, anotherMapOfInts[200]);

				Assert::AreEqual(0U, mapOfInts.Size());
			}

			{
				int a = 10;
				int b = 20;
				HashIntPtrInt mapOfInts(10);
				HashIntPtrInt::Iterator itr = mapOfInts.Insert(HashIntPtrInt::PairType(&a, 10));
				itr = mapOfInts.Insert(HashIntPtrInt::PairType(&b, 20));

				HashIntPtrInt anotherMapOfInts(std::move(mapOfInts));
				Assert::AreEqual(2U, anotherMapOfInts.Size());
				Assert::AreEqual(10, anotherMapOfInts[&a]);
				Assert::AreEqual(20, anotherMapOfInts[&b]);

				Assert::AreEqual(0U, mapOfInts.Size());
			}

			{
				char* key1 = "ten";
				char* key2 = "twenty";
				HashCharPtrInt mapOfInts(10);
				HashCharPtrInt::Iterator itr = mapOfInts.Insert(HashCharPtrInt::PairType(key1, 10));
				itr = mapOfInts.Insert(HashCharPtrInt::PairType(key2, 20));

				HashCharPtrInt anotherMapOfInts(std::move(mapOfInts));
				Assert::AreEqual(2U, anotherMapOfInts.Size());
				Assert::AreEqual(10, anotherMapOfInts[key1]);
				Assert::AreEqual(20, anotherMapOfInts[key2]);

				Assert::AreEqual(0U, mapOfInts.Size());
			}

			{
				Foo key1(10);
				Foo key2(20);
				HashFooInt mapOfInts(10);
				HashFooInt::Iterator itr = mapOfInts.Insert(HashFooInt::PairType(key1, 10));
				itr = mapOfInts.Insert(HashFooInt::PairType(key2, 20));

				HashFooInt anotherMapOfInts(std::move(mapOfInts));
				Assert::AreEqual(2U, anotherMapOfInts.Size());
				Assert::AreEqual(10, anotherMapOfInts[key1]);
				Assert::AreEqual(20, anotherMapOfInts[key2]);

				Assert::AreEqual(0U, mapOfInts.Size());
			}

		}

		TEST_METHOD(HashmapTestMoveAssignOperator)
		{
			{
				HashStringInt mapOfInts(10);
				HashStringInt::Iterator itr = mapOfInts.Insert(HashStringInt::PairType("ten", 10));
				itr = mapOfInts.Insert(HashStringInt::PairType("twenty", 20));

				HashStringInt anotherMapOfInts(10);
				anotherMapOfInts = (std::move(mapOfInts));
				Assert::AreEqual(2U, anotherMapOfInts.Size());
				Assert::AreEqual(10, anotherMapOfInts["ten"]);
				Assert::AreEqual(20, anotherMapOfInts["twenty"]);

				Assert::AreEqual(0U, mapOfInts.Size());
			}

			{
				int a = 10;
				int b = 20;
				HashStringIntPtr mapOfIntPtrs(10);
				HashStringIntPtr::Iterator itr = mapOfIntPtrs.Insert(HashStringIntPtr::PairType("ten", &a));
				itr = mapOfIntPtrs.Insert(HashStringIntPtr::PairType("twenty", &b));

				HashStringIntPtr anotherMapOfIntPtrs(10);
				anotherMapOfIntPtrs = (std::move(mapOfIntPtrs));
				Assert::AreEqual(2U, anotherMapOfIntPtrs.Size());
				Assert::AreEqual(&a, anotherMapOfIntPtrs["ten"]);
				Assert::AreEqual(&b, anotherMapOfIntPtrs["twenty"]);

				Assert::AreEqual(0U, mapOfIntPtrs.Size());
			}

			{
				Foo a(10);
				Foo b(20);
				HashStringFoo mapOfFoos(10);
				HashStringFoo::Iterator itr = mapOfFoos.Insert(HashStringFoo::PairType("ten", a));
				itr = mapOfFoos.Insert(HashStringFoo::PairType("twenty", b));

				HashStringFoo anotherMapOfFoos(10);
				anotherMapOfFoos = (std::move(mapOfFoos));
				Assert::AreEqual(2U, anotherMapOfFoos.Size());
				Assert::IsTrue(a == anotherMapOfFoos["ten"]);
				Assert::IsTrue(b == anotherMapOfFoos["twenty"]);

				Assert::AreEqual(0U, mapOfFoos.Size());
			}

			{
				HashIntInt mapOfInts(10);
				HashIntInt::Iterator itr = mapOfInts.Insert(HashIntInt::PairType(100, 10));
				itr = mapOfInts.Insert(HashIntInt::PairType(200, 20));

				HashIntInt anotherMapOfInts(10);
				anotherMapOfInts = (std::move(mapOfInts));
				Assert::AreEqual(2U, anotherMapOfInts.Size());
				Assert::AreEqual(10, anotherMapOfInts[100]);
				Assert::AreEqual(20, anotherMapOfInts[200]);

				Assert::AreEqual(0U, mapOfInts.Size());
			}

			{
				int a = 10;
				int b = 20;
				HashIntPtrInt mapOfInts(10);
				HashIntPtrInt::Iterator itr = mapOfInts.Insert(HashIntPtrInt::PairType(&a, 10));
				itr = mapOfInts.Insert(HashIntPtrInt::PairType(&b, 20));

				HashIntPtrInt anotherMapOfInts(10);
				anotherMapOfInts = (std::move(mapOfInts));
				Assert::AreEqual(2U, anotherMapOfInts.Size());
				Assert::AreEqual(10, anotherMapOfInts[&a]);
				Assert::AreEqual(20, anotherMapOfInts[&b]);

				Assert::AreEqual(0U, mapOfInts.Size());
			}

			{
				char* key1 = "ten";
				char* key2 = "twenty";
				HashCharPtrInt mapOfInts(10);
				HashCharPtrInt::Iterator itr = mapOfInts.Insert(HashCharPtrInt::PairType(key1, 10));
				itr = mapOfInts.Insert(HashCharPtrInt::PairType(key2, 20));

				HashCharPtrInt anotherMapOfInts(10);
				anotherMapOfInts = (std::move(mapOfInts));
				Assert::AreEqual(2U, anotherMapOfInts.Size());
				Assert::AreEqual(10, anotherMapOfInts[key1]);
				Assert::AreEqual(20, anotherMapOfInts[key2]);

				Assert::AreEqual(0U, mapOfInts.Size());
			}

			{
				Foo key1(10);
				Foo key2(20);
				HashFooInt mapOfInts(10);
				HashFooInt::Iterator itr = mapOfInts.Insert(HashFooInt::PairType(key1, 10));
				itr = mapOfInts.Insert(HashFooInt::PairType(key2, 20));

				HashFooInt anotherMapOfInts(10);
				anotherMapOfInts = (std::move(mapOfInts));
				Assert::AreEqual(2U, anotherMapOfInts.Size());
				Assert::AreEqual(10, anotherMapOfInts[key1]);
				Assert::AreEqual(20, anotherMapOfInts[key2]);

				Assert::AreEqual(0U, mapOfInts.Size());
			}

		}

		TEST_METHOD(HashmapTestInsert)
		{
			{
				HashStringInt mapOfInts(10);
				HashStringInt::Iterator itr = mapOfInts.Insert(HashStringInt::PairType("ten", 10));
				Assert::AreEqual(10, (*itr).second);

				itr = mapOfInts.Insert(HashStringInt::PairType("twenty", 20));
				Assert::AreEqual(20, (*itr).second);

				itr = mapOfInts.Insert(HashStringInt::PairType("ten", 30));
				Assert::AreEqual(10, (*itr).second);

				mapOfInts.LoadFactor();
			}

			{
				int a = 10;
				int b = 20;
				int c = 30;
				HashStringIntPtr mapOfIntPtrs(10);
				HashStringIntPtr::Iterator itr = mapOfIntPtrs.Insert(HashStringIntPtr::PairType("ten", &a));
				Assert::AreEqual(&a, (*itr).second);

				itr = mapOfIntPtrs.Insert(HashStringIntPtr::PairType("twenty", &b));
				Assert::AreEqual(&b, (*itr).second);

				itr = mapOfIntPtrs.Insert(HashStringIntPtr::PairType("ten", &c));
				Assert::AreEqual(&a, (*itr).second);

				mapOfIntPtrs.LoadFactor();
			}

			{
				Foo a(10);
				Foo b(20);
				Foo c(30);
				HashStringFoo mapOfFoos(10);
				HashStringFoo::Iterator itr = mapOfFoos.Insert(HashStringFoo::PairType("ten", a));
				Assert::IsTrue(a == (*itr).second);

				itr = mapOfFoos.Insert(HashStringFoo::PairType("twenty", b));
				Assert::IsTrue(b == (*itr).second);

				itr = mapOfFoos.Insert(HashStringFoo::PairType("ten", c));
				Assert::IsTrue(a == (*itr).second);

				mapOfFoos.LoadFactor();
			}

			{
				HashIntInt mapOfInts(10);
				HashIntInt::Iterator itr = mapOfInts.Insert(HashIntInt::PairType(100, 10));
				Assert::AreEqual(10, (*itr).second);

				itr = mapOfInts.Insert(HashIntInt::PairType(200, 20));
				Assert::AreEqual(20, (*itr).second);

				itr = mapOfInts.Insert(HashIntInt::PairType(100, 30));
				Assert::AreEqual(10, (*itr).second);

				mapOfInts.LoadFactor();
			}

			{
				int a = 10;
				int b = 20;
				HashIntPtrInt mapOfInts(10);
				HashIntPtrInt::Iterator itr = mapOfInts.Insert(HashIntPtrInt::PairType(&a, 10));
				Assert::AreEqual(10, (*itr).second);

				itr = mapOfInts.Insert(HashIntPtrInt::PairType(&b, 20));
				Assert::AreEqual(20, (*itr).second);

				itr = mapOfInts.Insert(HashIntPtrInt::PairType(&a, 30));
				Assert::AreEqual(10, (*itr).second);

				mapOfInts.LoadFactor();
			}

			{
				char* key1 = "ten";
				char* key2 = "twenty";

				HashCharPtrInt mapOfInts(10);
				HashCharPtrInt::Iterator itr = mapOfInts.Insert(HashCharPtrInt::PairType(key1, 10));
				Assert::AreEqual(10, (*itr).second);

				itr = mapOfInts.Insert(HashCharPtrInt::PairType(key2, 20));
				Assert::AreEqual(20, (*itr).second);

				itr = mapOfInts.Insert(HashCharPtrInt::PairType(key1, 30));
				Assert::AreEqual(10, (*itr).second);

				mapOfInts.LoadFactor();
			}

			{
				Foo key1(10);
				Foo key2(20);


				HashFooInt mapOfInts(10);
				HashFooInt::Iterator itr = mapOfInts.Insert(HashFooInt::PairType(key1, 10));
				Assert::AreEqual(10, (*itr).second);

				itr = mapOfInts.Insert(HashFooInt::PairType(key2, 20));
				Assert::AreEqual(20, (*itr).second);

				itr = mapOfInts.Insert(HashFooInt::PairType(key1, 30));
				Assert::AreEqual(10, (*itr).second);

				mapOfInts.LoadFactor();
			}
		}

		TEST_METHOD(HashmapTestFind)
		{
			{
				HashStringInt mapOfInts(10);
				HashStringInt::Iterator itr = mapOfInts.Insert(HashStringInt::PairType("ten", 10));
				itr = mapOfInts.Insert(HashStringInt::PairType("twenty", 20));

				itr = mapOfInts.Find("twenty");
				Assert::AreEqual(20, (*itr).second);

				itr = mapOfInts.Find("thirty");
				Assert::IsTrue(itr == mapOfInts.end());
			}

			{
				int a = 10;
				int b = 20;
				HashStringIntPtr mapOfIntPtrs(10);
				HashStringIntPtr::Iterator itr = mapOfIntPtrs.Insert(HashStringIntPtr::PairType("ten", &a));
				itr = mapOfIntPtrs.Insert(HashStringIntPtr::PairType("twenty", &b));

				itr = mapOfIntPtrs.Find("twenty");
				Assert::AreEqual(&b, (*itr).second);

				itr = mapOfIntPtrs.Find("thirty");
				Assert::IsTrue(itr == mapOfIntPtrs.end());
			}
			
			{
				Foo a(10);
				Foo b(20);
				HashStringFoo mapOfFoos(10);
				HashStringFoo::Iterator itr = mapOfFoos.Insert(HashStringFoo::PairType("ten", a));
				itr = mapOfFoos.Insert(HashStringFoo::PairType("twenty", b));

				itr = mapOfFoos.Find("twenty");
				Assert::IsTrue(b == (*itr).second);

				itr = mapOfFoos.Find("thirty");
				Assert::IsTrue(itr == mapOfFoos.end());
			}

			{
				HashIntInt mapOfInts(10);
				HashIntInt::Iterator itr = mapOfInts.Insert(HashIntInt::PairType(1, 10));
				itr = mapOfInts.Insert(HashIntInt::PairType(2, 20));

				itr = mapOfInts.Find(2);
				Assert::AreEqual(20, (*itr).second);

				itr = mapOfInts.Find(3);
				Assert::IsTrue(itr == mapOfInts.end());
			}

			{
				int a = 10;
				int b = 20;
				int c = 30;
				HashIntPtrInt mapOfIntPtrs(10);
				HashIntPtrInt::Iterator itr = mapOfIntPtrs.Insert(HashIntPtrInt::PairType(&a, a));
				itr = mapOfIntPtrs.Insert(HashIntPtrInt::PairType(&b, b));

				itr = mapOfIntPtrs.Find(&b);
				Assert::AreEqual(b, (*itr).second);

				itr = mapOfIntPtrs.Find(&c);
				Assert::IsTrue(itr == mapOfIntPtrs.end());
			}

			{
				int a = 10;
				int b = 20;

				char* key1 = "ten";
				char* key2 = "twenty";
				char* key3 = "thirty";

				HashCharPtrInt mapOfIntPtrs(10);
				HashCharPtrInt::Iterator itr = mapOfIntPtrs.Insert(HashCharPtrInt::PairType(key1, a));
				itr = mapOfIntPtrs.Insert(HashCharPtrInt::PairType(key2, b));

				itr = mapOfIntPtrs.Find(key2);
				Assert::AreEqual(b, (*itr).second);

				itr = mapOfIntPtrs.Find(key3);
				Assert::IsTrue(itr == mapOfIntPtrs.end());
			}

			{
				Foo a(10);
				Foo b(20);
				Foo c(30);
				HashFooInt mapOfFoos(10);
				HashFooInt::Iterator itr = mapOfFoos.Insert(HashFooInt::PairType(a, 10));
				itr = mapOfFoos.Insert(HashFooInt::PairType(b, 20));

				itr = mapOfFoos.Find(b);
				Assert::IsTrue(b == (*itr).second);

				itr = mapOfFoos.Find(c);
				Assert::IsTrue(itr == mapOfFoos.end());
			}

		}

		TEST_METHOD(HashmapTestIndexOperator)
		{
			{
				HashStringInt mapOfInts(10);
				HashStringInt::Iterator itr = mapOfInts.Insert(HashStringInt::PairType("ten", 10));
				itr = mapOfInts.Insert(HashStringInt::PairType("twenty", 20));
				Assert::AreEqual(10, mapOfInts["ten"]);
				
				mapOfInts["thirty"] = 30;
				Assert::AreEqual(30, mapOfInts["thirty"]);
			}

			{
				int a = 10;
				int b = 20;
				int c = 30;
				HashStringIntPtr mapOfIntPtrs(10);
				HashStringIntPtr::Iterator itr = mapOfIntPtrs.Insert(HashStringIntPtr::PairType("ten", &a));
				itr = mapOfIntPtrs.Insert(HashStringIntPtr::PairType("twenty", &b));
				Assert::AreEqual(&a, mapOfIntPtrs["ten"]);

				mapOfIntPtrs["thirty"] = &c;
				Assert::AreEqual(&c, mapOfIntPtrs["thirty"]);
			}
			
			{
				Foo a(10);
				Foo b(20);
				Foo c(30);
				HashStringFoo mapOfFoos(10);
				HashStringFoo::Iterator itr = mapOfFoos.Insert(HashStringFoo::PairType("ten", a));
				itr = mapOfFoos.Insert(HashStringFoo::PairType("twenty", b));
				Assert::IsTrue(a == mapOfFoos["ten"]);

				mapOfFoos["thirty"] = c;
				Assert::IsTrue(c == mapOfFoos["thirty"]);
			}

			{
				HashIntInt mapOfInts(10);
				HashIntInt::Iterator itr = mapOfInts.Insert(HashIntInt::PairType(10, 10));
				itr = mapOfInts.Insert(HashIntInt::PairType(20, 20));
				Assert::AreEqual(10, mapOfInts[10]);

				mapOfInts[10] = 30;
				Assert::AreEqual(30, mapOfInts[10]);
			}

			{
				int a = 10;
				int b = 20;

				HashIntPtrInt mapOfInts(10);
				HashIntPtrInt::Iterator itr = mapOfInts.Insert(HashIntPtrInt::PairType(&a, 10));
				itr = mapOfInts.Insert(HashIntPtrInt::PairType(&b, 20));
				Assert::AreEqual(10, mapOfInts[&a]);

				mapOfInts[&a] = 30;
				Assert::AreEqual(30, mapOfInts[&a]);
			}

			{
				char* key1 = "ten";
				char* key2 = "twenty";

				HashCharPtrInt mapOfInts(10);
				HashCharPtrInt::Iterator itr = mapOfInts.Insert(HashCharPtrInt::PairType(key1, 10));
				itr = mapOfInts.Insert(HashCharPtrInt::PairType(key2, 20));
				Assert::AreEqual(10, mapOfInts[key1]);

				mapOfInts[key1] = 30;
				Assert::AreEqual(30, mapOfInts[key1]);
			}

			{
				Foo key1(10);
				Foo key2(20);

				HashFooInt mapOfInts(10);
				HashFooInt::Iterator itr = mapOfInts.Insert(HashFooInt::PairType(key1, 10));
				itr = mapOfInts.Insert(HashFooInt::PairType(key2, 20));
				Assert::AreEqual(10, mapOfInts[key1]);

				mapOfInts[key1] = 30;
				Assert::AreEqual(30, mapOfInts[key1]);
			}
		}

		TEST_METHOD(HashmapTestRemove)
		{
			{
				HashStringInt mapOfInts(10);
				HashStringInt::Iterator itr = mapOfInts.Insert(HashStringInt::PairType("ten", 10));
				itr = mapOfInts.Insert(HashStringInt::PairType("twenty", 20));
				mapOfInts.Remove("twenty");

				itr = mapOfInts.Find("twenty");
				Assert::IsTrue(itr == mapOfInts.end());

				mapOfInts.Remove("thirty");
				itr = mapOfInts.Find("thirty");
				Assert::IsTrue(itr == mapOfInts.end());
			}

			{
				int a = 10;
				int b = 20;
				HashStringIntPtr mapOfIntPtrs(10);
				HashStringIntPtr::Iterator itr = mapOfIntPtrs.Insert(HashStringIntPtr::PairType("ten", &a));
				itr = mapOfIntPtrs.Insert(HashStringIntPtr::PairType("twenty", &b));
				mapOfIntPtrs.Remove("twenty");

				itr = mapOfIntPtrs.Find("twenty");
				Assert::IsTrue(itr == mapOfIntPtrs.end());

				mapOfIntPtrs.Remove("thirty");
				itr = mapOfIntPtrs.Find("thirty");
				Assert::IsTrue(itr == mapOfIntPtrs.end());
			}

			{
				Foo a(10);
				Foo b(20);
				HashStringFoo mapOfFoos(10);
				HashStringFoo::Iterator itr = mapOfFoos.Insert(HashStringFoo::PairType("ten", a));
				itr = mapOfFoos.Insert(HashStringFoo::PairType("twenty", b));
				mapOfFoos.Remove("twenty");

				itr = mapOfFoos.Find("twenty");
				Assert::IsTrue(itr == mapOfFoos.end());

				mapOfFoos.Remove("thirty");
				itr = mapOfFoos.Find("thirty");
				Assert::IsTrue(itr == mapOfFoos.end());
			}

			{
				int key1 = 10;
				int key2 = 20;
				int key3 = 30;

				HashIntInt mapOfInts(10);
				HashIntInt::Iterator itr = mapOfInts.Insert(HashIntInt::PairType(key1, 10));
				itr = mapOfInts.Insert(HashIntInt::PairType(key2, 20));
				mapOfInts.Remove(key2);

				itr = mapOfInts.Find(key2);
				Assert::IsTrue(itr == mapOfInts.end());

				mapOfInts.Remove(key3);
				itr = mapOfInts.Find(key3);
				Assert::IsTrue(itr == mapOfInts.end());
			}

			{
				int key1 = 10;
				int key2 = 20;
				int key3 = 30;

				HashIntPtrInt mapOfInts(10);
				HashIntPtrInt::Iterator itr = mapOfInts.Insert(HashIntPtrInt::PairType(&key1, 10));
				itr = mapOfInts.Insert(HashIntPtrInt::PairType(&key2, 20));
				mapOfInts.Remove(&key2);

				itr = mapOfInts.Find(&key2);
				Assert::IsTrue(itr == mapOfInts.end());

				mapOfInts.Remove(&key3);
				itr = mapOfInts.Find(&key3);
				Assert::IsTrue(itr == mapOfInts.end());
			}

			{
				char* key1 = "ten";
				char* key2 = "twenty";
				char* key3 = "thirty";

				HashCharPtrInt mapOfInts(10);
				HashCharPtrInt::Iterator itr = mapOfInts.Insert(HashCharPtrInt::PairType(key1, 10));
				itr = mapOfInts.Insert(HashCharPtrInt::PairType(key2, 20));
				mapOfInts.Remove(key2);

				itr = mapOfInts.Find(key2);
				Assert::IsTrue(itr == mapOfInts.end());

				mapOfInts.Remove(key3);
				itr = mapOfInts.Find(key3);
				Assert::IsTrue(itr == mapOfInts.end());
			}

			{
				Foo key1(10);
				Foo key2(20);
				Foo key3(30);

				HashFooInt mapOfInts(10);
				HashFooInt::Iterator itr = mapOfInts.Insert(HashFooInt::PairType(key1, 10));
				itr = mapOfInts.Insert(HashFooInt::PairType(key2, 20));
				mapOfInts.Remove(key2);

				itr = mapOfInts.Find(key2);
				Assert::IsTrue(itr == mapOfInts.end());

				mapOfInts.Remove(key3);
				itr = mapOfInts.Find(key3);
				Assert::IsTrue(itr == mapOfInts.end());
			}		
		}

		TEST_METHOD(HashmapTestClear)
		{
			{
				HashStringInt mapOfInts(10);
				HashStringInt::Iterator itr = mapOfInts.Insert(HashStringInt::PairType("ten", 10));
				itr = mapOfInts.Insert(HashStringInt::PairType("twenty", 20));

				mapOfInts.Clear();
				Assert::AreEqual(0U, mapOfInts.Size());
			}

			{
				int a = 10;
				int b = 20;
				HashStringIntPtr mapOfIntPtrs(10);
				HashStringIntPtr::Iterator itr = mapOfIntPtrs.Insert(HashStringIntPtr::PairType("ten", &a));
				itr = mapOfIntPtrs.Insert(HashStringIntPtr::PairType("twenty", &b));

				mapOfIntPtrs.Clear();
				Assert::AreEqual(0U, mapOfIntPtrs.Size());
			}

			{
				Foo a(10);
				Foo b(20);
				HashStringFoo mapOfFoos(10);
				HashStringFoo::Iterator itr = mapOfFoos.Insert(HashStringFoo::PairType("ten", a));
				itr = mapOfFoos.Insert(HashStringFoo::PairType("twenty", b));

				mapOfFoos.Clear();
				Assert::AreEqual(0U, mapOfFoos.Size());
			}

			{
				int key1 = 10;
				int key2 = 20;
				HashIntInt mapOfInts(10);
				HashIntInt::Iterator itr = mapOfInts.Insert(HashIntInt::PairType(key1, 10));
				itr = mapOfInts.Insert(HashIntInt::PairType(key2, 20));

				mapOfInts.Clear();
				Assert::AreEqual(0U, mapOfInts.Size());
			}
			{
				int key1 = 10;
				int key2 = 20;
				HashIntPtrInt mapOfInts(10);
				HashIntPtrInt::Iterator itr = mapOfInts.Insert(HashIntPtrInt::PairType(&key1, 10));
				itr = mapOfInts.Insert(HashIntPtrInt::PairType(&key2, 20));

				mapOfInts.Clear();
				Assert::AreEqual(0U, mapOfInts.Size());
			}
			{
				char* key1 = "ten";
				char* key2 = "twenty";

				HashCharPtrInt mapOfInts(10);
				HashCharPtrInt::Iterator itr = mapOfInts.Insert(HashCharPtrInt::PairType(key1, 10));
				itr = mapOfInts.Insert(HashCharPtrInt::PairType(key2, 20));

				mapOfInts.Clear();
				Assert::AreEqual(0U, mapOfInts.Size());
			}
			{
				Foo key1(10);
				Foo key2(20);

				HashFooInt mapOfInts(10);
				HashFooInt::Iterator itr = mapOfInts.Insert(HashFooInt::PairType(key1, 10));
				itr = mapOfInts.Insert(HashFooInt::PairType(key2, 20));

				mapOfInts.Clear();
				Assert::AreEqual(0U, mapOfInts.Size());
			}
		}

		TEST_METHOD(HashmapTestSize)
		{
			{
				HashStringInt mapOfInts(10);
				HashStringInt::Iterator itr = mapOfInts.Insert(HashStringInt::PairType("ten", 10));
				itr = mapOfInts.Insert(HashStringInt::PairType("twenty", 20));

				Assert::AreEqual(2U, mapOfInts.Size());
			}

			{
				int a = 10;
				int b = 20;
				HashStringIntPtr mapOfIntPtrs(10);
				HashStringIntPtr::Iterator itr = mapOfIntPtrs.Insert(HashStringIntPtr::PairType("ten", &a));
				itr = mapOfIntPtrs.Insert(HashStringIntPtr::PairType("twenty", &b));

				Assert::AreEqual(2U, mapOfIntPtrs.Size());
			}

			{
				Foo a(10);
				Foo b(20);
				HashStringFoo mapOfFoos(10);
				HashStringFoo::Iterator itr = mapOfFoos.Insert(HashStringFoo::PairType("ten", a));
				itr = mapOfFoos.Insert(HashStringFoo::PairType("twenty", b));

				Assert::AreEqual(2U, mapOfFoos.Size());
			}
			{
				int key1 = 10;
				int key2 = 20;
				HashIntInt mapOfInts(10);
				HashIntInt::Iterator itr = mapOfInts.Insert(HashIntInt::PairType(key1, 10));
				itr = mapOfInts.Insert(HashIntInt::PairType(key2, 20));

				Assert::AreEqual(2U, mapOfInts.Size());
			}
			{
				int key1 = 10;
				int key2 = 20;
				HashIntPtrInt mapOfInts(10);
				HashIntPtrInt::Iterator itr = mapOfInts.Insert(HashIntPtrInt::PairType(&key1, 10));
				itr = mapOfInts.Insert(HashIntPtrInt::PairType(&key2, 20));

				Assert::AreEqual(2U, mapOfInts.Size());
			}
			{
				char* key1 = "ten";
				char* key2 = "twenty";

				HashCharPtrInt mapOfInts(10);
				HashCharPtrInt::Iterator itr = mapOfInts.Insert(HashCharPtrInt::PairType(key1, 10));
				itr = mapOfInts.Insert(HashCharPtrInt::PairType(key2, 20));

				Assert::AreEqual(2U, mapOfInts.Size());
			}
			{
				Foo key1(10);
				Foo key2(20);


				HashFooInt mapOfInts(10);
				HashFooInt::Iterator itr = mapOfInts.Insert(HashFooInt::PairType(key1, 10));
				itr = mapOfInts.Insert(HashFooInt::PairType(key2, 20));

				Assert::AreEqual(2U, mapOfInts.Size());
			}
		}

		TEST_METHOD(HashmapTestContainsKey)
		{
			{
				HashStringInt mapOfInts(10);
				HashStringInt::Iterator itr = mapOfInts.Insert(HashStringInt::PairType("ten", 10));
				itr = mapOfInts.Insert(HashStringInt::PairType("twenty", 20));

				Assert::IsTrue(mapOfInts.ContainsKey("ten"));
				Assert::IsFalse(mapOfInts.ContainsKey("thirty"));
			}

			{
				int a = 10;
				int b = 20;
				HashStringIntPtr mapOfIntPtrs(10);
				HashStringIntPtr::Iterator itr = mapOfIntPtrs.Insert(HashStringIntPtr::PairType("ten", &a));
				itr = mapOfIntPtrs.Insert(HashStringIntPtr::PairType("twenty", &b));

				Assert::IsTrue(mapOfIntPtrs.ContainsKey("ten"));
				Assert::IsFalse(mapOfIntPtrs.ContainsKey("thirty"));
			}
			
			{
				Foo a(10);
				Foo b(20);
				HashStringFoo mapOfFoos(10);
				HashStringFoo::Iterator itr = mapOfFoos.Insert(HashStringFoo::PairType("ten", a));
				itr = mapOfFoos.Insert(HashStringFoo::PairType("twenty", b));

				Assert::IsTrue(mapOfFoos.ContainsKey("ten"));
				Assert::IsFalse(mapOfFoos.ContainsKey("thirty"));
			}
			{
				int key1 = 10;
				int key2 = 20;
				int key3 = 30;
				HashIntInt mapOfInts(10);
				HashIntInt::Iterator itr = mapOfInts.Insert(HashIntInt::PairType(key1, 10));
				itr = mapOfInts.Insert(HashIntInt::PairType(key2, 20));

				Assert::IsTrue(mapOfInts.ContainsKey(key1));
				Assert::IsFalse(mapOfInts.ContainsKey(key3));
			}
			{
				int key1 = 10;
				int key2 = 20;
				int key3 = 30;
				HashIntPtrInt mapOfInts(10);
				HashIntPtrInt::Iterator itr = mapOfInts.Insert(HashIntPtrInt::PairType(&key1, 10));
				itr = mapOfInts.Insert(HashIntPtrInt::PairType(&key2, 20));

				Assert::IsTrue(mapOfInts.ContainsKey(&key1));
				Assert::IsFalse(mapOfInts.ContainsKey(&key3));
			}
			{
				char* key1 = "ten";
				char* key2 = "twenty";
				char* key3 = "thirty";

				HashCharPtrInt mapOfInts(10);
				HashCharPtrInt::Iterator itr = mapOfInts.Insert(HashCharPtrInt::PairType(key1, 10));
				itr = mapOfInts.Insert(HashCharPtrInt::PairType(key2, 20));

				Assert::IsTrue(mapOfInts.ContainsKey(key1));
				Assert::IsFalse(mapOfInts.ContainsKey(key3));
			}
			{
				Foo key1(10);
				Foo key2(20);
				Foo key3(30);

				HashFooInt mapOfInts(10);
				HashFooInt::Iterator itr = mapOfInts.Insert(HashFooInt::PairType(key1, 10));
				itr = mapOfInts.Insert(HashFooInt::PairType(key2, 20));

				Assert::IsTrue(mapOfInts.ContainsKey(key1));
				Assert::IsFalse(mapOfInts.ContainsKey(key3));
			}

		}

		TEST_METHOD(HashmapTestBegin)
		{
			{
				HashStringInt mapOfInts(10);
				Assert::IsTrue(mapOfInts.end() == mapOfInts.begin());
				HashStringInt::Iterator itr = mapOfInts.Insert(HashStringInt::PairType("ten", 10));
				Assert::IsTrue(itr == mapOfInts.begin());
			}
			
			{
				int a = 10;
				HashStringIntPtr mapOfIntPtrs(10);
				Assert::IsTrue(mapOfIntPtrs.end() == mapOfIntPtrs.begin());
				HashStringIntPtr::Iterator itr = mapOfIntPtrs.Insert(HashStringIntPtr::PairType("ten", &a));
				Assert::IsTrue(itr == mapOfIntPtrs.begin());
			}
			
			{
				Foo a(10);
				HashStringFoo mapOfFoos(10);
				Assert::IsTrue(mapOfFoos.end() == mapOfFoos.begin());
				HashStringFoo::Iterator itr = mapOfFoos.Insert(HashStringFoo::PairType("ten", a));
				Assert::IsTrue(itr == mapOfFoos.begin());
			}
			{
				int key1 = 10;
				HashIntInt mapOfInts(10);
				Assert::IsTrue(mapOfInts.end() == mapOfInts.begin());
				HashIntInt::Iterator itr = mapOfInts.Insert(HashIntInt::PairType(key1, 10));
				Assert::IsTrue(itr == mapOfInts.begin());
			}
			{
				int key1 = 10;
				HashIntPtrInt mapOfInts(10);
				Assert::IsTrue(mapOfInts.end() == mapOfInts.begin());
				HashIntPtrInt::Iterator itr = mapOfInts.Insert(HashIntPtrInt::PairType(&key1, 10));
				Assert::IsTrue(itr == mapOfInts.begin());
			}
			{
				char* key1 = "ten";
				HashCharPtrInt mapOfInts(10);
				Assert::IsTrue(mapOfInts.end() == mapOfInts.begin());
				HashCharPtrInt::Iterator itr = mapOfInts.Insert(HashCharPtrInt::PairType(key1, 10));
				Assert::IsTrue(itr == mapOfInts.begin());
			}
			{
				Foo key1(10);
				HashFooInt mapOfInts(10);
				Assert::IsTrue(mapOfInts.end() == mapOfInts.begin());
				HashFooInt::Iterator itr = mapOfInts.Insert(HashFooInt::PairType(key1, 10));
				Assert::IsTrue(itr == mapOfInts.begin());
			}

		}

		TEST_METHOD(HashmapTestEnd)
		{
			{
				HashStringInt mapOfInts(10);
				HashStringInt::Iterator itr = mapOfInts.end();
				auto expression = [&] {int a = (*itr).second; UNREFERENCED_PARAMETER(a); };
				Assert::ExpectException<std::exception>(expression);
			}

			{
				HashStringIntPtr mapOfIntPtrs(10);
				HashStringIntPtr::Iterator itr = mapOfIntPtrs.end();
				auto expression = [&] {int* a = (*itr).second; UNREFERENCED_PARAMETER(a); };
				Assert::ExpectException<std::exception>(expression);
			}
			
			{
				HashStringFoo mapOfFoos(10);
				HashStringFoo::Iterator itr = mapOfFoos.end();
				auto expression = [&] {Foo a = (*itr).second; UNREFERENCED_PARAMETER(a); };
				Assert::ExpectException<std::exception>(expression);
			}
			{
				HashIntInt mapOfInts(10);
				HashIntInt::Iterator itr = mapOfInts.end();
				auto expression = [&] {int a = (*itr).second; UNREFERENCED_PARAMETER(a); };
				Assert::ExpectException<std::exception>(expression);
			}
			{
				HashIntPtrInt mapOfInts(10);
				HashIntPtrInt::Iterator itr = mapOfInts.end();
				auto expression = [&] {int a = (*itr).second; UNREFERENCED_PARAMETER(a); };
				Assert::ExpectException<std::exception>(expression);
			}
			{
				HashCharPtrInt mapOfInts(10);
				HashCharPtrInt::Iterator itr = mapOfInts.end();
				auto expression = [&] {int a = (*itr).second; UNREFERENCED_PARAMETER(a); };
				Assert::ExpectException<std::exception>(expression);
			}
			{
				HashFooInt mapOfInts(10);
				HashFooInt::Iterator itr = mapOfInts.end();
				auto expression = [&] {int a = (*itr).second; UNREFERENCED_PARAMETER(a); };
				Assert::ExpectException<std::exception>(expression);
			}
		}

		TEST_METHOD(HashmapTestRehash)
		{
			{
				HashStringInt mapOfInts(5);
				mapOfInts.Insert(HashStringInt::PairType("ten", 10));
				mapOfInts.Insert(HashStringInt::PairType("twenty", 20));
				std::float_t oldLoadFactor = mapOfInts.LoadFactor();

				mapOfInts.Rehash(10);
				Assert::AreEqual(10, mapOfInts["ten"]);
				Assert::AreEqual(20, mapOfInts["twenty"]);
				Assert::IsTrue(oldLoadFactor > mapOfInts.LoadFactor());
			}

			{
				int a = 10;
				int b = 20;
				HashStringIntPtr mapOfInts(5);
				mapOfInts.Insert(HashStringIntPtr::PairType("ten", &a));
				mapOfInts.Insert(HashStringIntPtr::PairType("twenty", &b));
				std::float_t oldLoadFactor = mapOfInts.LoadFactor();

				mapOfInts.Rehash(10);
				Assert::AreEqual(&a, mapOfInts["ten"]);
				Assert::AreEqual(&b, mapOfInts["twenty"]);
				Assert::IsTrue(oldLoadFactor > mapOfInts.LoadFactor());
			}

			{
				Foo a(10);
				Foo b(20);
				HashStringFoo mapOfInts(5);
				mapOfInts.Insert(HashStringFoo::PairType("ten", a));
				mapOfInts.Insert(HashStringFoo::PairType("twenty", b));
				std::float_t oldLoadFactor = mapOfInts.LoadFactor();

				mapOfInts.Rehash(10);
				Assert::IsTrue(a == mapOfInts["ten"]);
				Assert::IsTrue(b == mapOfInts["twenty"]);
				Assert::IsTrue(oldLoadFactor > mapOfInts.LoadFactor());
			}

			{
				int key1 = 10;
				int key2 = 20;

				HashIntInt mapOfInts(5);
				mapOfInts.Insert(HashIntInt::PairType(key1, 10));
				mapOfInts.Insert(HashIntInt::PairType(key2, 20));
				std::float_t oldLoadFactor = mapOfInts.LoadFactor();

				mapOfInts.Rehash(10);
				Assert::AreEqual(10, mapOfInts[key1]);
				Assert::AreEqual(20, mapOfInts[key2]);
				Assert::IsTrue(oldLoadFactor > mapOfInts.LoadFactor());
			}

			{
				int key1 = 10;
				int key2 = 20;

				HashIntPtrInt mapOfInts(5);
				mapOfInts.Insert(HashIntPtrInt::PairType(&key1, 10));
				mapOfInts.Insert(HashIntPtrInt::PairType(&key2, 20));
				std::float_t oldLoadFactor = mapOfInts.LoadFactor();

				mapOfInts.Rehash(10);
				Assert::AreEqual(10, mapOfInts[&key1]);
				Assert::AreEqual(20, mapOfInts[&key2]);
				Assert::IsTrue(oldLoadFactor > mapOfInts.LoadFactor());
			}

			{
				char* key1 = "ten";
				char* key2 = "twenty";

				HashCharPtrInt mapOfInts(5);
				mapOfInts.Insert(HashCharPtrInt::PairType(key1, 10));
				mapOfInts.Insert(HashCharPtrInt::PairType(key2, 20));
				std::float_t oldLoadFactor = mapOfInts.LoadFactor();

				mapOfInts.Rehash(10);
				Assert::AreEqual(10, mapOfInts[key1]);
				Assert::AreEqual(20, mapOfInts[key2]);
				Assert::IsTrue(oldLoadFactor > mapOfInts.LoadFactor());
			}

			{
				Foo key1(10);
				Foo key2(20);

				HashFooInt mapOfInts(5);
				mapOfInts.Insert(HashFooInt::PairType(key1, 10));
				mapOfInts.Insert(HashFooInt::PairType(key2, 20));
				std::float_t oldLoadFactor = mapOfInts.LoadFactor();

				mapOfInts.Rehash(10);
				Assert::AreEqual(10, mapOfInts[key1]);
				Assert::AreEqual(20, mapOfInts[key2]);
				Assert::IsTrue(oldLoadFactor > mapOfInts.LoadFactor());
			}
		}
		
		TEST_METHOD(HashmapTestIteratorCtor)
		{
			{
				HashStringInt mapOfInts(10);
				HashStringInt::Iterator itr;
				auto expression = [&] {int a = (*itr).second; UNREFERENCED_PARAMETER(a); };
				Assert::ExpectException<std::exception>(expression);
			}

			{
				HashStringIntPtr mapOfIntPtrs(10);
				HashStringIntPtr::Iterator itr;
				auto expression = [&] {int* a = (*itr).second; UNREFERENCED_PARAMETER(a); };
				Assert::ExpectException<std::exception>(expression);
			}
			
			{
				HashStringFoo mapOfFoos(10);
				HashStringFoo::Iterator itr;
				auto expression = [&] {Foo a = (*itr).second; UNREFERENCED_PARAMETER(a); };
				Assert::ExpectException<std::exception>(expression);
			}

			{
				HashIntInt mapOfInts(10);
				HashIntInt::Iterator itr;
				auto expression = [&] {int a = (*itr).second; UNREFERENCED_PARAMETER(a); };
				Assert::ExpectException<std::exception>(expression);
			}
			
			{
				HashIntPtrInt mapOfInts(10);
				HashIntPtrInt::Iterator itr;
				auto expression = [&] {int a = (*itr).second; UNREFERENCED_PARAMETER(a); };
				Assert::ExpectException<std::exception>(expression);
			}
			
			{
				HashCharPtrInt mapOfInts(10);
				HashCharPtrInt::Iterator itr;
				auto expression = [&] {int a = (*itr).second; UNREFERENCED_PARAMETER(a); };
				Assert::ExpectException<std::exception>(expression);
			}

			{
				HashFooInt mapOfInts(10);
				HashFooInt::Iterator itr;
				auto expression = [&] {int a = (*itr).second; UNREFERENCED_PARAMETER(a); };
				Assert::ExpectException<std::exception>(expression);
			}
		}

		TEST_METHOD(HashmapTestIteratorIncrementOperator)
		{
			{
				HashStringInt mapOfInts(10);
				HashStringInt::Iterator itr;
				
				auto expression = [&] {++itr; };
				Assert::ExpectException<std::exception>(expression);
				
				itr = mapOfInts.end();
				auto expression2 = [&] {++itr; };
				Assert::ExpectException<std::exception>(expression2);

				itr = mapOfInts.Insert(HashStringInt::PairType("ten", 10));
				itr = mapOfInts.Insert(HashStringInt::PairType("twenty", 20));

				std::uint32_t iteratorCount = 0;
				itr = mapOfInts.begin();
				for (; itr != mapOfInts.end(); ++itr)
				{
					++iteratorCount;
				}
				Assert::AreEqual(2U, iteratorCount);				
			}

			{
				HashStringIntPtr mapOfInts(10);
				HashStringIntPtr::Iterator itr;

				auto expression = [&] {++itr; };
				Assert::ExpectException<std::exception>(expression);

				itr = mapOfInts.end();
				auto expression2 = [&] {++itr; };
				Assert::ExpectException<std::exception>(expression2);

				int a = 10;
				int b = 20;

				itr = mapOfInts.Insert(HashStringIntPtr::PairType("ten", &a));
				itr = mapOfInts.Insert(HashStringIntPtr::PairType("twenty", &b));

				std::uint32_t iteratorCount = 0;
				itr = mapOfInts.begin();
				for (; itr != mapOfInts.end(); ++itr)
				{
					++iteratorCount;
				}
				Assert::AreEqual(2U, iteratorCount);
			}

			{
				HashStringFoo mapOfInts(10);
				HashStringFoo::Iterator itr;

				auto expression = [&] {++itr; };
				Assert::ExpectException<std::exception>(expression);

				itr = mapOfInts.end();
				auto expression2 = [&] {++itr; };
				Assert::ExpectException<std::exception>(expression2);

				Foo a(10);
				Foo b(20);

				itr = mapOfInts.Insert(HashStringFoo::PairType("ten", a));
				itr = mapOfInts.Insert(HashStringFoo::PairType("twenty", b));

				std::uint32_t iteratorCount = 0;
				itr = mapOfInts.begin();
				for (; itr != mapOfInts.end(); ++itr)
				{
					++iteratorCount;
				}
				Assert::AreEqual(2U, iteratorCount);
			}

			{
				int key1 = 10;
				int key2 = 20;
				HashIntInt mapOfInts(10);
				HashIntInt::Iterator itr;

				auto expression = [&] {++itr; };
				Assert::ExpectException<std::exception>(expression);

				itr = mapOfInts.end();
				auto expression2 = [&] {++itr; };
				Assert::ExpectException<std::exception>(expression2);

				itr = mapOfInts.Insert(HashIntInt::PairType(key1, 10));
				itr = mapOfInts.Insert(HashIntInt::PairType(key2, 20));

				std::uint32_t iteratorCount = 0;
				itr = mapOfInts.begin();
				for (; itr != mapOfInts.end(); ++itr)
				{
					++iteratorCount;
				}
				Assert::AreEqual(2U, iteratorCount);
			}

			{
				int key1 = 10;
				int key2 = 20;
				HashIntPtrInt mapOfInts(10);
				HashIntPtrInt::Iterator itr;

				auto expression = [&] {++itr; };
				Assert::ExpectException<std::exception>(expression);

				itr = mapOfInts.end();
				auto expression2 = [&] {++itr; };
				Assert::ExpectException<std::exception>(expression2);

				itr = mapOfInts.Insert(HashIntPtrInt::PairType(&key1, 10));
				itr = mapOfInts.Insert(HashIntPtrInt::PairType(&key2, 20));

				std::uint32_t iteratorCount = 0;
				itr = mapOfInts.begin();
				for (; itr != mapOfInts.end(); ++itr)
				{
					++iteratorCount;
				}
				Assert::AreEqual(2U, iteratorCount);
			}

			{
				char* key1 = "ten";
				char* key2 = "twenty";

				HashCharPtrInt mapOfInts(10);
				HashCharPtrInt::Iterator itr;

				auto expression = [&] {++itr; };
				Assert::ExpectException<std::exception>(expression);

				itr = mapOfInts.end();
				auto expression2 = [&] {++itr; };
				Assert::ExpectException<std::exception>(expression2);

				itr = mapOfInts.Insert(HashCharPtrInt::PairType(key1, 10));
				itr = mapOfInts.Insert(HashCharPtrInt::PairType(key2, 20));

				std::uint32_t iteratorCount = 0;
				itr = mapOfInts.begin();
				for (; itr != mapOfInts.end(); ++itr)
				{
					++iteratorCount;
				}
				Assert::AreEqual(2U, iteratorCount);
			}

			{
				Foo key1(10);
				Foo key2(20);

				HashFooInt mapOfInts(10);
				HashFooInt::Iterator itr;

				auto expression = [&] {++itr; };
				Assert::ExpectException<std::exception>(expression);

				itr = mapOfInts.end();
				auto expression2 = [&] {++itr; };
				Assert::ExpectException<std::exception>(expression2);

				itr = mapOfInts.Insert(HashFooInt::PairType(key1, 10));
				itr = mapOfInts.Insert(HashFooInt::PairType(key2, 20));

				std::uint32_t iteratorCount = 0;
				itr = mapOfInts.begin();
				for (; itr != mapOfInts.end(); ++itr)
				{
					++iteratorCount;
				}
				Assert::AreEqual(2U, iteratorCount);
			}
		}

		TEST_METHOD(HashmapTestIteratorEqualityOperator)
		{
			{
				HashStringInt mapOfInts(10);
				Assert::IsTrue(mapOfInts.end() == mapOfInts.begin());
				HashStringInt::Iterator itr = mapOfInts.Insert(HashStringInt::PairType("ten", 10));
				Assert::IsTrue(itr == mapOfInts.begin());
				Assert::IsTrue(itr != mapOfInts.end());
			}

			{
				int a = 10;
				HashStringIntPtr mapOfInts(10);
				Assert::IsTrue(mapOfInts.end() == mapOfInts.begin());
				HashStringIntPtr::Iterator itr = mapOfInts.Insert(HashStringIntPtr::PairType("ten", &a));
				Assert::IsTrue(itr == mapOfInts.begin());
				Assert::IsTrue(itr != mapOfInts.end());
			}

			{
				Foo a(10);
				HashStringFoo mapOfInts(10);
				Assert::IsTrue(mapOfInts.end() == mapOfInts.begin());
				HashStringFoo::Iterator itr = mapOfInts.Insert(HashStringFoo::PairType("ten", a));
				Assert::IsTrue(itr == mapOfInts.begin());
				Assert::IsTrue(itr != mapOfInts.end());
			}

			{
				int key1 = 10;
				HashIntInt mapOfInts(10);
				Assert::IsTrue(mapOfInts.end() == mapOfInts.begin());
				HashIntInt::Iterator itr = mapOfInts.Insert(HashIntInt::PairType(key1, 10));
				Assert::IsTrue(itr == mapOfInts.begin());
				Assert::IsTrue(itr != mapOfInts.end());
			}
			{
				int key1 = 10;
				HashIntPtrInt mapOfInts(10);
				Assert::IsTrue(mapOfInts.end() == mapOfInts.begin());
				HashIntPtrInt::Iterator itr = mapOfInts.Insert(HashIntPtrInt::PairType(&key1, 10));
				Assert::IsTrue(itr == mapOfInts.begin());
				Assert::IsTrue(itr != mapOfInts.end());
			}

			{
				char* key1 = "ten";
				HashCharPtrInt mapOfInts(10);
				Assert::IsTrue(mapOfInts.end() == mapOfInts.begin());
				HashCharPtrInt::Iterator itr = mapOfInts.Insert(HashCharPtrInt::PairType(key1, 10));
				Assert::IsTrue(itr == mapOfInts.begin());
				Assert::IsTrue(itr != mapOfInts.end());
			}

			{
				Foo key1(30);
				HashFooInt mapOfInts(10);
				Assert::IsTrue(mapOfInts.end() == mapOfInts.begin());
				HashFooInt::Iterator itr = mapOfInts.Insert(HashFooInt::PairType(key1, 10));
				Assert::IsTrue(itr == mapOfInts.begin());
				Assert::IsTrue(itr != mapOfInts.end());
			}
		}

		TEST_METHOD(HashmapTestIteratorDereferenceOperator)
		{
			{
				HashStringInt mapOfInts(10);

				HashStringInt::Iterator itr;
				auto expression = [&] {int a = (*itr).second; UNREFERENCED_PARAMETER(a); };
				Assert::ExpectException<std::exception>(expression);

				itr = mapOfInts.end();
				auto expression2 = [&] {int a = (*itr).second; UNREFERENCED_PARAMETER(a); };
				Assert::ExpectException<std::exception>(expression2);

				mapOfInts.Insert(HashStringInt::PairType("ten", 10));
				itr = mapOfInts.Insert(HashStringInt::PairType("twenty", 20));

				Assert::AreEqual(20, (*itr).second);
				(*itr).second = 30;
				Assert::AreEqual(30, (*(mapOfInts.Find("twenty"))).second);
			}

			{
				int a = 10;
				int b = 20;
				int c = 30;
				HashStringIntPtr mapOfInts(10);

				HashStringIntPtr::Iterator itr;
				auto expression = [&] {int* a = (*itr).second; UNREFERENCED_PARAMETER(a); };
				Assert::ExpectException<std::exception>(expression);

				itr = mapOfInts.end();
				auto expression2 = [&] {int* a = (*itr).second; UNREFERENCED_PARAMETER(a); };
				Assert::ExpectException<std::exception>(expression2);

				mapOfInts.Insert(HashStringIntPtr::PairType("ten", &a));
				itr = mapOfInts.Insert(HashStringIntPtr::PairType("twenty", &b));

				Assert::AreEqual(&b, (*itr).second);
				(*itr).second = &c;
				Assert::AreEqual(&c, (*(mapOfInts.Find("twenty"))).second);
			}
			
			{
				Foo a(10);
				Foo b(20);
				Foo c(30);
				HashStringFoo mapOfInts(10);

				HashStringFoo::Iterator itr;
				auto expression = [&] {Foo a = (*itr).second; UNREFERENCED_PARAMETER(a); };
				Assert::ExpectException<std::exception>(expression);

				itr = mapOfInts.end();
				auto expression2 = [&] {Foo a = (*itr).second; UNREFERENCED_PARAMETER(a); };
				Assert::ExpectException<std::exception>(expression2);

				mapOfInts.Insert(HashStringFoo::PairType("ten", a));
				itr = mapOfInts.Insert(HashStringFoo::PairType("twenty", b));

				Assert::IsTrue(b == (*itr).second);
				(*itr).second = c;
				Assert::IsTrue(c == (*(mapOfInts.Find("twenty"))).second);
				Assert::AreEqual(c.GetData(), itr->second.GetData());
			}

			{
				int key1 = 10;
				int key2 = 20;
				HashIntInt mapOfInts(10);

				HashIntInt::Iterator itr;
				auto expression = [&] {int a = (*itr).second; UNREFERENCED_PARAMETER(a); };
				Assert::ExpectException<std::exception>(expression);

				itr = mapOfInts.end();
				auto expression2 = [&] {int a = (*itr).second; UNREFERENCED_PARAMETER(a); };
				Assert::ExpectException<std::exception>(expression2);

				mapOfInts.Insert(HashIntInt::PairType(key1, 10));
				itr = mapOfInts.Insert(HashIntInt::PairType(key2, 20));

				Assert::AreEqual(20, (*itr).second);
				(*itr).second = 30;
				Assert::AreEqual(30, (*(mapOfInts.Find(key2))).second);
			}

			{
				int key1 = 10;
				int key2 = 20;
				HashIntPtrInt mapOfInts(10);

				HashIntPtrInt::Iterator itr;
				auto expression = [&] {int a = (*itr).second; UNREFERENCED_PARAMETER(a); };
				Assert::ExpectException<std::exception>(expression);

				itr = mapOfInts.end();
				auto expression2 = [&] {int a = (*itr).second; UNREFERENCED_PARAMETER(a); };
				Assert::ExpectException<std::exception>(expression2);

				mapOfInts.Insert(HashIntPtrInt::PairType(&key1, 10));
				itr = mapOfInts.Insert(HashIntPtrInt::PairType(&key2, 20));

				Assert::AreEqual(20, (*itr).second);
				(*itr).second = 30;
				Assert::AreEqual(30, (*(mapOfInts.Find(&key2))).second);
			}

			{
				char* key1 = "ten";
				char* key2 = "twenty";

				HashCharPtrInt mapOfInts(10);

				HashCharPtrInt::Iterator itr;
				auto expression = [&] {int a = (*itr).second; UNREFERENCED_PARAMETER(a); };
				Assert::ExpectException<std::exception>(expression);

				itr = mapOfInts.end();
				auto expression2 = [&] {int a = (*itr).second; UNREFERENCED_PARAMETER(a); };
				Assert::ExpectException<std::exception>(expression2);

				mapOfInts.Insert(HashCharPtrInt::PairType(key1, 10));
				itr = mapOfInts.Insert(HashCharPtrInt::PairType(key2, 20));

				Assert::AreEqual(20, (*itr).second);
				(*itr).second = 30;
				Assert::AreEqual(30, (*(mapOfInts.Find(key2))).second);
			}

			{
				Foo key1(10);
				Foo key2(20);


				HashFooInt mapOfInts(10);

				HashFooInt::Iterator itr;
				auto expression = [&] {int a = (*itr).second; UNREFERENCED_PARAMETER(a); };
				Assert::ExpectException<std::exception>(expression);

				itr = mapOfInts.end();
				auto expression2 = [&] {int a = (*itr).second; UNREFERENCED_PARAMETER(a); };
				Assert::ExpectException<std::exception>(expression2);

				mapOfInts.Insert(HashFooInt::PairType(key1, 10));
				itr = mapOfInts.Insert(HashFooInt::PairType(key2, 20));

				Assert::AreEqual(20, (*itr).second);
				(*itr).second = 30;
				Assert::AreEqual(30, (*(mapOfInts.Find(key2))).second);
			}
		}

		TEST_METHOD(HashmapTestIteratorCopyCtor)
		{
			{
				HashStringInt mapOfInts(10);
				mapOfInts.Insert(HashStringInt::PairType("ten", 10));
				mapOfInts.Insert(HashStringInt::PairType("twenty", 20));
				HashStringInt::Iterator itr = mapOfInts.begin();
				HashStringInt::Iterator anotherItr = itr;
				Assert::AreEqual((*itr).second, (*anotherItr).second);
			}

			{
				int a = 10;
				int b = 20;
				HashStringIntPtr mapOfIntPtrs(10);
				mapOfIntPtrs.Insert(HashStringIntPtr::PairType("ten", &a));
				mapOfIntPtrs.Insert(HashStringIntPtr::PairType("twenty", &b));
				HashStringIntPtr::Iterator itr = mapOfIntPtrs.begin();
				HashStringIntPtr::Iterator anotherItr = itr;
				Assert::AreEqual((*itr).second, (*anotherItr).second);
			}

			{
				Foo a(10);
				Foo b(20);
				HashStringFoo mapOfFoos(10);
				mapOfFoos.Insert(HashStringFoo::PairType("ten", a));
				mapOfFoos.Insert(HashStringFoo::PairType("twenty", b));
				HashStringFoo::Iterator itr = mapOfFoos.begin();
				HashStringFoo::Iterator anotherItr = itr;
				Assert::IsTrue((*itr).second == (*anotherItr).second);
			}

			{
				int key1 = 10;
				int key2 = 20;
				HashIntInt mapOfInts(10);
				mapOfInts.Insert(HashIntInt::PairType(key1, 10));
				mapOfInts.Insert(HashIntInt::PairType(key2, 20));
				HashIntInt::Iterator itr = mapOfInts.begin();
				HashIntInt::Iterator anotherItr = itr;
				Assert::AreEqual((*itr).second, (*anotherItr).second);
			}

			{
				int key1 = 10;
				int key2 = 20;
				HashIntPtrInt mapOfInts(10);
				mapOfInts.Insert(HashIntPtrInt::PairType(&key1, 10));
				mapOfInts.Insert(HashIntPtrInt::PairType(&key2, 20));
				HashIntPtrInt::Iterator itr = mapOfInts.begin();
				HashIntPtrInt::Iterator anotherItr = itr;
				Assert::AreEqual((*itr).second, (*anotherItr).second);
			}

			{
				char* key1 = "ten";
				char* key2 = "twenty";

				HashCharPtrInt mapOfInts(10);
				mapOfInts.Insert(HashCharPtrInt::PairType(key1, 10));
				mapOfInts.Insert(HashCharPtrInt::PairType(key2, 20));
				HashCharPtrInt::Iterator itr = mapOfInts.begin();
				HashCharPtrInt::Iterator anotherItr = itr;
				Assert::AreEqual((*itr).second, (*anotherItr).second);
			}

			{
				Foo key1(10);
				Foo key2(20);

				HashFooInt mapOfInts(10);
				mapOfInts.Insert(HashFooInt::PairType(key1, 10));
				mapOfInts.Insert(HashFooInt::PairType(key2, 20));
				HashFooInt::Iterator itr = mapOfInts.begin();
				HashFooInt::Iterator anotherItr = itr;
				Assert::AreEqual((*itr).second, (*anotherItr).second);
			}
		}

		TEST_METHOD(HashmapTestIteratorAssignOperator)
		{
			{
				HashStringInt mapOfInts(10);
				mapOfInts.Insert(HashStringInt::PairType("ten", 10));
				mapOfInts.Insert(HashStringInt::PairType("twenty", 20));
				HashStringInt::Iterator itr = mapOfInts.begin();

				HashStringInt::Iterator anotherItr;
				anotherItr = itr;
				Assert::AreEqual((*itr).second, (*anotherItr).second);
			}

			{
				int a = 10;
				int b = 20;
				HashStringIntPtr mapOfIntPtrs(10);
				mapOfIntPtrs.Insert(HashStringIntPtr::PairType("ten", &a));
				mapOfIntPtrs.Insert(HashStringIntPtr::PairType("twenty", &b));
				HashStringIntPtr::Iterator itr = mapOfIntPtrs.begin();
				HashStringIntPtr::Iterator anotherItr;
				anotherItr = itr;
				Assert::AreEqual((*itr).second, (*anotherItr).second);
			}

			{
				Foo a(10);
				Foo b(20);
				HashStringFoo mapOfFoos(10);
				mapOfFoos.Insert(HashStringFoo::PairType("ten", a));
				mapOfFoos.Insert(HashStringFoo::PairType("twenty", b));
				HashStringFoo::Iterator itr = mapOfFoos.begin();
				HashStringFoo::Iterator anotherItr;
				anotherItr = itr;
				Assert::IsTrue((*itr).second == (*anotherItr).second);
			}

			{
				int key1 = 10;
				int key2 = 20;
				HashIntInt mapOfInts(10);
				mapOfInts.Insert(HashIntInt::PairType(key1, 10));
				mapOfInts.Insert(HashIntInt::PairType(key2, 20));
				HashIntInt::Iterator itr = mapOfInts.begin();
				HashIntInt::Iterator anotherItr;
				anotherItr = itr;
				Assert::AreEqual((*itr).second, (*anotherItr).second);
			}

			{
				int key1 = 10;
				int key2 = 20;
				HashIntPtrInt mapOfInts(10);
				mapOfInts.Insert(HashIntPtrInt::PairType(&key1, 10));
				mapOfInts.Insert(HashIntPtrInt::PairType(&key2, 20));
				HashIntPtrInt::Iterator itr = mapOfInts.begin();
				HashIntPtrInt::Iterator anotherItr;
				anotherItr = itr;
				Assert::AreEqual((*itr).second, (*anotherItr).second);
			}

			{
				char* key1 = "ten";
				char* key2 = "twenty";

				HashCharPtrInt mapOfInts(10);
				mapOfInts.Insert(HashCharPtrInt::PairType(key1, 10));
				mapOfInts.Insert(HashCharPtrInt::PairType(key2, 20));
				HashCharPtrInt::Iterator itr = mapOfInts.begin();
				HashCharPtrInt::Iterator anotherItr;
				anotherItr = itr;
				Assert::AreEqual((*itr).second, (*anotherItr).second);
			}

			{
				Foo key1(10);
				Foo key2(20);

				HashFooInt mapOfInts(10);
				mapOfInts.Insert(HashFooInt::PairType(key1, 10));
				mapOfInts.Insert(HashFooInt::PairType(key2, 20));
				HashFooInt::Iterator itr = mapOfInts.begin();
				HashFooInt::Iterator anotherItr;
				anotherItr = itr;
				Assert::AreEqual((*itr).second, (*anotherItr).second);
			}
		}

		TEST_METHOD(HashmapTestInitList)
		{
			Hashmap<std::string, std::int32_t> map = {
				{"name", 10},
				{"value", 20}
			};

			Assert::AreEqual(2U, map.Size());
			Assert::AreEqual(10, map["name"]);
			Assert::AreEqual(20, map["value"]);
		}


#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState sStartMemState;
#endif

	};

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState HashmapTest::sStartMemState;
#endif
}