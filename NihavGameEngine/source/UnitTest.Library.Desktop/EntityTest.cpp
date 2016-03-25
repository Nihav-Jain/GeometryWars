#include "pch.h"
#include "CppUnitTest.h"
#include "Factory.h"
#include "RTTI.h"
#include "Entity.h"
#include "Sector.h"
#include "World.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;

namespace UnitTestLibraryDesktop
{

	TEST_CLASS(EntityTest)
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

		TEST_METHOD(EntityTestSimple)
		{
			EntityFactory entityFactory;

			std::string worldName = "World";
			World world;
			world.SetName("World");

			Assert::AreEqual(worldName, world.Name());
			Assert::AreEqual(worldName, world["name"].Get<std::string>());

			Sector& sector1 = world.CreateSector("sector1");
			Sector& sector2 = world.CreateSector("sector2");

			Datum& worldSectors = world.Sectors();
			Assert::IsTrue(worldSectors.Get<Scope*>(0) == &sector1);
			Assert::IsTrue(worldSectors.Get<Scope*>(1) == &sector2);
			Assert::IsTrue(sector1.Name() == "sector1");
			Assert::IsTrue(sector2.Name() == "sector2");

			Assert::IsTrue(&world == sector1.GetWorld());

			Entity& entity11 = sector1.CreateEntity("Entity", "entity11");
			Entity& entity12 = sector1.CreateEntity("Entity", "entity12");

			Entity& entity21 = sector2.CreateEntity("Entity", "entity21");
			Entity& entity22 = sector2.CreateEntity("Entity", "entity22");

			Datum& sector1Entities = sector1.Entities();
			Assert::IsTrue(sector1Entities.Get<Scope*>(0) == &entity11);
			Assert::IsTrue(sector1Entities.Get<Scope*>(1) == &entity12);
			Assert::IsTrue(entity11.Name() == "entity11");
			Assert::IsTrue(entity12.Name() == "entity12");

			Datum& sector2Entities = sector2.Entities();
			Assert::IsTrue(sector2Entities.Get<Scope*>(0) == &entity21);
			Assert::IsTrue(sector2Entities.Get<Scope*>(1) == &entity22);
			Assert::IsTrue(entity21.Name() == "entity21");
			Assert::IsTrue(entity22.Name() == "entity22");

			Assert::IsTrue(&sector1 == entity11.GetSector());
			Assert::IsTrue(&sector2 == entity21.GetSector());
		}

#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState sStartMemState;
#endif
	};

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState EntityTest::sStartMemState;
#endif
}