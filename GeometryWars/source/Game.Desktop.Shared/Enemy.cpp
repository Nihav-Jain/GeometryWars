#include "pch.h"
#include "../../source/Library.Desktop/Game.h"
#include "../../source/Library.Desktop/PolygonRenderer.h"
#include "Enemy.h"
#include "Player.h"
#include "../../source/Library.Desktop/ParticleSystem.h"
#include "../../source/Library.Desktop/LineParticle.h"

namespace Library
{
	RTTI_DEFINITIONS(Enemy, GameObject);

	std::int32_t Enemy::NumberOfEnemies = 0;

	const std::string Enemy::ATTRIBUTE_ISDEAD = "isdead";
	const std::string Enemy::ATTRIBUTE_CANSPAWNCOLLECTIBLE = "canspawncollectible";
	const std::string Enemy::ATTRIBUTE_CHANNEL = "enemychannel";
	const std::string Enemy::ATTRIBUTE_SCORE = "score";
	const std::string Enemy::ATTRIBUTE_NUMBEROFENEMIES = "numberOfEnemies";

	Enemy::Enemy()
		: mIsDead(false), mCanSpawnCollectible(false), mCollisionChannel(), mScore(0)
	{
		AddExternalAttribute(ATTRIBUTE_ISDEAD, 1, &mIsDead);
		AddExternalAttribute(ATTRIBUTE_CANSPAWNCOLLECTIBLE, 1, &mCanSpawnCollectible);
		AddExternalAttribute(ATTRIBUTE_CHANNEL, 1, &mCollisionChannel);
		AddExternalAttribute(ATTRIBUTE_SCORE, 1, &mScore);
		AddExternalAttribute(ATTRIBUTE_NUMBEROFENEMIES, 1, &NumberOfEnemies);
	}

	Enemy::Enemy(const Enemy & rhs)
		: GameObject::GameObject(rhs), mIsDead(rhs.mIsDead), mCollisionChannel(rhs.mCollisionChannel), mScore(rhs.mScore)
	{
		ResetAttributePointers();
	}

	void Enemy::EnemyDeath(WorldState & worldState, bool canSpawnCollectible)
	{
		//MarkForDestroy(worldState);

		UNREFERENCED_PARAMETER(worldState);
		mIsDead = true;
		mCanSpawnCollectible = canSpawnCollectible;
		GetComponent(CircleColliderComponent::TypeName())->AssertiveAs<CircleColliderComponent>()->SetEnabled(false);
		// TODO: Spawn score multiplier at current location
	}

	std::int32_t Enemy::Score() const
	{
		return mScore;
	}

	void Enemy::SetCanSpawnCollectible(bool canSpawn)
	{
		mCanSpawnCollectible = canSpawn;
	}

	Scope * Enemy::Clone(const Scope & rhs) const
	{
		Enemy& entity = *rhs.AssertiveAs<Enemy>();
		return new Enemy(entity);
	}

	void Enemy::BeginPlay(WorldState & worldState)
	{
		Entity* worldStateEntityCache = worldState.entity;
		worldState.entity = this;

		CircleColliderComponent::sCollidableEntitiesByChannel.Insert(mCollisionChannel, Player::TypeIdClass());

		GameObject::BeginPlay(worldState);
		worldState.entity = worldStateEntityCache;
	}

	void Enemy::Update(WorldState & worldState)
	{
		glm::vec4 dir = Direction();

		// Check if out of bounds
		if (mPosition.x > (mWorldWidth / 2.0f) - mScale.x)
		{
			mPosition.x = (mWorldWidth / 2.0f) - mScale.x;
			dir.x *= -1.0f;
		}
		else if (mPosition.x < (-mWorldWidth / 2.0f) + mScale.x)
		{
			mPosition.x = (-mWorldWidth / 2.0f) + mScale.x;
			dir.x *= -1.0f;
		}

		if (mPosition.y > (mWorldHeight / 2.0f) - mScale.y)
		{
			mPosition.y = (mWorldHeight / 2.0f) - mScale.y;
			dir.y *= -1.0f;
		}
		else if (mPosition.y < (-mWorldHeight / 2.0f) + mScale.y)
		{
			mPosition.y = (-mWorldHeight / 2.0f) + mScale.y;
			dir.y *= -1.0f;
		}

		SetDirection(dir);

		GameObject::Update(worldState);
	}

	void Enemy::OnDestroy(WorldState & worldState)
	{
		if (HasComponent(PolygonRenderer::TypeName()))
		{
			ParticleSystem<LineParticle> *p = ParticleSystem<LineParticle>::CreateParticleSystem(GetSector(), 6,
				mPosition, mScale * 0.25f, this->FindAction("PolygonRenderer")->Find("color")->Get<glm::vec4>());
			p->SetEnalbe(true);
		}

		GameObject::OnDestroy(worldState);

	}

	void Enemy::OnOverlapBegin(const GameObject & other, const std::string& channel, WorldState & worldState)
	{
		if (channel == mCollisionChannel)
		{
			Player* player = other.AssertiveAs<Player>();
			player->PlayerDeath(worldState);

			EnemyDeath(worldState);
		}

	}

	void Enemy::ResetAttributePointers()
	{
		(*this)[ATTRIBUTE_ISDEAD].SetStorage(&mIsDead, 1);
		(*this)[ATTRIBUTE_CHANNEL].SetStorage(&mCollisionChannel, 1);
		(*this)[ATTRIBUTE_SCORE].SetStorage(&mScore, 1);
	}

}