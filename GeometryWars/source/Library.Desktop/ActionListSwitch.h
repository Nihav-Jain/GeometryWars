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
			 *	disallow copy construtor
			 */
			ActionListSwitchCase(const ActionListSwitchCase& rhs) = delete;

			/**
			 *	Default destructor
			 */
			virtual ~ActionListSwitchCase() = default;

			/**
			 *	disallow copy assignment operator
			 */
			ActionListSwitchCase& operator=(const ActionListSwitchCase& rhs) = delete;

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
		 *	disallow copy construtor
		 */
		ActionListSwitch(const ActionListSwitch& rhs) = delete;

		/**
		 *	Destructor
		 */
		virtual ~ActionListSwitch();

		/**
		 *	disallow copy assignment operator
		 */
		ActionListSwitch& operator=(const ActionListSwitch& rhs) = delete;

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

