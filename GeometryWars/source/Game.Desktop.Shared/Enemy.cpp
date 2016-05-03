#include "pch.h"
#include "../../source/Library.Desktop/Game.h"
#include "../../source/Library.Desktop/PolygonRenderer.h"
#include "Enemy.h"
#include "Player.h"

namespace Library
{
	RTTI_DEFINITIONS(Enemy, GameObject);

	const std::string Enemy::ATTRIBUTE_VELOCITY = "velocity";
	const std::string Enemy::ATTRIBUTE_ISDEAD = "isdead";
	const std::string Enemy::ATTRIBUTE_CHANNEL = "enemychannel";
	const std::string Enemy::ATTRIBUTE_SCORE = "score";

	Enemy::Enemy()
		: mVelocity(), mIsDead(false), mCollisionChannel(), mScore(0)
	{
		AddExternalAttribute(ATTRIBUTE_VELOCITY, 1, &mVelocity);
		AddExternalAttribute(ATTRIBUTE_ISDEAD, 1, &mIsDead);
		AddExternalAttribute(ATTRIBUTE_CHANNEL, 1, &mCollisionChannel);
		AddExternalAttribute(ATTRIBUTE_SCORE, 1, &mScore);
	}

	Enemy::Enemy(const Enemy & rhs)
		: GameObject::GameObject(rhs), mVelocity(rhs.mVelocity), mIsDead(rhs.mIsDead), mCollisionChannel(rhs.mCollisionChannel), mScore(rhs.mScore)
	{
		ResetAttributePointers();
	}

	const glm::vec4 & Enemy::Velocity() const
	{
		return mVelocity;
	}

	void Enemy::SetVelocity(const glm::vec4 & velocity)
	{
		mVelocity = velocity;
	}

	void Enemy::EnemyDeath(WorldState & worldState)
	{
		//MarkForDestroy(worldState);

		UNREFERENCED_PARAMETER(worldState);
		mIsDead = true;
		GetComponent(CircleColliderComponent::TypeName())->AssertiveAs<CircleColliderComponent>()->SetEnabled(false);
		// TODO: Spawn score multiplier at current location
	}

	std::int32_t Enemy::Score() const
	{
		return mScore;
	}

	Scope * Enemy::Clone(const Scope & rhs) const
	{
		Enemy& entity = *rhs.AssertiveAs<Enemy>();
		return new Enemy(entity);
	}

	void Enemy::BeginPlay(WorldState & worldState)
	{
		CircleColliderComponent::sCollidableEntitiesByChannel.Insert(mCollisionChannel, Player::TypeIdClass());

		GameObject::BeginPlay(worldState);
	}

	void Enemy::Update(WorldState & worldState)
	{
		// Check if out of bounds
		if (mPosition.x > (mWorldWidth / 2.0f) - mScale.x)
		{
			mPosition.x = (mWorldWidth / 2.0f) - mScale.x;
			mVelocity.x *= -1.0f;
		}
		else if (mPosition.x < (-mWorldWidth / 2.0f) + mScale.x)
		{
			mPosition.x = (-mWorldWidth / 2.0f) + mScale.x;
			mVelocity.x *= -1.0f;
		}

		if (mPosition.y > (mWorldHeight / 2.0f) - mScale.y)
		{
			mPosition.y = (mWorldHeight / 2.0f) - mScale.y;
			mVelocity.y *= -1.0f;
		}
		else if (mPosition.y < (-mWorldHeight / 2.0f) + mScale.y)
		{
			mPosition.y = (-mWorldHeight / 2.0f) + mScale.y;
			mVelocity.y *= -1.0f;
		}

		GameObject::Update(worldState);
	}

	void Enemy::OnDestroy(WorldState & worldState)
	{
		GameObject::OnDestroy(worldState);

		// TODO: find a better way to do this
		PolygonRenderer* renderer = GetComponent(PolygonRenderer::TypeName())->AssertiveAs<PolygonRenderer>();
		Renderer::GetInstance()->RemoveRenderable(renderer);
	}

	void Enemy::OnOverlapBegin(const GameObject & other, WorldState & worldState)
	{
		Player* player = other.AssertiveAs<Player>();

		player->PlayerDeath(worldState);
		
		EnemyDeath(worldState);
	}

	void Enemy::ResetAttributePointers()
	{
		(*this)[ATTRIBUTE_VELOCITY].SetStorage(&mVelocity, 1);
		(*this)[ATTRIBUTE_ISDEAD].SetStorage(&mIsDead, 1);
		(*this)[ATTRIBUTE_CHANNEL].SetStorage(&mCollisionChannel, 1);
		(*this)[ATTRIBUTE_SCORE].SetStorage(&mScore, 1);
	}

}