#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestLibraryDesktop
{		
	TEST_CLASS(FooTest)
	{
	public:
		
		TEST_METHOD(FooConstructorTest)
		{
			Foo f(10);

			Assert::AreEqual(10, f.GetData());
		}

	};
}