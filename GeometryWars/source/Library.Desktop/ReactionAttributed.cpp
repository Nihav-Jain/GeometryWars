#include "pch.h"
#include "ReactionAttributed.h"
#include "EventMessageAttributed.h"
#include "Event.h"

namespace Library
{
	RTTI_DEFINITIONS(ReactionAttributed);

	ReactionAttributed::ReactionAttributed()
	{
		AddInternalAttribute(EventMessageAttributed::ATTRIBUTE_SUBTYPE, "", 0);
		Event<EventMessageAttributed>::Subscribe(*this);
	}


	ReactionAttributed::~ReactionAttributed()
	{
		Event<EventMessageAttributed>::Unsubscribe(*this);
	}

	void ReactionAttributed::Notify(const EventPublisher& publisher)
	{
		if (!publisher.Is(Event<EventMessageAttributed>::TypeIdClass()))
			return;
		Event<EventMessageAttributed>& eventPublisher = *(publisher.AssertiveAs<Event<EventMessageAttributed>>());
		const EventMessageAttributed& message = eventPublisher.Message();

		std::uint32_t i;
		Datum& subtypes = (*this)[EventMessageAttributed::ATTRIBUTE_SUBTYPE];
		std::uint32_t size = subtypes.Size();
		for (i = 0; i < size; i++)
		{
			if (message.GetSubtype() == subtypes.Get<std::string>(i))
				break;
		}
		if (i == size)
			return;

		CopyAuxiliaryAttributesFromAnotherAttributed(message);

		ActionList::Update(message.GetWorldState());
	}

}
