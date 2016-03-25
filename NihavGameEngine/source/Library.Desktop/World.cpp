#include "pch.h"
#include "World.h"

namespace Library
{
	RTTI_DEFINITIONS(World);

	World::World() :
		mName(), mWorldState()
	{
		mWorldState.world = this;
		Populate();
	}

	World::World(const World& rhs) :
		mName(rhs.mName), mWorldState(rhs.mWorldState)
	{
		mWorldState.world = this;
		Populate();
	}

	World::World(World&& rhs) :
		mName(std::move(rhs.mName)), mWorldState(std::move(mWorldState)), Attributed(std::move(rhs))
	{
		mWorldState.world = this;
	}

	World::~World()
	{}

	World& World::operator=(const World& rhs)
	{
		if (this != &rhs)
		{
			mName = rhs.mName;
			mWorldState = rhs.mWorldState;
			Attributed::operator=(rhs);

			(*this)[ATTRIBUTE_NAME_NAME].SetStorage(&mName, 1);
			mWorldState.world = this;
		}
		return *this;
	}

	World& World::operator=(World&& rhs)
	{
		if (this != &rhs)
		{
			mName = std::move(rhs.mName);
			mWorldState = std::move(rhs.mWorldState);
			Attributed::operator=(std::move(rhs));

			(*this)[ATTRIBUTE_NAME_NAME].SetStorage(&mName, 1);
			mWorldState.world = this;
		}

		return *this;
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

	Sector& World::CreateSector(const std::string& sectorName)
	{
		Sector* sector = new Sector();
		sector->SetName(sectorName);

		Adopt(ATTRIBUTE_NAME_SECTOR, *sector);

		return *sector;
	}

	void World::Update()
	{
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
		AddExternalAttribute(ATTRIBUTE_NAME_NAME, 1, &mName);
		AddNestedScope(ATTRIBUTE_NAME_SECTOR, Scope(), 0);
	}

}
