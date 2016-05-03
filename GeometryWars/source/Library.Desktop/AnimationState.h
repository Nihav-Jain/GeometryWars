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

		virtual Scope* Clone(const Scope& rhs) const override;
	};

	CONCRETE_ACTION_FACTORY(AnimationState);

}
