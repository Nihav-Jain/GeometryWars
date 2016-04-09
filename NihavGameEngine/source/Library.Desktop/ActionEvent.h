#pragma once
#include "Action.h"

namespace Library
{
	class ActionEvent : public Action
	{
		RTTI_DECLARATIONS(ActionEvent, Action);

	public:
		ActionEvent();
		virtual ~ActionEvent();

		ActionEvent(const ActionEvent& rhs) = delete;
		ActionEvent& operator=(const ActionEvent& rhs) = delete;

		virtual void Update(WorldState& worldState) override;

		static const std::string ATTRIBUTE_SUBTYPE;
		static const std::string ATTRIBUTE_DELAY;
		static const std::string ATTRIBUTE_ARGUMENTS;

	protected:
		virtual void DefinePrescribedAttributes() override;
	};

	CONCRETE_ACTION_FACTORY(ActionEvent);
}

