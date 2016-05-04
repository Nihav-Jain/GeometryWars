#pragma once
#include "RTTI.h"
#include "Attributed.h"
#include "Factory.h"
#include "WorldState.h"
#include "Action.h"

namespace Library
{
	class Sector;

	/**
	 *	Defines a game entity
	 *	Implement the CONCRETE_ENTITY_FACTORY macro with the attribute <ChildClassName> to create a factory for your derived Entity class
	 *	Has 1 prescribed attribute - name (STRING)
	 *	requires XmlParseHelperEntity to being created via XML
	 *	@inherits Attributed
	 */
	class Entity : public Attributed
	{
		RTTI_DECLARATIONS(Entity, Attributed);
	public:

		/**
	 	 *	Constructor - initializes member variables and declares prescribed attributes
		 */
		Entity();

		/**
		 *	Default destructor
		 */
		virtual ~Entity();

		/**
		 *	copy Constructor
		 */
		Entity(const Entity& rhs);

		/**
		 *	move Constructor
		 */
		Entity(Entity&& rhs);

		/**
		 *	copy assignemnt operator
		 */
		Entity& operator=(const Entity& rhs);

		/**
		 *	move assignemnt operator
		 */
		Entity& operator=(Entity&& rhs);

		/**
		 *	Getter for the name of this Entity
		 *	@return constance reference to the string representing the name
		 */
		const std::string& Name() const;

		/**
		 *	Setter for the name of this Entity
		 *	@param constance reference to the string representing the name
		 */
		void SetName(const std::string& name);

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
		 *	Getter for the Sector which contains this Entity
		 *	@return pointer to the Sector
		 */
		Sector* GetSector() const;

		/**
		 *	Setter for the Sector which contains this Entity
		 *	@param reference to the Sector which adopts this Entity
		 */
		void SetSector(Sector& parent);

		/**
		 *	Calls the scripted begin play of the entity and the begin play of all its actions, reactions
		 */
		virtual void BeginPlay(WorldState& worldState);

		/**
		 *	Calls the update method on all of its child Actions, called by the parent Sector's Update method
		 *	Must be overriden by children of they want to dp anything extra
		 *	@param reference to the WorldState
		 */
		virtual void Update(WorldState& worldState);

		/**
		 *	Virtual copy constructor
		 *	@return pointer to newly copy constructed Entity
		 */
		virtual Scope* Clone(const Scope& rhs) const override;

		/**
		 *	Calls the scripted on-destroy of the entity and the on-destroy of all its actions, reactions
		 */
		virtual void OnDestroy(WorldState& worldState);

		/**
		 *	checks if the Entity has been marked for destroy
		 *	@return true if entity has been marked for destruction
		 */
		bool IsPendingDestroy() const;

		/**
		 *	Mark this entity to be destroyed in the next frame update
		 *	@param world state
		 */
		void MarkForDestroy(WorldState& worldState);

		static const std::uint32_t NUM_RESERVED_PRESCRIBED_ATTRIBUTES;
		static const std::string ATTRIBUTE_NAME;
		static const std::string ATTRIBUTE_ACTIONS;
		static const std::string ATTRIBUTE_OWNER_SECTOR;

	private:
		void ScriptedBeginPlay(WorldState& worldState);
		void ActionsBeginPlay(WorldState& worldState);
		void ReactionsBeginPlay(WorldState& worldState);

		void ScriptedOnDestroy(WorldState& worldState);
		void ActionsOnDestroy(WorldState& worldState);
		void ReactionsOnDestroy(WorldState& worldState);

		std::string mName;
		bool mIsPendingDestroy;
		Datum* mSector;
	};

#define CONCRETE_ENTITY_FACTORY(ConcreteEntityType) CONCRETE_FACTORY(ConcreteEntityType, Entity);

	CONCRETE_ENTITY_FACTORY(Entity);
}

