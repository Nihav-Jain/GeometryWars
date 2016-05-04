#pragma once
#include "../../source/Library.Desktop/GameObject.h"

namespace Library
{
	class Bullet : public GameObject
	{
		RTTI_DECLARATIONS(Bullet, GameObject);

	public:

		Bullet();
		virtual ~Bullet();

		Bullet(const Bullet& rhs);

		const glm::vec4 & Velocity() const;
		void SetVelocity(const glm::vec4 & velocity);

		void BulletDeath(WorldState& worldState);

		virtual Scope* Clone(const Scope& rhs) const override;
		virtual void BeginPlay(WorldState& worldState) override;
		virtual void Update(WorldState& worldState) override;
		virtual void OnDestroy(WorldState& worldState) override;
		virtual void OnOverlapBegin(const GameObject& other, const std::string& channel, WorldState& worldState) override;

		static const std::string ATTRIBUTE_PLAYEROWNER;
		static const std::string ATTRIBUTE_VELOCITY;
		static const std::string ATTRIBUTE_ISDEAD;
		static const std::string ATTRIBUTE_CHANNEL;

	protected:

		glm::vec4 mVelocity;
		bool mIsDead;
		std::string mCollisionChannel;

	private:

		void ResetAttributePointers();
		Datum* mPlayerOwner;
	};

	CONCRETE_ENTITY_FACTORY(Bullet);
}
