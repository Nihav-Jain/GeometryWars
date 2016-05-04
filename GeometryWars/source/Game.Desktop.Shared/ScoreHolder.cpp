#include "pch.h"
#include "ScoreHolder.h"
#include "Player.h"
#include "ScoreManager.h"


namespace Library
{
	RTTI_DEFINITIONS(ScoreHolder, GameObject);


	ScoreHolder::ScoreHolder()
		: mScore(0), mScoreBase(10), mSetScore(false)
	{
		AddExternalAttribute(Player::ATTRIBUTE_SCORE, 1, &mScore);
		AddExternalAttribute(Player::ATTRIBUTE_SCOREBASE, 1, &mScoreBase);
	}

	ScoreHolder::~ScoreHolder()
	{
		ScoreManager::DeleteInstance();
	}

	Scope* ScoreHolder::Clone(const Scope & rhs) const
	{
		ScoreHolder& clone = *rhs.AssertiveAs<ScoreHolder>();
		return new ScoreHolder(clone);
	}

	void ScoreHolder::BeginPlay(WorldState & worldState)
	{
		UNREFERENCED_PARAMETER(worldState);
	}

	void ScoreHolder::Update(WorldState & worldState)
	{
		UNREFERENCED_PARAMETER(worldState);

		if (!mSetScore)
		{
			mSetScore = true;
			InitScore();
		}
	}

	void ScoreHolder::OnDestroy(WorldState & worldState)
	{
		UNREFERENCED_PARAMETER(worldState);

		ScoreManager::GetInstance()->CleanupSprites();
	}

	void ScoreHolder::InitScore() const
	{
		ScoreManager* score = ScoreManager::CreateInstance();
		score->SetData(mScore, 10, 40, 200, 315, 10, false, "Content//resource//", "digits//", ".png");
		score->SetNumberBase(mScoreBase);
		score->Init();
		score->RefreshSprites();
	}

}