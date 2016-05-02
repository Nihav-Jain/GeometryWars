#pragma once
#include "Action.h"
namespace Library
{
	class AnimationState : public ActionList
	{
		RTTI_DECLARATIONS(AnimationState, ActionList);

	public:
		AnimationState();
		~AnimationState();

		virtual void Update(WorldState& worldState) {
			(worldState);
		};
	};

	CONCRETE_ACTION_FACTORY(AnimationState);

}
