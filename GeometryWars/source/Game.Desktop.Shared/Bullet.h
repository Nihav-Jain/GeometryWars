#pragma once
#include "../../source/Library.Desktop/GameObject.h"

namespace Library
{
	class Bullet : public GameObject
	{
		RTTI_DECLARATIONS(Bullet, GameObject);

	public:

		Bullet();
		virtual ~Bullet() = default;

		Bullet(const Bullet& rhs);

		void BulletDeath(WorldState& worldState);

		virtual Scope* Clone(const Scope& rhs) const override;
		virtual void BeginPlay(WorldState& worldState) override;
		virtual void Update(WorldState& worldState) override;
		virtual void OnDestroy(WorldState& worldState) override;
		virtual void OnOverlapBegin(const GameObject& other, const std::string& channel, WorldState& worldState) override;

		static const std::string ATTRIBUTE_ISDEAD;
		static const std::string ATTRIBUTE_CHANNEL;

	protected:

		bool mIsDead;
		std::string mCollisionChannel;

	private:

		void ResetAttributePointers();
		class Player* mPlayerOwner;
	};

	CONCRETE_ENTITY_FACTORY(Bullet);
}
