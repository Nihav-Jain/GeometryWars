#include "pch.h"
#include "ReactionAttributed.h"
#include "EventMessageAttributed.h"
#include "Event.h"

namespace Library
{
	RTTI_DEFINITIONS(ReactionAttributed, Reaction);

	ReactionAttributed::ReactionAttributed()
	{
		AddInternalAttribute(EventMessageAttributed::ATTRIBUTE_SUBTYPE, "", 0);
	}

	ReactionAttributed::~ReactionAttributed()
	{
		Event<EventMessageAttributed>::Unsubscribe(*this);
	}

	ReactionAttributed::ReactionAttributed(const ReactionAttributed& rhs) :
		Reaction::Reaction(rhs)
	{}

	ReactionAttributed::ReactionAttributed(ReactionAttributed&& rhs) :
		Reaction::Reaction(std::move(rhs))
	{}

	ReactionAttributed& ReactionAttributed::operator=(const ReactionAttributed& rhs)
	{
		Reaction::operator=(rhs);
		return *this;
	}

	ReactionAttributed& ReactionAttributed::operator=(ReactionAttributed&& rhs)
	{
		Reaction::operator=(std::move(rhs));
		return *this;
	}

	void ReactionAttributed::BeginPlay(WorldState& worldState)
	{
		Event<EventMessageAttributed>::Subscribe(*this);
		Reaction::BeginPlay(worldState);
	}

	void ReactionAttributed::OnDestroy(WorldState& worldState)
	{
		Event<EventMessageAttributed>::Unsubscribe(*this);
		Reaction::OnDestroy(worldState);
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

	Scope* ReactionAttributed::Clone(const Scope& rhs) const
	{
		ReactionAttributed& action = *rhs.AssertiveAs<ReactionAttributed>();
		return new ReactionAttributed(action);
	}
}
