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
		 *	disallow copy construtor
		 */
		Entity(const Entity& rhs) = delete;

		/**
		 *	Default destructor
		 */
		virtual ~Entity() = default;

		/**
		 *	disallow copy assignemnt operator
		 */
		Entity& operator=(const Entity& rhs) = delete;
		

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

		void BeginPlay(WorldState& worldState);

		/**
		 *	Calls the update method on all of its child Actions, called by the parent Sector's Update method
		 *	Must be overriden by children of they want to dp anything extra
		 *	@param reference to the WorldState
		 */
		virtual void Update(WorldState& worldState);

		bool IsPendingDestroy() const;
		void MarkForDestroy(WorldState& worldState);

		static const std::uint32_t NUM_RESERVED_PRESCRIBED_ATTRIBUTES;
		static const std::string ATTRIBUTE_NAME;
		static const std::string ATTRIBUTE_ACTIONS;

	private:
		std::string mName;
		bool mIsPendingDestroy;
	};

#define CONCRETE_ENTITY_FACTORY(ConcreteEntityType) CONCRETE_FACTORY(ConcreteEntityType, Entity);

	CONCRETE_ENTITY_FACTORY(Entity);
}

