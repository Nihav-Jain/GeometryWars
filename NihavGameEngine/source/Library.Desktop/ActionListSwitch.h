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
			 *	Copy Constructor - deep copies the member variables and attributes
			 *	@param constant reference to the ActionListSwitchCase to be copied
			 */
			ActionListSwitchCase(const ActionListSwitchCase& rhs);

			/**
			 *	Move Constructor
			 *	@param R value reference to the ActionListSwitchCase to be moved
			 */
			ActionListSwitchCase(ActionListSwitchCase&& rhs);

			/**
			 *	Default destructor
			 */
			virtual ~ActionListSwitchCase() = default;

			/**
			 *	Copy assignment operator - deep copies the member variables and attributes
			 *	@param constant reference to the ActionListSwitchCase to be copied
			 *	@return reference to the updated ActionListSwitchCase
			 */
			ActionListSwitchCase& operator=(const ActionListSwitchCase& rhs);

			/**
			 *	Move assignment operator
			 *	@param R value reference to the ActionListSwitchCase to be moved
			 *	@return reference to the updated ActionListSwitchCase
			 */
			ActionListSwitchCase& operator=(ActionListSwitchCase&& rhs);

			/**
			 *	acts as a virtual copy constructor
			 *	@param constant reference to the Entity to be copied / cloned
			 *	@return pointer to the newly instantiated ActionListSwitchCase
			 */
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
		protected:
			virtual void DefinePrescribedAttributes();
		};

		CONCRETE_ACTION_FACTORY(ActionListSwitchCase);

		/**
		 *	Constructor - initializes member variables and declares prescribed attributes
		 */
		ActionListSwitch();

		/**
		 *	Copy Constructor - deep copies the member variables and attributes
		 *	@param constant reference to the ActionListSwitch to be copied
		 */
		ActionListSwitch(const ActionListSwitch& rhs);

		/**
		 *	Move Constructor
		 *	@param R value reference to the ActionListSwitch to be moved
		 */
		ActionListSwitch(ActionListSwitch&& rhs);

		/**
		 *	Destructor
		 */
		virtual ~ActionListSwitch();

		/**
		 *	Copy assignment operator - deep copies the member variables and attributes
		 *	@param constant reference to the ActionListSwitch to be copied
		 *	@return reference to the updated ActionListSwitch
		 */
		ActionListSwitch& operator=(const ActionListSwitch& rhs);

		/**
		 *	Move assignment operator
		 *	@param R value reference to the ActionListSwitch to be moved
		 *	@return reference to the updated ActionListSwitch
		 */
		ActionListSwitch& operator=(ActionListSwitch&& rhs);

		/**
		 *	parses the cases inthis ActionList and generates a Hashmap for quick comparison
		 *	called by the Action parser when the end tag is excountered
		 */
		virtual void PostParsingProcess() override;

		/**
		 *	Checks if the current value of the switch value variable matches any case
		 *	executes the matching case action list
		 *	executes the default case if there are no matching cases
		 *	@param reference to the world state
		 */
		virtual void Update(WorldState& worldState) override;

		/**
		 *	acts as a virtual copy constructor
		 *	@param constant reference to the Entity to be copied / cloned
		 *	@return pointer to the newly instantiated ActionListSwitch
		 */
		virtual Scope* Clone(const Scope& rhs) const override;

		static const std::string ATTRIBUTE_SWITCH_VALUE;

	protected:
		virtual void DefinePrescribedAttributes() override;

	private:
		/**
		 *	Parses the cases and generates the Hashmap for quick lookup of matching case values
		 */
		void GenerateCaseMap();
		Hashmap<Datum, ActionListSwitchCase*>* mCaseMap;

	};

	CONCRETE_ACTION_FACTORY(ActionListSwitch);
}

