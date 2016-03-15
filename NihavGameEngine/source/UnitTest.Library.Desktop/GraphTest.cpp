#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;

namespace UnitTestLibraryDesktop
{

	enum class GraphTestEnum
	{
		UNKNOWN,
		ENUM1,
		ENUM2,
		ENUM3,
		ENUM4,
		ENUM5,
		END
	};
	TEST_CLASS(GraphTest)
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

		TEST_METHOD(GraphTestTraversal)
		{
			GraphTestEnum enum1 = GraphTestEnum::ENUM1;
			GraphTestEnum enum2 = GraphTestEnum::ENUM2;
			GraphTestEnum enum3 = GraphTestEnum::ENUM3;
			GraphTestEnum enum4 = GraphTestEnum::ENUM4;
			GraphTestEnum enum5 = GraphTestEnum::ENUM5;

			Graph<GraphTestEnum> graph;
			Graph<GraphTestEnum>::Traversor root = graph.AddVertex(enum1);
			Graph<GraphTestEnum>::Traversor rootChild1 = graph.AddVertex(enum2, root);
			Graph<GraphTestEnum>::Traversor rootChild2 = graph.AddVertex(enum3, root);
			Graph<GraphTestEnum>::Traversor child1Child1 = graph.AddVertex(enum4, rootChild2);
			Graph<GraphTestEnum>::Traversor child1Child2 = graph.AddVertex(enum5, child1Child1);

			Graph<GraphTestEnum>::Traversor rootTraversor = graph.Begin();
			Assert::IsTrue(rootTraversor == root);
			Assert::IsTrue(*rootTraversor == GraphTestEnum::ENUM1);
			Assert::IsTrue(rootTraversor.GetCurrentChildVertex() == GraphTestEnum::ENUM2);

			Assert::IsTrue(rootTraversor.TraverseToCurrentChild());
			Assert::IsTrue(*rootTraversor == GraphTestEnum::ENUM2);
			Assert::IsFalse(rootTraversor.TraverseToCurrentChild());

			rootTraversor = graph.Begin();
			Assert::IsTrue(rootTraversor.MoveToNextChild());
			Assert::IsTrue(rootTraversor.GetCurrentChildVertex() == GraphTestEnum::ENUM3);
			Assert::IsTrue(rootTraversor.TraverseToCurrentChild());
			Assert::IsTrue(*rootTraversor == GraphTestEnum::ENUM3);
			Assert::IsTrue(rootTraversor.GetCurrentChildVertex() == GraphTestEnum::ENUM4);
			Assert::IsTrue(rootTraversor.TraverseToCurrentChild());
			Assert::IsTrue(*rootTraversor == GraphTestEnum::ENUM4);
			Assert::IsTrue(rootTraversor.TraverseToCurrentChild());
			Assert::IsTrue(*rootTraversor == GraphTestEnum::ENUM5);
			Assert::IsFalse(rootTraversor.HasMoreChildren());
		}

#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState sStartMemState;
#endif
	};

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState GraphTest::sStartMemState;
#endif
}