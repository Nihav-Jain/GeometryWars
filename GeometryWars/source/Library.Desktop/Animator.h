#pragma once
#include "ActionList.h"

namespace Library
{
	class AnimationState;
	class SpriteRenderer;

	class Animator : public ActionList
	{
		RTTI_DECLARATIONS(Animator, ActionList);
	public:
		Animator();
		~Animator();

		void SetState(const std::string & stateName);

		virtual void Update(WorldState& worldState) override;

		virtual Scope* Clone(const Scope& rhs) const override;

	private:

		AnimationState * mState;
		SpriteRenderer * mSprite;

		std::chrono::milliseconds mDuration;
		std::uint32_t mSpriteIndex;
		float mX;
		float mY;

		static const std::string ATTRIBUTE_SPRITESHEET;
		static const std::string ATTRIBUTE_SIZE;
	};

	CONCRETE_ACTION_FACTORY(Animator);

}
