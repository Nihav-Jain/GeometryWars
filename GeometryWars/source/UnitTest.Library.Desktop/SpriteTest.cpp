#include "pch.h"
#include "CppUnitTest.h"

#include "Sprite.h"
#include "Renderer.h"

#include "XmlParseMaster.h"
#include "SharedDataTable.h"
#include "XmlParseHelperNameValue.h"
#include "XmlParseHelperPrimitives.h"
#include "XmlParseHelperSprite.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;

namespace UnitTestLibraryDesktop
{

	TEST_CLASS(SpriteTest)
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
			SharedDataTable::ClearStateGraph();
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

		TEST_METHOD(SpriteTestXML)
		{
			Sprite theSprite;
			SharedDataTable sharedData;
			sharedData.SetRootScope(theSprite);

			XmlParseMaster master(sharedData);
			XmlParseHelperSprite spriteParser;
			XmlParseHelperPrimitives primitivesParser;
			XmlParseHelperNameValue nameValueParser;

			master.AddHelper(spriteParser);
			master.AddHelper(primitivesParser);
			master.AddHelper(nameValueParser);

			Assert::IsTrue(master.ParseFromFile("Content/config/sprite.xml"));

			Sprite* sprite = &theSprite;

			Assert::IsTrue(sprite->GetImagePath() == "Content/resource/mushroom.png");

			glm::vec4 position = glm::vec4(0, 0, 0, 0);

			Assert::IsTrue(sprite->GetPosition() == position);

			glm::vec4 color = glm::vec4(1, 0, 0, 0);

			Assert::IsTrue(sprite->GetColor() == color);

			Library::Renderer render(nullptr);

			render.AddRenderable(sprite);

			render.Update();

		}

#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState sStartMemState;
#endif
	};

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState SpriteTest::sStartMemState;
#endif
}