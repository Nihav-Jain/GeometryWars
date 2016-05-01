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

		const glm::vec4 & Velocity() const;
		void SetVelocity(const glm::vec4 & velocity);

		void EnemyDeath(WorldState& worldState);

		virtual void BeginPlay(WorldState& worldState) override;
		virtual void Update(WorldState& worldState) override;
		virtual void OnDestroy(WorldState& worldState) override;
		virtual void OnOverlapBegin(const GameObject& other, WorldState& worldState) override;

		static const std::string ATTRIBUTE_VELOCITY;
		static const std::string ATTRIBUTE_ISDEAD;

	protected:

		glm::vec4 mVelocity;
		bool mIsDead;

	};

	CONCRETE_ENTITY_FACTORY(Enemy);
}

