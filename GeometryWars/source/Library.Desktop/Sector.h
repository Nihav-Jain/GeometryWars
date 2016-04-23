#pragma once
#include <string>
#include <sstream>

#include "Attributed.h"
#include "Entity.h"
#include "WorldState.h"

namespace Library
{
	class World;

	/**
	 *	Equivalent to a Level or a Scene of a game, container of Entities
	 *	Has 2 prescribed attributes - name (STRING) and entities (TABLE)
	 *	requires XmlParseHelperSector to being created via XML
	 *	@inherits Attributed
	 */
	class Sector final : public Attributed
	{
		RTTI_DECLARATIONS(Sector, Attributed);
	public:

		/**
		 *	Constructor - initializes member variables and declares prescribed attributes
		 */
		Sector();

		/**
		 *	disallow copy construtor
		 */
		Sector(const Sector& rhs) = delete;

		/**
		 *	Default destructor
		 */
		virtual ~Sector() = default;

		/**
		 *	disallow copy assignment operator
		 */
		Sector& operator=(const Sector& rhs);

		/**
		 *	Getter for the name of this Sector
		 *	@return constance reference to the string representing the name
		 */
		const std::string& Name() const;

		/**
		 *	Setter for the name of this Sector
		 *	@param constance reference to the string representing the name
		 */
		void SetName(const std::string& name);

		/**
		 *	Getter for the Datum which contains the array of Entities of this Sector
		 *	@return reference to the Datum
		 */
		Datum& Entities();

		/**
		 *	const override for the Getter for the Datum which contains the array of Entities of this Sector
		 *	@return constant reference to the Datum
		 */
		const Datum& Entities() const;

		/**
		 *	Adds an Entity of the given name to this Sector
		 *	@param name of the derived Entity class which is to be created
		 *	@param  instance name of the Entity
		 *	@return reference to the newly created Entity
		 */
		Entity& CreateEntity(const std::string& entityClassName, const std::string& entityInstanceName);
		
		/**
		 *	Searches for the Entity of the given name in this Sector
		 *	@param name of the Entity to be searched
		 *	@return pointer to the Entity if found, nullptr if there is no sector of this name
		 */
		Entity* FindEntity(const std::string& entityName) const;

		Vector<Entity*> FindAllEntities(const std::string& entityName) const;

		Datum& Actions();

		const Datum& Actions() const;

		Action* FindAction(const std::string& actionName) const;

		/**
		 *	Getter for the World which contains this Sector
		 *	@return pointer to the World
		 */
		World* GetWorld() const;

		/**
		 *	Setter for the World which contains this Sector
		 *	@param reference to the World which adopts this Sector
		 */
		void SetWorld(World& parent);

		void BeginPlay(WorldState& worldState);

		/**
		 *	Calls the update method on all of its child Entities, called by the parent World's Update method
		 *	@param reference to the WorldState
		 */
		void Update(WorldState& worldState);

		const Vector<Entity*>& GetAllEntitiesOfType(std::uint64_t typeId) const;

		static const std::string ATTRIBUTE_ENTITIES;
		static const std::string ATTRIBUTE_NAME;

	private:
		void UpdateSectorActions(WorldState& worldState);
		void DeletePendingDestroyEntites();
		void UpdateEntites(WorldState& worldState);

		void AddEntityToTypeMap(Entity& entity, Graph<std::uint64_t*>::Traversor& typeIdTraversor);
		void RemoveEntityFromTypeMap(Entity& entity, Graph<std::uint64_t*>::Traversor& typeIdTraversor);

		std::string mName;

		typedef Hashmap<std::uint64_t, Vector<Entity*>> EntityTypeMap;
		EntityTypeMap mEntityListByType;
	};

}

