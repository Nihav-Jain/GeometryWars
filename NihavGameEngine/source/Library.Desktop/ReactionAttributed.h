#pragma once
#include "Reaction.h"

namespace Library
{
	class ReactionAttributed : public Reaction
	{
		RTTI_DECLARATIONS(ReactionAttributed, Reaction);
	public:
		ReactionAttributed();
		virtual ~ReactionAttributed();

		ReactionAttributed(const ReactionAttributed& rhs) = delete;
		ReactionAttributed& operator=(const ReactionAttributed& rhs) = delete;

		virtual void Notify(const EventPublisher& publisher) override;

	protected:
		virtual void DefinePrescribedAttributes() override;
	};

	CONCRETE_REACTION_FACTORY(ReactionAttributed);
}

