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

		/**
		 *	Disallow copy constructor
		 */
		ReactionAttributed(const ReactionAttributed& rhs) = delete;

		/**
		 *	Disallow copy assignment operator
		 */
		ReactionAttributed& operator=(const ReactionAttributed& rhs) = delete;

		/**
		 *	@override
		 *	checks if the given puvlisher is EventMessageAttributed and has the same subtype as itself
		 *	copies the auxiliary attributes of the message and then triggers ActionList::Update
		 */
		virtual void Notify(const EventPublisher& publisher) override;

	protected:
		virtual void DefinePrescribedAttributes() override;
	};

	CONCRETE_REACTION_FACTORY(ReactionAttributed);
}

