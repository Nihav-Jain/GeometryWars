#include "pch.h"
#include "World.h"

namespace Library
{
	RTTI_DEFINITIONS(World);

	const std::string World::ATTRIBUTE_NAME_SECTOR = "sectors";
	const std::string World::ATTRIBUTE_NAME = "name";

	World::World(const GameTime& gameTime) :
		mName(), mWorldState(gameTime)
	{
		mWorldState.world = this;
		Populate();
	}

	const std::string& World::Name() const
	{
		return mName;
	}

	void World::SetName(const std::string& name)
	{
		mName = name;
	}

	Datum& World::Sectors()
	{
		return operator[](ATTRIBUTE_NAME_SECTOR);
	}

	const Datum& World::Sectors() const
	{
		return *Find(ATTRIBUTE_NAME_SECTOR);
	}

	Sector& World::CreateSector(const std::string& sectorName)
	{
		Sector* sector = new Sector();
		sector->SetName(sectorName);
		sector->SetWorld(*this);

		return *sector;
	}

	Sector* World::FindSector(const std::string& sectorName) const
	{
		const Datum& sectors = Sectors();
		for (std::uint32_t i = 0; i < sectors.Size(); i++)
		{
			Sector* sector = sectors.Get<Scope*>(i)->As<Sector>();
			assert(sector != nullptr);
			if (sector->Name() == sectorName)
				return sector;
		}
		return nullptr;
	}

	void World::Update()
	{
		mWorldState.sector = nullptr;
		mWorldState.entity = nullptr;
		mWorldState.action = nullptr;

		std::uint32_t i;

		Datum& sectors = Sectors();
		for (i = 0; i < sectors.Size(); i++)
		{
			Sector* sector = sectors.Get<Scope*>(i)->As<Sector>();
			assert(sector != nullptr);
			mWorldState.sector = sector;
			sector->Update(mWorldState);
		}
	}

	void World::DefinePrescribedAttributes()
	{
		AddExternalAttribute(ATTRIBUTE_NAME, 1, &mName);
		AddNestedScope(ATTRIBUTE_NAME_SECTOR, Scope(), 0);
	}

}
