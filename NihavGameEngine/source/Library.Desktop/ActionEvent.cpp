#include "pch.h"
#include <memory>
#include "ActionEvent.h"
#include "Event.h"
#include "EventMessageAttributed.h"

namespace Library
{
	RTTI_DEFINITIONS(ActionEvent);

	const std::string ActionEvent::ATTRIBUTE_SUBTYPE = "subtype";
	const std::string ActionEvent::ATTRIBUTE_DELAY = "delay";
	const std::string ActionEvent::ATTRIBUTE_ARGUMENTS = "arguments";

	ActionEvent::ActionEvent()
	{
		Populate();
	}

	void ActionEvent::Update(WorldState& worldState)
	{
		EventMessageAttributed message;
		message.SetSubtype((*this)[ATTRIBUTE_SUBTYPE].Get<std::string>());
		message.SetWorldState(worldState);

		message.CopyAuxiliaryAttributesFromAnotherAttributed(*this);

		std::int32_t delay = (*this)[ATTRIBUTE_DELAY].Get<std::int32_t>();
		std::shared_ptr<Event<EventMessageAttributed>> attributedEvent = std::make_shared<Event<EventMessageAttributed>>(message);
		worldState.world->GetEventQueue().Enqueue(attributedEvent, *worldState.mGameTime, std::chrono::milliseconds(delay));

	}

	void ActionEvent::DefinePrescribedAttributes()
	{
		AddInternalAttribute(ATTRIBUTE_SUBTYPE, "", 0);
		AddInternalAttribute(ATTRIBUTE_DELAY, 0, 1);
	}

}
