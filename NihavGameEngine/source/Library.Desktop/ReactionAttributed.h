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

		ReactionAttributed(const ReactionAttributed& rhs) = delete;	// for now
		ReactionAttributed& operator=(const ReactionAttributed& rhs) = delete; // fpr now

		virtual void Notify(const EventPublisher& publisher) override;
		virtual void CopyMessageAttributes(const Scope& message);

	protected:
		virtual void DefinePrescribedAttributes() override;
	};

	CONCRETE_REACTION_FACTORY(ReactionAttributed);
}

