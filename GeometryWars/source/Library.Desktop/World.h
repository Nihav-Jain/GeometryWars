#pragma once
#include "Attributed.h"
#include "Sector.h"
#include "WorldState.h"
#include "GameTime.h"
#include "EventQueue.h"
#include "ActionList.h"
#include "AudioManager.h"
#include "XmlParseMaster.h"

namespace Library
{
	/**
	 *	Defines the container of Sectors (Levels / Scenes)
	 *	Has 2 prescribed attributes - name (STRING) and sectors (TABLE)
	 *	requires XmlParseHelperWorld to being created via XML
	 *	@inherits Attributed
	 */
	class World final : public Attributed
	{
		RTTI_DECLARATIONS(World, Attributed);
	public:

		/**
		 *	Constructor - initializes member variables and declares prescribed attributes
		 */
		explicit World(const GameTime& gameTime, XmlParseMaster& parseMaster);

		/**
		 s*	disallow copy construtor
		 */
		World(const World& rhs) = delete;

		/**
		 *	Default destructor
		 */
		virtual ~World();

		/**
		 *	disallow copy construtor
		 */
		World& operator=(const World& rhs) = delete;

		/**
		 *	Getter for the name of this World
		 *	@return constance reference to the string representing the name
		 */
		const std::string& Name() const;

		/**
		 *	Setter for the name of this World
		 *	@param constance reference to the string representing the name
		 */
		void SetName(const std::string& name);

		/**
		 *	Getter for the Datum which contains the array of Sectors of this World
		 *	@return reference to the Datum
		 */
		Datum& Sectors();

		/**
		 *	const override for the Getter for the Datum which contains the array of Sectors of this World
		 *	@return constant reference to the Datum
		 */
		const Datum& Sectors() const;

		/**
		 *	Adds a Sector of the given name to this World
		 *	@param name of the Sector
		 *	@return reference to the newly created Sector
		 */
		Sector& CreateSector(const std::string& sectorName);

		/**
		 *	Searches for the Sector of the given name in this World
		 *	@param name of the Sector to be searched
		 *	@return pointer to the Sector if found, nullptr if there is no sector of this name
		 */
		Sector* FindSector(const std::string& sectorName) const;

		/**
		 *	Getter for the Datum which contains the array of Actions of this World
		 *	@return reference to the Datum
		 */
		Datum& Actions();

		/**
		 *	const override for the Getter for the Datum which contains the array of Actions of this World
		 *	@return constant reference to the Datum
		 */
		const Datum& Actions() const;

		/**
		 *	Searches for the Action of the given name in this World
		 *	@param name of the Action to be searched
		 *	@return pointer to the Action if found, nullptr if there is no action of this name
		 */
		Action* FindAction(const std::string& actionName) const;

		/**
		 *	Calls the scripted begin play of the world and the begin play of all its actions, reactions and sectors
		 */
		void BeginPlay();

		/**
		 *	Calls the update method on all of its child Sectors
		 *	Should be called every frame from the game loop
		 */
		void Update();

		/**
		 *	Calls the scripted on-destroy of the world and the on-destroy of all its actions, reactions and sectors
		 */
		void OnDestroy();

		/**
		 *	Getter for world state
		 *	@return reference to WorldState
		 */
		WorldState& GetWorldState();

		/**
		 *	Getter for EventQueue
		 *	@return reference to EventQueue
		 */
		EventQueue& GetEventQueue();

		/**
		 *	Creates a copy of the sector to be loaded, if found in memory, and prepares for loading that sector in the next frame update
		 *	@param name of sector to be loaded
		 *	@return true if sector was succesfully copied, false if no sector of given name exists
		 */
		bool LoadSector(const std::string& sectorName);

		/**
		 *	SetAudioManager - Set the address of contained variableof audio manager
		 */
		void SetAudioManager(AudioManager &audioManager);

		/**
		 *	GetAudioManager - Get the reference to contained variableof audio manager
		 *	@return reference to the contained variable of audio manager
		 */
		AudioManager& GetAudioManager();
		
		/**
		 *	Used to search for dot referenced attribtues though World, Sector, Entities and Actions 
		 *	@param dot referenced name of attribute to be searched, initial scope which invoked the search
		 *	@return pointer to the Datum, if found.. nullptr if not found
		 */
		static Datum* ComplexSearch(const std::string& name, const Scope& caller);

		/**
		 *	getter for the World's parse master
		 *	@return reference to the parse master
		 */
		XmlParseMaster& ParseMaster();

		/**
		 *	sets the viewport width of the World
		 *	@param width
		 */
		void SetWidth(std::int32_t width);

		/**
		 *	sets the viewport height of the World
		 *	@param height
		 */
		void SetHeight(std::int32_t height);

		/**
		 *	getter for the viewport width of the World
		 *	@param width
		 */
		std::int32_t GetWidth();

		/**
		 *	getter for the viewport height of the World
		 *	@param height
		 */
		std::int32_t GetHeight();

		/**
		 *	container for the constructed copies of all XML defined entitites
		 */
		Sector ClassDefinitionContainer;

		/**
		 *	container for the constructed copies of all XML defined sectors
		 */
		World* SectorDefinitionContainer;

		/**
		 *	getter for the active sector
		 */
		Sector* ActiveSector();

		static const std::string ATTRIBUTE_NAME_SECTOR;
		static const std::string ATTRIBUTE_BEGIN_PLAY;
		static const std::string ATTRIBUTE_REACTIONS;
		static const std::string ATTRIBUTE_ON_DESTROY;
		static const std::string ATTRIBUTE_WIDTH;
		static const std::string ATTRIBUTE_HEIGHT;
		static const std::string ATTRIBUTE_DELTA_TIME;

	private:
		static Scope* ComplexSearchHelper(const std::string& name, const Scope& caller, bool doRecursiveSearch = false);
		static Stack<Datum*> sTempReferenceDatums;

		void ScriptedBeginPlay();
		void SectorsBeginPlay();
		void ActionsBeginPlay();
		void ReactionsBeginPlay();

		void ScriptedOnDestroy();
		void SectorsOnDestroy();
		void ActionsOnDestroy();
		void ReactionsOnDestroy();

		void UpdateWorldActions();
		void UpdateWorldSectors();

		std::string mName;
		WorldState mWorldState;
		EventQueue mEventQueue;
		XmlParseMaster* mParseMaster;

		Sector* mActiveSector;
		Sector* mLastActiveSector;
		Sector* mSectorToLoad;

		std::int32_t mWidth;
		std::int32_t mHeight;

		AudioManager *mAudioManager;

		static const std::string ATTRIBUTE_NAME;

	};
}

