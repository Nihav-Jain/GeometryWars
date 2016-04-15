#include "pch.h"
#include "World.h"

namespace Library
{
	RTTI_DEFINITIONS(World);

	const std::string World::ATTRIBUTE_NAME_SECTOR = "sectors";
	const std::string World::ATTRIBUTE_NAME = "name";

	World::World(const GameTime& gameTime) :
		mName(), mWorldState(gameTime), mEventQueue()
	{
		mWorldState.world = this;
		AddExternalAttribute(ATTRIBUTE_NAME, 1, &mName);
		AddNestedScope(ATTRIBUTE_NAME_SECTOR);
		AddNestedScope(Entity::ATTRIBUTE_ACTIONS);
	}

	World::~World()
	{
		// debatable
		// mEventQueue.Clear(*mWorldState.mGameTime);
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
			Sector* sector = sectors.Get<Scope>(i).As<Sector>();
			assert(sector != nullptr);
			if (sector->Name() == sectorName)
				return sector;
		}
		return nullptr;
	}

	Datum& World::Actions()
	{
		return operator[](Entity::ATTRIBUTE_ACTIONS);
	}

	const Datum& World::Actions() const
	{
		return *Find(Entity::ATTRIBUTE_ACTIONS);
	}

	Action* World::FindAction(const std::string& actionName) const
	{
		return Action::FindAction(actionName, Actions());
	}

	void World::Update()
	{
		mWorldState.sector = nullptr;
		mWorldState.entity = nullptr;
		mWorldState.action = nullptr;

		mEventQueue.Update(*mWorldState.mGameTime);

		std::uint32_t i;

		Datum& actions = Actions();
		for (i = 0; i < actions.Size(); i++)
		{
			Action* action = actions.Get<Scope>(i).As<Action>();
			assert(action != nullptr);
			mWorldState.action = action;
			action->Update(mWorldState);
		}

		Datum& sectors = Sectors();
		for (i = 0; i < sectors.Size(); i++)
		{
			Sector* sector = sectors.Get<Scope>(i).As<Sector>();
			assert(sector != nullptr);
			mWorldState.sector = sector;
			sector->Update(mWorldState);
		}
	}

	WorldState& World::GetWorldState()
	{
		return mWorldState;
	}

	EventQueue& World::GetEventQueue()
	{
		return mEventQueue;
	}

}
