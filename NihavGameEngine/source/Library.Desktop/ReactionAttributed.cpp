#include "pch.h"
#include "ReactionAttributed.h"
#include "EventMessageAttributed.h"
#include "Event.h"

namespace Library
{
	RTTI_DEFINITIONS(ReactionAttributed);

	ReactionAttributed::ReactionAttributed()
	{
		Populate();
		Event<EventMessageAttributed>::Subscribe(*this);
	}


	ReactionAttributed::~ReactionAttributed()
	{
	}

	void ReactionAttributed::Notify(const EventPublisher& publisher)
	{
		if (!publisher.Is(EventMessageAttributed::TypeIdClass()))
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
		
		const Datum& attributes = message[EventMessageAttributed::ATTRIBUTE_ATTRIBUTES];
		if(attributes.Size() > 0)
			CopyMessageAttributes(*attributes.Get<Scope*>());

		ActionList::Update(message.GetWorldState());
	}

	void ReactionAttributed::CopyMessageAttributes(const Scope& attributes)
	{
		UNREFERENCED_PARAMETER(attributes);
	}

	void ReactionAttributed::DefinePrescribedAttributes()
	{
		AddInternalAttribute(EventMessageAttributed::ATTRIBUTE_SUBTYPE, "", 0);
	}

}
