#pragma once
#include "Hashmap.h"
#include "ActionList.h"

namespace Library
{
	/**
	 *	Represents a switch-case block
	 *	@grammar <switch name="switchName" switchValue="variable">
	 *	@attribute name
	 *	@attribute switchValue - should be the name of the variable to be compared in the cases
	 *	@inherits ActionList
	 */
	class ActionListSwitch : public ActionList
	{
		RTTI_DECLARATIONS(ActionListSwitch, ActionList);

	public:

		/**
		 *	Represents a case block of a switch statement
		 *	@grammar <case name="caseName" caseValue="{const literal}">
		 *	@attribute name
		 *	@attribute caseValue - should be a constant literal value to be compared with the switch variable
		 *	@inherits ActionList
		 */
		class ActionListSwitchCase : public ActionList
		{
			RTTI_DECLARATIONS(ActionListSwitchCase, ActionList);

		public:
			/**
			 *	Constructor - initializes member variables and declares prescribed attributes
			 */
			ActionListSwitchCase();

			/**
			 *	Default destructor
			 */
			virtual ~ActionListSwitchCase() = default;

			ActionListSwitchCase(const ActionListSwitchCase& rhs);
			ActionListSwitchCase(ActionListSwitchCase&& rhs);

			ActionListSwitchCase& operator=(const ActionListSwitchCase& rhs);
			ActionListSwitchCase& operator=(ActionListSwitchCase&& rhs);

			virtual Scope* Clone(const Scope& rhs) const override;

			/**
			 *	boolean to indicate if this case must break from the switch block after executing itself
			 */
			bool MustBreak;

			/**
			 *	boolean to indicate if this is a default case
			 */
			bool DefaultCase;

			static const std::string ATTRIBUTE_CASE_VALUE;
			static const std::string ATTRIBUTE_BREAK;
			static const std::string ATTRIBUTE_DEFAULT;
		};

		CONCRETE_ACTION_FACTORY(ActionListSwitchCase);

		/**
		 *	Constructor - initializes member variables and declares prescribed attributes
		 */
		ActionListSwitch();

		/**
		 *	Destructor
		 */
		virtual ~ActionListSwitch();

		ActionListSwitch(const ActionListSwitch& rhs);
		ActionListSwitch(ActionListSwitch&& rhs);

		ActionListSwitch& operator=(const ActionListSwitch& rhs);
		ActionListSwitch& operator=(ActionListSwitch&& rhs);

		/**
		 *	parses the cases inthis ActionList and generates a Hashmap for quick comparison
		 *	called by the Action parser when the end tag is excountered
		 */
		virtual void BeginPlay(WorldState& worldState) override;

		/**
		 *	Checks if the current value of the switch value variable matches any case
		 *	executes the matching case action list
		 *	executes the default case if there are no matching cases
		 *	@param reference to the world state
		 */
		virtual void Update(WorldState& worldState) override;

		virtual Scope* Clone(const Scope& rhs) const override;

		static const std::string ATTRIBUTE_SWITCH_VALUE;

	private:
		/**
		 *	Parses the cases and generates the Hashmap for quick lookup of matching case values
		 */
		void GenerateCaseMap();
		Hashmap<Datum, ActionListSwitchCase*>* mCaseMap;

	};

	CONCRETE_ACTION_FACTORY(ActionListSwitch);
}

