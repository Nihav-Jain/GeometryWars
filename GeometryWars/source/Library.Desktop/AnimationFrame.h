#pragma once
#include "Action.h"
namespace Library
{
	class AnimationFrame : public Action
	{
		RTTI_DECLARATIONS(AnimationFrame, Action);

	public:
		AnimationFrame();
		~AnimationFrame();

		virtual void Update(WorldState& worldState) {
			(worldState);
		};
	};

	CONCRETE_ACTION_FACTORY(AnimationFrame);

}
