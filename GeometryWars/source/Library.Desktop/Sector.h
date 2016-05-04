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
		 *	Default destructor
		 */
		virtual ~Sector();

		/**
		 *	copy Constructor
		 */
		Sector(const Sector& rhs);

		/**
		 *	move Constructor
		 */
		Sector(Sector&& rhs);

		/**
		 *	copy assignemnt operator
		 */
		Sector& operator=(const Sector& rhs);

		/**
		 *	move assignemnt operator
		 */
		Sector& operator=(Sector&& rhs);

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
		 *	Adopts the given Entity into itself
		 *	@param refernce to the entity to be adopted
		 *	@param  instance name of the Entity
		 */
		void AdoptEntity(Entity& entity, const std::string& name);
		
		/**
		 *	Searches for the Entity of the given name in this Sector
		 *	@param name of the Entity to be searched
		 *	@return pointer to the Entity if found, nullptr if there is no sector of this name
		 */
		Entity* FindEntity(const std::string& entityName) const;

		/**
		 *	Finds all entitites of the given name
		 *	@param name of entity to be searched
		 *	@return vetor of enitity poitners
		 */
		Vector<Entity*> FindAllEntities(const std::string& entityName) const;

		/**
		 *	Getter for the Datum which contains the array of Actions of this Sector
		 *	@return reference to the Datum
		 */
		Datum& Actions();

		/**
		 *	const override for the Getter for the Datum which contains the array of Actions of this Sector
		 *	@return constant reference to the Datum
		 */
		const Datum& Actions() const;

		/**
		 *	Searches for the Action of the given name in this Entity
		 *	@param name of the Action to be searched
		 *	@return pointer to the Action if found, nullptr if there is no sector of this name
		 */
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

		/**
		 *	Calls the scripted begin play of the sector and the begin play of all its actions, reactions and entities
		 */
		void BeginPlay(WorldState& worldState);

		/**
		 *	Calls the update method on all of its child Entities, called by the parent World's Update method
		 *	@param reference to the WorldState
		 */
		void Update(WorldState& worldState);
		
		/**
		 *	Virtual copy constructors
		 *	@return pointer to newly copy constructed Sector
		 */
		virtual Scope* Clone(const Scope& rhs) const override;

		/**
		 *	Calls the scripted on-destroy of the sector and the on-destroy of all its actions, reactions and entities
		 */
		void OnDestroy(WorldState& worldState);

		/**
		 *	Gets all entities which belong to the given typeid or are children of this typeid
		 *	@param type id of parent entity
		 *	@return const vector of entity pointers
		 */
		const Vector<Entity*>& GetAllEntitiesOfType(std::uint64_t typeId);

		static const std::string ATTRIBUTE_ENTITIES;
		static const std::string ATTRIBUTE_NAME;
		static const std::string ATTRIBUTE_OWNER_WORLD;

	private:
		void AddEntityToTypeMap(Entity& entity, const std::uint64_t* parentTypeIdPtr);
		void RemoveEntityFromTypeMap(Entity& entity, const std::uint64_t* parentTypeIdPtr);

		void ScriptedBeginPlay(WorldState& worldState);
		void EntitiesBeginPlay(WorldState& worldState);
		void ActionsBeginPlay(WorldState& worldState);
		void ReactionsBeginPlay(WorldState& worldState);

		void ScriptedOnDestroy(WorldState& worldState);
		void EntitiesOnDestroy(WorldState& worldState);
		void ActionsOnDestroy(WorldState& worldState);
		void ReactionsOnDestroy(WorldState& worldState);

		void UpdateSectorActions(WorldState& worldState);
		void DeletePendingDestroyEntities(WorldState& worldState);
		void UpdateSectorEntities(WorldState& worldState);

		std::string mName;
		Datum* mWorld;

		typedef Hashmap<std::uint64_t, Vector<Entity*>> EntityTypeMap;
		EntityTypeMap mEntityListByType;
	};

}

