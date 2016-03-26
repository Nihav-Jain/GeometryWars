#pragma once
#include "Attributed.h"
#include "Sector.h"
#include "WorldState.h"

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
		World();

		/**
		 *	Copy Constructor - deep copies the member variables and attributes
		 *	@param constant reference to the World to be copied
		 */
		World(const World& rhs);

		/**
		 *	Move Constructor
		 *	@param R value reference to the World to be moved
		 */
		World(World&& rhs);

		/**
		 *	Default destructor
		 */
		virtual ~World() = default;

		/**
		 *	Copy assignment operator - deep copies the member variables and attributes
		 *	@param constant reference to the World to be copied
		 *	@return reference to the updated World
		 */
		World& operator=(const World& rhs);

		/**
		 *	Move assignment operator
		 *	@param R value reference to the World to be moved
		 *	@return reference to the updated World
		 */
		World& operator=(World&& rhs);

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
		 *	Calls the update method on all of its child Sectors
		 *	Should be called every frame from the game loop
		 */
		void Update();

		/**
		 *	Sets the GameTime pointer in the WorldState object
		 *	@param constant reference to the GameTime object
		 */
		void SetGameTime(const GameTime& gameTime);

		/**
		 *	static function to create a World of the given name, as a child of the provided Scope
		 *	@param name of the World
		 *	@param reference to the parent Scope of the newly created World
		 *	@return reference to the newly created World
		 */
		static World& CreateWorld(const std::string& name, Scope& parentScope);

		static const std::string ATTRIBUTE_NAME_SECTOR;

	protected:
		virtual void DefinePrescribedAttributes() override;

	private:
		std::string mName;
		WorldState mWorldState;

		static const std::string ATTRIBUTE_NAME;

	};
}

