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

	Enemy::Enemy()
		: mVelocity(), mIsDead(false)
	{
		AddExternalAttribute(ATTRIBUTE_VELOCITY, 1, &mVelocity);
		AddExternalAttribute(ATTRIBUTE_ISDEAD, 1, &mIsDead);
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
	}

	void Enemy::BeginPlay(WorldState & worldState)
	{
		GameObject::BeginPlay(worldState);
	}

	void Enemy::Update(WorldState & worldState)
	{
		GameObject::Update(worldState);

		// TODO: separate this out for different derived enemy types
		mPosition += mVelocity;

		// Check if out of bounds
		if (mPosition.x > mWorldWidth / 2.0f)
		{
			mPosition.x = mWorldWidth / 2.0f;
			mVelocity.x *= -1.0f;
		}
		else if (mPosition.x < -mWorldWidth / 2.0f)
		{
			mPosition.x = -mWorldWidth / 2.0f;
			mVelocity.x *= -1.0f;
		}

		if (mPosition.y > mWorldHeight / 2.0f)
		{
			mPosition.y = mWorldHeight / 2.0f;
			mVelocity.y *= -1.0f;
		}
		else if (mPosition.y < -mWorldHeight / 2.0f)
		{
			mPosition.y = -mWorldHeight / 2.0f;
			mVelocity.y *= -1.0f;
		}
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

}