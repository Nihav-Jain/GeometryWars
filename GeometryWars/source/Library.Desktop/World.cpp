#include "pch.h"
#include "World.h"
#include "ActionList.h"

namespace Library
{
	RTTI_DEFINITIONS(World);

	const std::string World::ATTRIBUTE_NAME_SECTOR = "sectors";
	const std::string World::ATTRIBUTE_NAME = "name";
	const std::string World::ATTRIBUTE_BEGIN_PLAY = "beginplay";

	World::World(const GameTime& gameTime) :
		mName(), mWorldState(gameTime), mEventQueue()
	{
		mWorldState.world = this;

		AddExternalAttribute(ATTRIBUTE_NAME, 1, &mName);
		AddNestedScope(ATTRIBUTE_NAME_SECTOR);
		AddNestedScope(Entity::ATTRIBUTE_ACTIONS);
		//AddNestedScope(ATTRIBUTE_BEGIN_PLAY);
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

	void World::BeginPlay()
	{
		Datum* beginPlayDatum = Find(ATTRIBUTE_BEGIN_PLAY);
		if (beginPlayDatum!= nullptr && beginPlayDatum->Size() > 0)
		{
			ActionList* beginPlayList = beginPlayDatum->Get<Scope>().AssertiveAs<ActionList>();
			beginPlayList->Update(mWorldState);

			std::uint32_t i;
			Datum& sectors = Sectors();
			for (i = 0; i < sectors.Size(); i++)
			{
				Sector* sector = sectors.Get<Scope>(i).AssertiveAs<Sector>();
				mWorldState.sector = sector;
				sector->BeginPlay(mWorldState);
			}
		}
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
			Action* action = actions.Get<Scope>(i).AssertiveAs<Action>();
			mWorldState.action = action;
			action->Update(mWorldState);
		}

		Datum& sectors = Sectors();
		for (i = 0; i < sectors.Size(); i++)
		{
			Sector* sector = sectors.Get<Scope>(i).AssertiveAs<Sector>();
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
