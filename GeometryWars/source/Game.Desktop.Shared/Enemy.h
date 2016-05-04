#pragma once
#include "../../source/Library.Desktop/GameObject.h"

namespace Library
{
	class Enemy : public GameObject
	{
		RTTI_DECLARATIONS(Enemy, GameObject);

	public:
		
		Enemy();
		virtual ~Enemy() = default;

		Enemy(const Enemy& rhs);

		void EnemyDeath(WorldState& worldState, bool canSpawnCollectible = false);
		std::int32_t Score() const;

		void SetCanSpawnCollectible(bool canSpawn);

		virtual Scope* Clone(const Scope& rhs) const override;
		virtual void BeginPlay(WorldState& worldState) override;
		virtual void Update(WorldState& worldState) override;
		virtual void OnDestroy(WorldState& worldState) override;
		virtual void OnOverlapBegin(const GameObject& other, const std::string& channel, WorldState& worldState) override;

		static const std::string ATTRIBUTE_ISDEAD;
		static const std::string ATTRIBUTE_CANSPAWNCOLLECTIBLE;
		static const std::string ATTRIBUTE_CHANNEL;
		static const std::string ATTRIBUTE_SCORE;
		static const std::string ATTRIBUTE_NUMBEROFENEMIES;

	protected:

		bool mIsDead;
		bool mCanSpawnCollectible;
		std::string mCollisionChannel;
		std::int32_t mScore;

	private:

		void ResetAttributePointers();
		static std::int32_t NumberOfEnemies;
	};

	CONCRETE_ENTITY_FACTORY(Enemy);
}

