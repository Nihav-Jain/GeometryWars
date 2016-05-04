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
	const std::string World::ATTRIBUTE_DELTA_TIME = "deltaTime";

	const std::string World::ATTRIBUTE_WIDTH = "width";
	const std::string World::ATTRIBUTE_HEIGHT = "height";

	Stack<Datum*> World::sTempReferenceDatums;

	World::World(const GameTime& gameTime, XmlParseMaster& parseMaster) :
		mName(), mWorldState(gameTime), mEventQueue(), mParseMaster(&parseMaster),
		mWidth(0), mHeight(0), ClassDefinitionContainer(), mActiveSector(nullptr), mLastActiveSector(nullptr),
		SectorDefinitionContainer(nullptr), mSectorToLoad(nullptr)
	{
		mWorldState.world = this;

		AddExternalAttribute(ATTRIBUTE_NAME, 1, &mName);
		AddInternalAttribute(ATTRIBUTE_DELTA_TIME, 0, 1);

		AddExternalAttribute(ATTRIBUTE_WIDTH, 1, &mWidth);
		AddExternalAttribute(ATTRIBUTE_HEIGHT, 1, &mHeight);

		AddNestedScope(ATTRIBUTE_NAME_SECTOR);
		AddNestedScope(Entity::ATTRIBUTE_ACTIONS);
	}

	World::~World()
	{
		while (!sTempReferenceDatums.IsEmpty())
		{
			delete sTempReferenceDatums.Top();
			sTempReferenceDatums.Pop();
		}
		delete SectorDefinitionContainer;
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

		if (mActiveSector == nullptr && Sectors().Size() >= 1U)
		{
			mActiveSector = Sectors().Get<Scope>().AssertiveAs<Sector>();
			mLastActiveSector = mActiveSector;
			mSectorToLoad = nullptr;
		}

		SectorsBeginPlay();
		ActionsBeginPlay();
		ReactionsBeginPlay();
	}

	void World::Update()
	{
		mWorldState.sector = nullptr;
		mWorldState.entity = nullptr;
		mWorldState.action = nullptr;

		(*this)[ATTRIBUTE_DELTA_TIME] = static_cast<std::int32_t>(mWorldState.mGameTime->ElapsedGameTime().count());

		if (mSectorToLoad != nullptr)
		{
			if (mActiveSector != nullptr)
			{
				mWorldState.sector = mActiveSector;
				mActiveSector->OnDestroy(mWorldState);

				if (FindSector(mActiveSector->Name()) == nullptr)
					delete mActiveSector;
			}
			mActiveSector = mSectorToLoad;
			mSectorToLoad = nullptr;
			mWorldState.sector = mActiveSector;
			mWorldState.entity = nullptr;
			mWorldState.action = nullptr;
			mActiveSector->BeginPlay(mWorldState);
		}

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

	bool World::LoadSector(const std::string& sectorName)
	{
		if (SectorDefinitionContainer == nullptr)
			return false;

		Sector* sector = SectorDefinitionContainer->FindSector(sectorName);
		if (sector != nullptr)
		{
			mLastActiveSector = mActiveSector;
			mSectorToLoad = mActiveSector->Clone(*sector)->AssertiveAs<Sector>();
			//mSectorToLoad->SetWorld(*this);
			Adopt(mSectorToLoad->Name(), *mSectorToLoad);
			return true;
		}
		return false;
	}

	Datum* World::ComplexSearch(const std::string& name, const Scope& caller)
	{
		std::string referenceName = name;
		Scope* targetScope = nullptr;

		std::uint32_t pos = (std::uint32_t)referenceName.find('.');
		if (pos > referenceName.length())
			return caller.Search(referenceName);
		else
		{
			targetScope = ComplexSearchHelper(referenceName.substr(0, pos), caller, true);
			if (targetScope == nullptr)
				return nullptr;
			referenceName = referenceName.substr(pos + 1, (std::uint32_t)referenceName.length() - pos);
		}

		while ((pos = (std::uint32_t)referenceName.find('.')) < referenceName.length())
		{
			targetScope = ComplexSearchHelper(referenceName.substr(0, pos), *targetScope);
			if (targetScope == nullptr)
				return nullptr;
			referenceName = referenceName.substr(pos + 1, (std::uint32_t)referenceName.length() - pos);
		}
		Datum* foundDatum = targetScope->Find(referenceName);
		if (foundDatum == nullptr)
		{
			Scope* foundScope = ComplexSearchHelper(referenceName, *targetScope, false);
			if (foundScope != nullptr)
			{
				foundDatum = new Datum();
				*foundDatum = *foundScope;
				sTempReferenceDatums.Push(foundDatum);
			}
		}
		return foundDatum;
	}

	XmlParseMaster& World::ParseMaster()
	{
		return *mParseMaster;
	}

	void World::SetWidth(std::int32_t width)
	{
		mWidth = width;
	}

	void World::SetHeight(std::int32_t height)
	{
		mHeight = height;
	}

	std::int32_t World::GetWidth()
	{
		return mWidth;
	}

	std::int32_t World::GetHeight()
	{
		return mHeight;
	}

	Sector* World::ActiveSector()
	{
		return mActiveSector;
	}

	Scope* World::ComplexSearchHelper(const std::string& name, const Scope& caller, bool doRecursiveSearch)
	{
		Scope* scopeToFind = nullptr;
		Datum* referenceToFind = caller.Find(name);
		if (referenceToFind == nullptr)
		{
			if (caller.Is(Entity::TypeIdClass()))
			{
				Entity& callerEntity = *caller.AssertiveAs<Entity>();
				Action* action = callerEntity.FindAction(name);
				if (action != nullptr)
					scopeToFind = action;
			}
			else if (caller.Is(ActionList::TypeIdClass()))
			{
				ActionList& callerAction = *caller.AssertiveAs<ActionList>();
				Action* action = callerAction.FindAction(name);
				if (action != nullptr)
					scopeToFind = action;
			}
			else if (caller.Is(Sector::TypeIdClass()))
			{
				Sector& callerSector = *caller.AssertiveAs<Sector>();
				Attributed* attribute = callerSector.FindEntity(name);
				if (attribute == nullptr)
					attribute = callerSector.FindAction(name);
				
				// cannot use else here
				if (attribute != nullptr)
					scopeToFind = attribute;
			}
			else if (caller.Is(World::TypeIdClass()))
			{
				World& callerWorld = *caller.AssertiveAs<World>();
				Attributed* attribute = nullptr;
				Sector* activeSector = callerWorld.ActiveSector();
				if (activeSector != nullptr && activeSector->Name() == name)
					attribute = activeSector;
				if (attribute == nullptr)
				{
					attribute = callerWorld.FindAction(name);
					if (attribute == nullptr && name == callerWorld.Name())
						attribute = &callerWorld;
				}

				// cannot use else here
				if (attribute != nullptr)
					scopeToFind = attribute;
			}
		}

		if (scopeToFind == nullptr)
		{
			if (referenceToFind != nullptr)
			{
				if (referenceToFind->Type() == Datum::DatumType::TABLE)
					scopeToFind = &referenceToFind->Get<Scope>();
				else if (referenceToFind->Type() == Datum::DatumType::REFERENCE)
					scopeToFind = &referenceToFind->Get<Datum>().Get<Scope>();
			}
			else if(caller.GetParent() != nullptr && doRecursiveSearch)
				scopeToFind = ComplexSearchHelper(name, *caller.GetParent(), doRecursiveSearch);
		}
		return scopeToFind;
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
		if (mActiveSector != nullptr)
		{
			mLastActiveSector = mActiveSector;
			Sector* sector = mActiveSector;
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
		if (mActiveSector != nullptr)
		{
			mWorldState.sector = mActiveSector;
			mActiveSector->Update(mWorldState);
		}
	}

	void World::SetAudioManager(AudioManager & audioManager)
	{
		mAudioManager = &audioManager;
	}

	AudioManager & World::GetAudioManager()
	{
		return (*mAudioManager);
	}

}
