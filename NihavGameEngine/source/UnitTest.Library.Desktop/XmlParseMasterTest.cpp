#include "pch.h"
#include "CppUnitTest.h"

#include "XmlParseMaster.h"
#include "IXmlParseHelper.h"

#include "AngryBirdsSharedData.h"
#include "AngryBirdsXmlParseHelper.h"
#include "XmlParseHelper.h"
#include "TestSharedData.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;

namespace UnitTestLibraryDesktop
{

	TEST_CLASS(XmlParseMasterTest)
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
			AngryBirdsXmlParseHelper::ClearElementParsers();

			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
		}
#endif

		TEST_METHOD(XmlParseMasterTestCtor)
		{
			XmlParseMaster::SharedData sharedData;
			XmlParseMaster parser(sharedData);

			Assert::IsTrue(parser.GetSharedData() == &sharedData);
			Assert::IsTrue(sharedData.GetXmlParseMaster() == &parser);
			Assert::IsTrue(parser.GetFileName().empty());
		}

		TEST_METHOD(XmlParseMasterTestParse)
		{
			const char* xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><angrybirds><birds src=\"Textures/SR_Regular_Birds-1.png\" imageWidth=\"1017\" imageHeight=\"1024\"/></angrybirds>";

			{
				AngryBirdsSharedData angryBirdsData;
				XmlParseMaster angryBirdsParser(angryBirdsData);
				AngryBirdsXmlParseHelper angrybirdsParseHelper;

				angryBirdsParser.AddHelper(angrybirdsParseHelper);
				Assert::IsTrue(angryBirdsParser.Parse(xml, static_cast<std::int32_t>(strlen(xml))));
				Assert::AreEqual(0U, angryBirdsData.Depth());

				Datum* angrybirds = angryBirdsData.mAngryBirdsData.Find("angrybirds");
				Assert::IsNotNull(angrybirds);

				Scope& birds = ((angrybirds->Get<Scope>())["birds"].Get<Scope>());
				Datum* src = birds.Find("src");
				Datum* imageWidth = birds.Find("imageWidth");
				Datum* imageHeight = birds.Find("imageHeight");

				Assert::IsNotNull(src);
				Assert::IsTrue(src->Get<std::string>() == "Textures/SR_Regular_Birds-1.png");
				Assert::IsNotNull(imageWidth);
				Assert::IsNotNull(imageHeight);
				Assert::AreEqual(1017, imageWidth->Get<std::int32_t>());
				Assert::AreEqual(1024, imageHeight->Get<std::int32_t>());

				angryBirdsParser.RemoveHelper(angrybirdsParseHelper);
			}
		}

		TEST_METHOD(XmlParseMasterTestParseFromFile)
		{
			AngryBirdsSharedData angryBirdsData;
			XmlParseMaster angryBirdsParser(angryBirdsData);
			AngryBirdsXmlParseHelper angrybirdsParseHelper;

			angryBirdsParser.AddHelper(angrybirdsParseHelper);
			Assert::IsTrue(angryBirdsParser.ParseFromFile("Content/config/angrybirds_small.xml"));
			Assert::AreEqual(0U, angryBirdsData.Depth());
			Assert::IsTrue(angryBirdsParser.GetFileName() == "Content/config/angrybirds_small.xml");
			{
				Datum* angrybirds = angryBirdsData.mAngryBirdsData.Find("angrybirds");
				Assert::IsNotNull(angrybirds);

				Scope& birds = ((angrybirds->Get<Scope>())["birds"].Get<Scope>());
				Datum* src = birds.Find("src");
				Datum* imageWidth = birds.Find("imageWidth");
				Datum* imageHeight = birds.Find("imageHeight");

				Assert::IsNotNull(src);
				Assert::IsTrue(src->Get<std::string>() == "Textures/SR_Regular_Birds-1.png");
				Assert::IsNotNull(imageWidth);
				Assert::IsNotNull(imageHeight);
				Assert::AreEqual(1017, imageWidth->Get<std::int32_t>());
				Assert::AreEqual(1024, imageHeight->Get<std::int32_t>());
			}

			{
				AngryBirdsSharedData anotherAngryBirdsData;
				XmlParseMaster anotherAngryBirdsParser(angryBirdsData);
				AngryBirdsXmlParseHelper anotherAngrybirdsParseHelper;

				anotherAngryBirdsParser.AddHelper(anotherAngrybirdsParseHelper);
				anotherAngryBirdsParser.SetSharedData(anotherAngryBirdsData);

				Assert::IsTrue(anotherAngryBirdsParser.ParseFromFile("Content/config/angrybirds_small.xml"));
				Assert::AreEqual(0U, anotherAngryBirdsData.Depth());

				Datum* angrybirds = anotherAngryBirdsData.mAngryBirdsData.Find("angrybirds");
				Assert::IsNotNull(angrybirds);

				Scope& birds = ((angrybirds->Get<Scope>())["birds"].Get<Scope>());
				Datum* src = birds.Find("src");
				Datum* imageWidth = birds.Find("imageWidth");
				Datum* imageHeight = birds.Find("imageHeight");

				Assert::IsNotNull(src);
				Assert::IsTrue(src->Get<std::string>() == "Textures/SR_Regular_Birds-1.png");
				Assert::IsNotNull(imageWidth);
				Assert::IsNotNull(imageHeight);
				Assert::AreEqual(1017, imageWidth->Get<std::int32_t>());
				Assert::AreEqual(1024, imageHeight->Get<std::int32_t>());
			}
		}

		TEST_METHOD(XmlParseMasterTestClone)
		{
			XmlParseMaster::SharedData sharedData;
			XmlParseMaster parseMaster(sharedData);
			XmlParseHelper parseHelper;
			parseMaster.AddHelper(parseHelper);

			XmlParseMaster* clonedParseMaster = parseMaster.Clone();
			const char* xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><angrybirds><birds src=\"Textures/SR_Regular_Birds-1.png\" imageWidth=\"1017\" imageHeight=\"1024\"/></angrybirds>";
			Assert::IsTrue(clonedParseMaster->Parse(xml, static_cast<std::int32_t>(strlen(xml))));
			Assert::AreEqual(0U, clonedParseMaster->GetSharedData()->Depth());

			delete clonedParseMaster;

			AngryBirdsSharedData angryBirdsData;
			XmlParseMaster angryBirdsParser(angryBirdsData);
			AngryBirdsXmlParseHelper angrybirdsParseHelper;
			angryBirdsParser.AddHelper(angrybirdsParseHelper);

			clonedParseMaster = angryBirdsParser.Clone();
			Assert::IsTrue(clonedParseMaster->ParseFromFile("Content/config/angrybirds_small.xml"));
			delete clonedParseMaster;
		}

		TEST_METHOD(XmlParseMasterTestCharDataHandler)
		{
			TestSharedData sharedData;
			XmlParseMaster parseMaster(sharedData);
			XmlParseHelper helper;
			parseMaster.AddHelper(helper);

			const char* xml = "<first>Char Data</first>";
			Assert::IsTrue(parseMaster.Parse(xml, static_cast<std::int32_t>(strlen(xml))));
			Assert::IsTrue(sharedData.mData == "Char Data");
		}

#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState sStartMemState;
#endif
	};

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState XmlParseMasterTest::sStartMemState;
#endif
}