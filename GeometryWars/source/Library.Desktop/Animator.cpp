#include "pch.h"
#include "Animator.h"
#include "AnimationState.h"
#include "SpriteRenderer.h"

namespace Library
{
	RTTI_DEFINITIONS(Animator, ActionList);

	const std::string Animator::ATTRIBUTE_SPRITESHEET = "spritesheet";

	Animator::Animator() :
		mState(nullptr),
		mSprite(nullptr),
		mDuration(0),
		mSpriteIndex(0),
		mX(.0f),
		mY(.0f)
	{
	}


	Animator::~Animator()
	{
	}

	void Animator::SetState(const std::string & stateName)
	{
		mState = FindAction(stateName)->As<AnimationState>();
		mX = (*mState)["x"].Get<float>(0);
		mY = (*mState)["y"].Get<float>(0);
	}

	void Animator::Update(WorldState & worldState)
	{
		if (mState == nullptr) {
			mState = Actions().Get<Scope>(0).As<AnimationState>();
			mX = (*mState)["x"].Get<float>(0);
			mY = (*mState)["y"].Get<float>(0);
		}

		if (mSprite == nullptr) {
			mSprite = FindAction(ATTRIBUTE_SPRITESHEET)->As<SpriteRenderer>();
		}

		std::chrono::milliseconds duration = std::chrono::milliseconds(
				mState->Actions()[mSpriteIndex]["duration_ms"].Get<std::int32_t>(0)
			);
		if (mDuration + worldState.mGameTime->ElapsedGameTime() > duration) {
			std::uint32_t size = mState->Actions().Size();
			if (mSpriteIndex < size - 1) {
				float width = mState->Actions()[mSpriteIndex]["width"].Get<float>(0);
				mSpriteIndex++;
				mX = mX + width;
			}
			else {
				mSpriteIndex = 0;
				mX = (*mState)["x"].Get<float>(0);
			}
			mDuration = mDuration + worldState.mGameTime->ElapsedGameTime() - duration;
		}
		else {
			mDuration += worldState.mGameTime->ElapsedGameTime();
		}

		float width = mState->Actions()[mSpriteIndex]["width"].Get<float>(0);
		float height = mState->Actions()[mSpriteIndex]["height"].Get<float>(0);

		mSprite->SetUV(mX, mY, width, height);
	}
	Scope * Animator::Clone(const Scope & rhs) const
	{
		Animator& action = *rhs.AssertiveAs<Animator>();
		return new Animator(action);
	}
}
