#pragma once
#include "ActionList.h"
#include "IEventSubscriber.h"

namespace Library
{
	/**
	 *	Abstract base class for subscribers of events triggers by Actions. 
	 *	Implement void Notify(const EventPublisher&) to create a concrete Reaction type
	 *	Register your concrete Reaction by using the macro CONCRETE_REACTION_FACTORY(ConcreteReactionType)
	 *	@inherits ActionList, IEventSubscriber
	 */
	class Reaction : public ActionList, public IEventSubscriber
	{
		RTTI_DECLARATIONS(Reaction, ActionList);
	public:
		
		/**
		 *	Default constructor
		 */
		Reaction();

		/**
		 *	Default destructor
		 */
		virtual ~Reaction() = default;

		Reaction(const Reaction& rhs);
		Reaction(Reaction&& rhs);

		Reaction& operator=(const Reaction& rhs);
		Reaction& operator=(Reaction&& rhs);

		/**
		 *	@override from ActionList - empty implementation, does nothing
		 *	Child classes must either invoke ActionList::Update() or override the Update method themselves
		 */
		virtual void Update(WorldState& worldState) override;
	};

#define CONCRETE_REACTION_FACTORY(ConcreteReactionType) CONCRETE_FACTORY(ConcreteReactionType, Reaction);

}

