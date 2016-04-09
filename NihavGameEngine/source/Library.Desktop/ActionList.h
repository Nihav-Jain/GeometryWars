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
		 *	disallow copy construtor
		 */
		ActionList(const ActionList& rhs) = delete;

		/**
		 *	Default destructor
		 */
		virtual ~ActionList() = default;

		/**
		*	disallow assignement operator
		*/
		ActionList& operator=(const ActionList& rhs) = delete;


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

	private:
		static const std::string ATTRIBUTE_ACTIONS;
	};

	CONCRETE_ACTION_FACTORY(ActionList);
}

