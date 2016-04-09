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

		for (i = message.AuxiliaryBegin(); i < message.Size(); i++)
		{
			const SymbolPair& pair = message.GetPair(i);
			Remove(pair.first);
			if (pair.second.Type() == Datum::DatumType::TABLE)
			{
				const Datum& nestedScopeDatum = pair.second;
				std::uint32_t j;
				for (j = 0; j < nestedScopeDatum.Size(); j++)
				{
					Scope* nestedScopeAttribute = new Scope(*nestedScopeDatum.Get<Scope*>(j));
					Adopt(pair.first, *nestedScopeAttribute);
				}
			}
			else
			{
				AppendAuxiliaryAttribute(pair.first) = pair.second;
			}
		}

		ActionList::Update(message.GetWorldState());
	}

	void ReactionAttributed::DefinePrescribedAttributes()
	{
		AddInternalAttribute(EventMessageAttributed::ATTRIBUTE_SUBTYPE, "", 0);
	}

}
