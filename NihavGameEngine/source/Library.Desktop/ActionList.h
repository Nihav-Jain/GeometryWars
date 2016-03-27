#pragma once
#include "Action.h"
#include "WorldState.h"

namespace Library
{
	class ActionList : public Action
	{
		RTTI_DECLARATIONS(ActionList, Action);

	public:
		/**
		 *	Constructor - initializes member variables and declares prescribed attributes
		 */
		ActionList();
	
		/**
		 *	Copy Constructor - deep copies the member variables and attributes
		 *	@param constant reference to the Action to be copied
		 */
		ActionList(const ActionList& rhs);

		/**
		 *	Move Constructor
		 *	@param R value reference to the Action to be moved
		 */
		ActionList(ActionList&& rhs);

		/**
		 *	Default destructor
		 */
		virtual ~ActionList();

		/**
		 *	Copy assignment operator - deep copies the member variables and attributes
		 *	@param constant reference to the Action to be copied
		 *	@return reference to the updated Action
		 */
		ActionList& operator=(const ActionList& rhs);

		/**
		 *	Move assignment operator
		 *	@param R value reference to the Action to be moved
		 *	@return reference to the updated Action
	 	 */
		ActionList& operator=(ActionList&& rhs);

		/**
		 *	Getter for the Datum which contains the array of Entities of this Sector
		 *	@return reference to the Datum
		 */
		Datum& Actions();

		/**
		 *	const override for the Getter for the Datum which contains the array of Entities of this Sector
		 *	@return constant reference to the Datum
		 */
		const Datum& Actions() const;

		/**
		 *	Adds an Entity of the given name to this Sector
		 *	@param name of the derived Entity class which is to be created
		 *	@param  instance name of the Entity
		 *	@return reference to the newly created Entity
		 */
		Action& CreateAction(const std::string& actionClassName, const std::string& actionInstanceName);

		/**
		 *	Searches for the Entity of the given name in this Sector
		 *	@param name of the Entity to be searched
		 *	@return pointer to the Entity if found, nullptr if there is no sector of this name
		 */
		Action* FindAction(const std::string& actionName) const;

		/**
		 *	Calls the update method on all of its child Actions, called by the parent Sector's Update method
		 *	Must be overriden by children of they want to dp anything extra
		 *	@param reference to the WorldState
		 */
		virtual void Update(WorldState& worldState) override;

		/**
		 *	acts as a virtual copy constructor
		 *	@param constant reference to the Entity to be copied / cloned
		 *	@return pointer to the newly instantiated Entity
		 */
		virtual Scope* Clone(const Scope& rhs) const override;

	protected:
		virtual void DefinePrescribedAttributes() override;

	private:
		static const std::string ATTRIBUTE_ACTIONS;
	};

	CONCRETE_ACTION_FACTORY(ActionList);
}

