#pragma once

#include "../../source/Library.Desktop/Game.h"
#include "../../source/Library.Desktop/GameObject.h"

namespace Library
{
	class ScoreHolder : public GameObject
	{
		RTTI_DECLARATIONS(ScoreHolder, GameObject);

	public:

		ScoreHolder();
		~ScoreHolder();

		virtual Scope* Clone(const Scope& rhs) const override;
		virtual void BeginPlay(WorldState& worldState) override;
		virtual void Update(WorldState& worldState) override;
		virtual void OnDestroy(WorldState& worldState) override;

		void InitScore() const;

	private:

		std::int32_t mScore;
		std::int32_t mScoreBase;
		bool mSetScore;
	};

	CONCRETE_ENTITY_FACTORY(ScoreHolder);
}
