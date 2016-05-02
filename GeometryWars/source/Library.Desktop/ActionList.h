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
		ActionList(const ActionList& rhs);
		ActionList(ActionList&& rhs);

		/**
		 *	Default destructor
		 */
		virtual ~ActionList() = default;

		/**
		*	disallow assignement operator
		*/
		ActionList& operator=(const ActionList& rhs);
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
		 *	Searches for the Action of the given name in this ActionList
		 *	@param name of the Action to be searched
		 *	@return pointer to the Action if found, nullptr if there is no sector of this name
		 */
		Action* FindAction(const std::string& actionName) const;

		virtual void BeginPlay(WorldState& worldState) override;

		/**
		 *	Calls the update method on all of its child Actions, called by the parent Action / Entity / Sector / World's Update method
		 *	@param reference to the WorldState
		 */
		virtual void Update(WorldState& worldState) override;

		virtual void OnDestroy(WorldState& worldState) override;

		virtual Scope* Clone(const Scope& rhs) const override;

		static const std::string ATTRIBUTE_ACTIONS;

	private:
		void ScriptedBeginPlay(WorldState& worldState);
		void ActionsBeginPlay(WorldState& worldState);
		void ReactionsBeginPlay(WorldState& worldState);

		void ScriptedOnDestroy(WorldState& worldState);
		void ActionsOnDestroy(WorldState& worldState);
		void ReactionsOnDestroy(WorldState& worldState);
		
	};

	CONCRETE_ACTION_FACTORY(ActionList);
}

