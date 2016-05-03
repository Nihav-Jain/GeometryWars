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

		const glm::vec4 & Velocity() const;
		void SetVelocity(const glm::vec4 & velocity);

		void EnemyDeath(WorldState& worldState);
		std::int32_t Score() const;

		virtual Scope* Clone(const Scope& rhs) const override;
		virtual void BeginPlay(WorldState& worldState) override;
		virtual void Update(WorldState& worldState) override;
		virtual void OnDestroy(WorldState& worldState) override;
		virtual void OnOverlapBegin(const GameObject& other, WorldState& worldState) override;

		static const std::string ATTRIBUTE_VELOCITY;
		static const std::string ATTRIBUTE_ISDEAD;
		static const std::string ATTRIBUTE_CHANNEL;
		static const std::string ATTRIBUTE_SCORE;

	protected:

		glm::vec4 mVelocity;
		bool mIsDead;
		std::string mCollisionChannel;
		std::int32_t mScore;

	private:

		void ResetAttributePointers();
	};

	CONCRETE_ENTITY_FACTORY(Enemy);
}

