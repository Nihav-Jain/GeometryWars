#pragma once
#include "Reaction.h"

namespace Library
{
	/**
	 *	A generic concrete action event subscriber
	 *	Has 2 prescribed attributes - name (STRING) and subtype (STRING - represents the type of events this subscriber will handle)
	 *	@grammar  <reaction name="testReaction" class="ReactionAttributed"> <string name="subtype" value="mousedown" /> ... </reaction>
	 *	@requires XmlParseHelperReaction, ReactionAttributedFactory
	 *	@inherits Reaction
	 */
	class ReactionAttributed : public Reaction
	{
		RTTI_DECLARATIONS(ReactionAttributed, Reaction);
	public:
		
		/**
		 *	Constructor - subscribes itself to Events of type EventMessageAttributed
		 */
		ReactionAttributed();

		/**
		 *	Destructor - unsubscribes itself from Events of type EventMessageAttributed
		 */
		virtual ~ReactionAttributed();

		ReactionAttributed(const ReactionAttributed& rhs);
		ReactionAttributed(ReactionAttributed&& rhs);

		ReactionAttributed& operator=(const ReactionAttributed& rhs);
		ReactionAttributed& operator=(ReactionAttributed&& rhs);

		virtual void BeginPlay(WorldState& worldState) override;
		virtual void OnDestroy(WorldState& worldState) override;
		/**
		 *	@override
		 *	checks if the given puvlisher is EventMessageAttributed and has the same subtype as itself
		 *	copies the auxiliary attributes of the message and then triggers ActionList::Update
		 */
		virtual void Notify(const EventPublisher& publisher) override;
		virtual Scope* Clone(const Scope& rhs) const override;

	};

	CONCRETE_REACTION_FACTORY(ReactionAttributed);
}

