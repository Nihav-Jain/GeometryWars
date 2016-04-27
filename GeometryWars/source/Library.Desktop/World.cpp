#include "pch.h"
#include "World.h"
#include "ActionList.h"

namespace Library
{
	RTTI_DEFINITIONS(World, Attributed);

	const std::string World::ATTRIBUTE_NAME_SECTOR = "sectors";
	const std::string World::ATTRIBUTE_NAME = "name";
	const std::string World::ATTRIBUTE_BEGIN_PLAY = "beginplay";
	const std::string World::ATTRIBUTE_REACTIONS = "reactions";
	const std::string World::ATTRIBUTE_ON_DESTROY = "on-destroy";

	World::World(const GameTime& gameTime) :
		mName(), mWorldState(gameTime), mEventQueue()
	{
		mWorldState.world = this;

		AddExternalAttribute(ATTRIBUTE_NAME, 1, &mName);
		AddNestedScope(ATTRIBUTE_NAME_SECTOR);
		AddNestedScope(Entity::ATTRIBUTE_ACTIONS);
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
		ScriptedBeginPlay();
		SectorsBeginPlay();
		ActionsBeginPlay();
		ReactionsBeginPlay();
	}

	void World::Update()
	{
		mWorldState.sector = nullptr;
		mWorldState.entity = nullptr;
		mWorldState.action = nullptr;

		mEventQueue.Update(*mWorldState.mGameTime);
		UpdateWorldActions();
		UpdateWorldSectors();
	}

	void World::OnDestroy()
	{
		ScriptedOnDestroy();
		SectorsOnDestroy();
		ActionsOnDestroy();
		ReactionsOnDestroy();
	}

	WorldState& World::GetWorldState()
	{
		return mWorldState;
	}

	EventQueue& World::GetEventQueue()
	{
		return mEventQueue;
	}

	void World::ScriptedBeginPlay()
	{
		Datum* beginPlayDatum = Find(ATTRIBUTE_BEGIN_PLAY);
		if (beginPlayDatum != nullptr && beginPlayDatum->Size() > 0)
		{
			ActionList* beginPlayList = beginPlayDatum->Get<Scope>().AssertiveAs<ActionList>();
			mWorldState.action = beginPlayList;
			beginPlayList->BeginPlay(mWorldState);
			beginPlayList->Update(mWorldState);
		}
	}

	void World::SectorsBeginPlay()
	{
		std::uint32_t i;
		Datum& sectors = Sectors();
		for (i = 0; i < sectors.Size(); i++)
		{
			Sector* sector = sectors.Get<Scope>(i).AssertiveAs<Sector>();
			mWorldState.sector = sector;
			sector->BeginPlay(mWorldState);
		}
		mWorldState.sector = nullptr;
	}

	void World::ActionsBeginPlay()
	{
		std::uint32_t i;
		Datum& actions = Actions();
		for (i = 0; i < actions.Size(); i++)
		{
			Action* action = actions.Get<Scope>(i).AssertiveAs<Action>();
			mWorldState.action = action;
			action->BeginPlay(mWorldState);
		}
	}

	void World::ReactionsBeginPlay()
	{
		std::uint32_t i;
		Datum* reactions = Find(ATTRIBUTE_REACTIONS);
		if (reactions != nullptr)
		{
			for (i = 0; i < reactions->Size(); i++)
			{
				Action* reaction = reactions->Get<Scope>(i).AssertiveAs<Action>();
				mWorldState.action = reaction;
				reaction->BeginPlay(mWorldState);
			}
		}
	}

	void World::ScriptedOnDestroy()
	{
		Datum* onDestroyDatum = Find(ATTRIBUTE_ON_DESTROY);
		if (onDestroyDatum != nullptr && onDestroyDatum->Size() > 0)
		{
			ActionList* onDestroyList = onDestroyDatum->Get<Scope>().AssertiveAs<ActionList>();
			mWorldState.action = onDestroyList;
			onDestroyList->OnDestroy(mWorldState);
		}
	}

	void World::SectorsOnDestroy()
	{
		std::uint32_t i;
		Datum& sectors = Sectors();
		for (i = 0; i < sectors.Size(); i++)
		{
			Sector* sector = sectors.Get<Scope>(i).AssertiveAs<Sector>();
			mWorldState.sector = sector;
			sector->OnDestroy(mWorldState);
		}
		mWorldState.sector = nullptr;
	}

	void World::ActionsOnDestroy()
	{
		std::uint32_t i;
		Datum& actions = Actions();
		for (i = 0; i < actions.Size(); i++)
		{
			Action* action = actions.Get<Scope>(i).AssertiveAs<Action>();
			mWorldState.action = action;
			action->OnDestroy(mWorldState);
		}
	}

	void World::ReactionsOnDestroy()
	{
		std::uint32_t i;
		Datum* reactions = Find(ATTRIBUTE_REACTIONS);
		if (reactions != nullptr)
		{
			for (i = 0; i < reactions->Size(); i++)
			{
				Action* reaction = reactions->Get<Scope>(i).AssertiveAs<Action>();
				mWorldState.action = reaction;
				reaction->OnDestroy(mWorldState);
			}
		}
	}

	void World::UpdateWorldActions()
	{
		std::uint32_t i;
		Datum& actions = Actions();
		for (i = 0; i < actions.Size(); i++)
		{
			Action* action = actions.Get<Scope>(i).AssertiveAs<Action>();
			if ((*action)[Action::ATTRIBUTE_CAN_EVER_TICK].Get<bool>())
			{
				mWorldState.action = action;
				action->Update(mWorldState);
			}
		}
	}

	void World::UpdateWorldSectors()
	{
		std::uint32_t i;
		Datum& sectors = Sectors();
		for (i = 0; i < sectors.Size(); i++)
		{
			Sector* sector = sectors.Get<Scope>(i).AssertiveAs<Sector>();
			mWorldState.sector = sector;
			sector->Update(mWorldState);
		}
	}

}
