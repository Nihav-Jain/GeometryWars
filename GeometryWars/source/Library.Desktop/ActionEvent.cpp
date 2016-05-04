#include "pch.h"
#include <memory>
#include "ActionEvent.h"
#include "Event.h"
#include "EventMessageAttributed.h"

namespace Library
{
	RTTI_DEFINITIONS(ActionEvent, ActionList);

	const std::string ActionEvent::ATTRIBUTE_SUBTYPE = "subtype";
	const std::string ActionEvent::ATTRIBUTE_DELAY = "delay";
	const std::string ActionEvent::ATTRIBUTE_ARGUMENTS = "arguments";

	ActionEvent::ActionEvent()
	{
		AddInternalAttribute(ATTRIBUTE_SUBTYPE, "", 0);
		AddInternalAttribute(ATTRIBUTE_DELAY, 0, 1);
	}

	ActionEvent::ActionEvent(const ActionEvent& rhs) :
		ActionList::ActionList(rhs)
	{}

	ActionEvent::ActionEvent(ActionEvent&& rhs) :
		ActionList::ActionList(std::move(rhs))
	{}

	ActionEvent& ActionEvent::operator=(const ActionEvent& rhs)
	{
		ActionList::operator=(rhs);
		return *this;
	}

	ActionEvent& ActionEvent::operator=(ActionEvent&& rhs)
	{
		ActionList::operator=(std::move(rhs));
		return *this;
	}

	void ActionEvent::Update(WorldState& worldState)
	{
		ActionList::Update(worldState);

		EventMessageAttributed message;
		message.SetSubtype((*this)[ATTRIBUTE_SUBTYPE].Get<std::string>());
		message.SetWorldState(worldState);

		message.CopyAuxiliaryAttributesFromAnotherAttributed(*this);

		std::int32_t delay = (*this)[ATTRIBUTE_DELAY].Get<std::int32_t>();
		std::shared_ptr<Event<EventMessageAttributed>> attributedEvent = std::make_shared<Event<EventMessageAttributed>>(message);
		worldState.world->GetEventQueue().Enqueue(attributedEvent, *worldState.mGameTime, std::chrono::milliseconds(delay));

	}

	Scope* ActionEvent::Clone(const Scope& rhs) const
	{
		ActionEvent& action = *rhs.AssertiveAs<ActionEvent>();
		return new ActionEvent(action);
	}
}
