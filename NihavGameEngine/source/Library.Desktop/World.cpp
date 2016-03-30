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

	World::World(const World& rhs) :
		mName(rhs.mName), mWorldState(rhs.mWorldState), Attributed(rhs)
	{
		mWorldState.world = this;
		(*this)[ATTRIBUTE_NAME].SetStorage(&mName, 1);
	}

	World::World(World&& rhs) :
		mName(std::move(rhs.mName)), mWorldState(std::move(mWorldState)), Attributed(std::move(rhs))
	{
		mWorldState.world = this;
	}

	World& World::operator=(const World& rhs)
	{
		if (this != &rhs)
		{
			mName = rhs.mName;
			mWorldState = rhs.mWorldState;
			Attributed::operator=(rhs);

			(*this)[ATTRIBUTE_NAME].SetStorage(&mName, 1);
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

			(*this)[ATTRIBUTE_NAME].SetStorage(&mName, 1);
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

	Scope* World::Clone(const Scope& rhs) const
	{
		if (!rhs.Is(World::TypeIdClass()))
			throw std::exception("Given Scope reference is not a World.");
		return new World(*(rhs.As<World>()));
	}

	void World::DefinePrescribedAttributes()
	{
		AddExternalAttribute(ATTRIBUTE_NAME, 1, &mName);
		AddNestedScope(ATTRIBUTE_NAME_SECTOR, Scope(), 0);
	}

}
