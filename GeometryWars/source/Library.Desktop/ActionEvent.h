#pragma once
#include "Action.h"

namespace Library
{
	/**
	 *	Registers an event with the EventQueue of the world with the specified delay
	 *	Has 3 prescribed attributes - name (STRING), event subtype (STIRNG) and delay (INTEGER - defaulted to 0)
	 *	@grammar  <event name="mouseDownEvent" subtype="mousedown" delay="100"/>
	 *	@requires XmlParseHelperActionEvent, ActionEventFactory
	 *	@inherits Action
	 */
	class ActionEvent : public ActionList
	{
		RTTI_DECLARATIONS(ActionEvent, ActionList);

	public:
		/**
		 *	Constructor
		 */
		ActionEvent();

		/**
		 *	Default destructor
		 */
		virtual ~ActionEvent() = default;

		ActionEvent(const ActionEvent& rhs);
		ActionEvent(ActionEvent&& rhs);

		ActionEvent& operator=(const ActionEvent& rhs);
		ActionEvent& operator=(ActionEvent&& rhs);

		/**
		 *	Preapres the event payload with the proper subtype, copies its auxiliary attributes to it and enqueues the event
		 *	@param reference to the world state
		 */
		virtual void Update(WorldState& worldState) override;
		virtual Scope* Clone(const Scope& rhs) const override;

		static const std::string ATTRIBUTE_SUBTYPE;
		static const std::string ATTRIBUTE_DELAY;
		static const std::string ATTRIBUTE_ARGUMENTS;
	};

	CONCRETE_ACTION_FACTORY(ActionEvent);
}

