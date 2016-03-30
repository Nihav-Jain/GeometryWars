#pragma once
#include "Action.h"
#include "WorldState.h"

namespace Library
{
	/**
	 *	Concrete Action which can contain a list of Actions
	 *	Has 1 prescribed attribute - name (STRING)
	 *	@grammar <action class="ActionList" name="{instanceName}" />
	 *	@inherits Action
	 */
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
		 *	Getter for the Datum which contains the array of Actions of this ActionList
		 *	@return reference to the Datum
		 */
		Datum& Actions();

		/**
		 *	const override for the Getter for the Datum which contains the array of Actions of this ActionList
		 *	@return constant reference to the Datum
		 */
		const Datum& Actions() const;

		/**
		 *	Adds an Action of the given name to this ActionList
		 *	@param name of the derived Action class which is to be created
		 *	@param  instance name of the Action
		 *	@return reference to the newly created Action
		 */
		Action& CreateAction(const std::string& actionClassName, const std::string& actionInstanceName);

		/**
		 *	Searches for the Action of the given name in this ActionList
		 *	@param name of the Action to be searched
		 *	@return pointer to the Action if found, nullptr if there is no sector of this name
		 */
		Action* FindAction(const std::string& actionName) const;

		/**
		 *	Calls the update method on all of its child Actions, called by the parent Action / Entity / Sector / World's Update method
		 *	@param reference to the WorldState
		 */
		virtual void Update(WorldState& worldState) override;

		/**
		 *	acts as a virtual copy constructor
		 *	@param constant reference to the Entity to be copied / cloned
		 *	@return pointer to the newly instantiated ActionList
		 */
		virtual Scope* Clone(const Scope& rhs) const override;

	protected:
		virtual void DefinePrescribedAttributes() override;

	private:
		static const std::string ATTRIBUTE_ACTIONS;
	};

	CONCRETE_ACTION_FACTORY(ActionList);
}

