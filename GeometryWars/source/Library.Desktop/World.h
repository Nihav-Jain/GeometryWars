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
		virtual ~World() = default;

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

		Datum& Actions();

		const Datum& Actions() const;

		Action* FindAction(const std::string& actionName) const;

		void BeginPlay();

		/**
		 *	Calls the update method on all of its child Sectors
		 *	Should be called every frame from the game loop
		 */
		void Update();

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
		*	SetAudioManager - Set the address of contained variableof audio manager
		*/
		void SetAudioManager(AudioManager &audioManager);

		/**
		*	GetAudioManager - Get the reference to contained variableof audio manager
		*	@return reference to the contained variable of audio manager
		*/
		AudioManager& GetAudioManager();
		
		
		static Datum* ComplexSearch(const std::string& name, const Scope& caller);

		XmlParseMaster& ParseMaster();

		void SetWidth(std::int32_t width);
		void SetHeight(std::int32_t height);
		std::int32_t GetWidth();
		std::int32_t GetHeight();

		Sector ClassDefinitionContainer;

		static const std::string ATTRIBUTE_NAME_SECTOR;
		static const std::string ATTRIBUTE_BEGIN_PLAY;
		static const std::string ATTRIBUTE_REACTIONS;
		static const std::string ATTRIBUTE_ON_DESTROY;
		static const std::string ATTRIBUTE_WIDTH;
		static const std::string ATTRIBUTE_HEIGHT;
		static const std::string ATTRIBUTE_DELTA_TIME;

	private:
		static Scope* ComplexSearchHelper(const std::string& name, const Scope& caller, bool doRecursiveSearch = false);

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

		std::int32_t mWidth;
		std::int32_t mHeight;

		AudioManager *mAudioManager;

		static const std::string ATTRIBUTE_NAME;

	};
}

