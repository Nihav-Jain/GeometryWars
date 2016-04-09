#pragma once
#include "ActionList.h"
#include "IEventSubscriber.h"

namespace Library
{
	class Reaction : public ActionList, public IEventSubscriber
	{
		RTTI_DECLARATIONS(Reaction, ActionList);
	public:
		Reaction();
		virtual ~Reaction() = default;

		virtual void Update(WorldState& worldState) override;
	};

#define CONCRETE_REACTION_FACTORY(ConcreteReactionType) CONCRETE_FACTORY(ConcreteReactionType, Reaction);

}

