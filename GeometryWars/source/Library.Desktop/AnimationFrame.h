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

		virtual Scope* Clone(const Scope& rhs) const override;
	};

	CONCRETE_ACTION_FACTORY(AnimationFrame);

}
