#include "pch.h"
#include "../../source/Library.Desktop/Game.h"
#include "../../source/Library.Desktop/SpriteRenderer.h"
#include "../../source/Library.Desktop/PolygonRenderer.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Player.h"

namespace Library
{
	RTTI_DEFINITIONS(Bullet, GameObject);

	const std::string Bullet::ATTRIBUTE_ISDEAD = "isdead";
	const std::string Bullet::ATTRIBUTE_CHANNEL = "bulletchannel";


	Bullet::Bullet()
		: mIsDead(false), mCollisionChannel(), mPlayerOwner(nullptr)
	{
		AddExternalAttribute(ATTRIBUTE_ISDEAD, 1, &mIsDead);
		AddExternalAttribute(ATTRIBUTE_CHANNEL, 1, &mCollisionChannel);
	}

	Bullet::Bullet(const Bullet & rhs)
		: GameObject::GameObject(rhs), mIsDead(rhs.mIsDead), mCollisionChannel(rhs.mCollisionChannel)
	{
		ResetAttributePointers();
	}

	void Bullet::BulletDeath(WorldState & worldState)
	{
		//MarkForDestroy(worldState);

		UNREFERENCED_PARAMETER(worldState);
		mIsDead = true;
		GetComponent(CircleColliderComponent::TypeName())->AssertiveAs<CircleColliderComponent>()->SetEnabled(false);
	}

	Scope * Bullet::Clone(const Scope & rhs) const
	{
		Bullet& entity = *rhs.AssertiveAs<Bullet>();
		return new Bullet(entity);
	}

	void Bullet::BeginPlay(WorldState & worldState)
	{
		CircleColliderComponent::sCollidableEntitiesByChannel.Insert(mCollisionChannel, Enemy::TypeIdClass());

		Entity* worldStateEntityCache = worldState.entity;
		worldState.entity = this;

		GameObject::BeginPlay(worldState);

		worldState.entity = worldStateEntityCache;

		Player* player = worldState.entity->As<Player>();
		if (player != nullptr)
		{
			mPlayerOwner = player;
		}
	}

	void Bullet::Update(WorldState & worldState)
	{
		GameObject::Update(worldState);

		// Destroy if out of bounds
		if ((mPosition.x > mWorldWidth / 2.0f) ||
			(mPosition.x < - mWorldWidth / 2.0f) ||
			(mPosition.y > mWorldHeight / 2.0f) ||
			(mPosition.y < - mWorldHeight / 2.0f))
		{
			mIsDead = true;
		}
	}

	void Bullet::OnDestroy(WorldState & worldState)
	{
		GameObject::OnDestroy(worldState);
	}

	void Bullet::OnOverlapBegin(const GameObject & other, const std::string& channel, WorldState & worldState)
	{
		if (channel == mCollisionChannel)
		{
			Enemy* enemy = other.AssertiveAs<Enemy>();
			enemy->EnemyDeath(worldState, true);

			if (mPlayerOwner != nullptr)
			{
				mPlayerOwner->AddScore(enemy->Score());
			}
		}	
	}

	void Bullet::ResetAttributePointers()
	{
		(*this)[ATTRIBUTE_ISDEAD].SetStorage(&mIsDead, 1);
		(*this)[ATTRIBUTE_CHANNEL].SetStorage(&mCollisionChannel, 1);
	}

}