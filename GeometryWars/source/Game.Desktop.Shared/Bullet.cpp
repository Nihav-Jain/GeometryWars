#include "pch.h"
#include "../../source/Library.Desktop/Game.h"
#include "../../source/Library.Desktop/SpriteRenderer.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Player.h"

namespace Library
{
	RTTI_DEFINITIONS(Bullet, GameObject);

	const std::string Bullet::ATTRIBUTE_VELOCITY = "velocity";
	const std::string Bullet::ATTRIBUTE_ISDEAD = "isdead";
	const std::string Bullet::ATTRIBUTE_CHANNEL = "bulletchannel";


	Bullet::Bullet()
		: mVelocity(), mIsDead(false), mCollisionChannel()
	{
		AddExternalAttribute(ATTRIBUTE_VELOCITY, 1, &mVelocity);
		AddExternalAttribute(ATTRIBUTE_ISDEAD, 1, &mIsDead);
		AddExternalAttribute(ATTRIBUTE_CHANNEL, 1, &mCollisionChannel);
	}

	const glm::vec4 & Bullet::Velocity() const
	{
		return mVelocity;
	}

	void Bullet::SetVelocity(const glm::vec4 & velocity)
	{
		mVelocity = velocity;
	}

	void Bullet::BulletDeath(WorldState & worldState)
	{
		//MarkForDestroy(worldState);

		UNREFERENCED_PARAMETER(worldState);
		mIsDead = true;
	}

	void Bullet::BeginPlay(WorldState & worldState)
	{
		CircleColliderComponent::sCollidableEntitiesByChannel.Insert(mCollisionChannel, Enemy::TypeIdClass());

		GameObject::BeginPlay(worldState);

		Player& player = *worldState.entity->AssertiveAs<Player>();
		mPosition = player.Position();
		mVelocity = player.Heading() * mMoveSpeed;
		mRotation = player.Heading();
	}

	void Bullet::Update(WorldState & worldState)
	{
		GameObject::Update(worldState);

		mPosition += mVelocity;

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

		// TODO: find a better way to do this
		SpriteRenderer* renderer = GetComponent(SpriteRenderer::TypeName())->AssertiveAs<SpriteRenderer>();
		Renderer::GetInstance()->RemoveRenderable(renderer);
	}

	void Bullet::OnOverlapBegin(const GameObject & other, WorldState & worldState)
	{
		Enemy* enemy = other.AssertiveAs<Enemy>();

		enemy->EnemyDeath(worldState);

		BulletDeath(worldState);
	}

}