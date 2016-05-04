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

	std::int32_t Bullet::sBulletCount = 0;

	const std::string Bullet::ATTRIBUTE_VELOCITY = "velocity";
	const std::string Bullet::ATTRIBUTE_ISDEAD = "isdead";
	const std::string Bullet::ATTRIBUTE_CHANNEL = "bulletchannel";
	const std::string Bullet::ATTRIBUTE_PLAYEROWNER = "playerOwner";


	Bullet::Bullet()
		: mVelocity(), mIsDead(false), mCollisionChannel(), mPlayerOwner(new Datum())
	{
		AddExternalAttribute(ATTRIBUTE_VELOCITY, 1, &mVelocity);
		AddExternalAttribute(ATTRIBUTE_ISDEAD, 1, &mIsDead);
		AddExternalAttribute(ATTRIBUTE_CHANNEL, 1, &mCollisionChannel);
		AddExternalAttribute(ATTRIBUTE_PLAYEROWNER, 1, &mPlayerOwner);

		ActionExpression::AddFunction("GetBullets", ActionExpression::FunctionDefinition(0, [](const Vector<Datum*>& params)
		{
			assert(params.Size() >= 0);
			Datum result;
			result = std::to_string(Bullet::sBulletCount++);
			if (Bullet::sBulletCount < 0)
				Bullet::sBulletCount = 0;
			return result;
		}
		));
	}
	
	Bullet::~Bullet()
	{
		delete mPlayerOwner;
	}

	Bullet::Bullet(const Bullet & rhs) : GameObject::GameObject(rhs), mVelocity(rhs.mVelocity), 
		mIsDead(rhs.mIsDead), mCollisionChannel(rhs.mCollisionChannel), mPlayerOwner(new Datum(*rhs.mPlayerOwner))
	{
		ResetAttributePointers();
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

		worldState.entity = worldStateEntityCache;

		//mPlayerOwner = worldState.entity->AssertiveAs<Scope>();
		*mPlayerOwner = *worldState.entity;

		mRotation.z = atan2(mVelocity.y, mVelocity.x) - 1.571f;

		GameObject::BeginPlay(worldState);
	}

	void Bullet::Update(WorldState & worldState)
	{
		GameObject::Update(worldState);

		//mPosition += mVelocity;

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
		PolygonRenderer* renderer = GetComponent(PolygonRenderer::TypeName())->AssertiveAs<PolygonRenderer>();
		Renderer::GetInstance()->RemoveRenderable(renderer);
	}

	void Bullet::OnOverlapBegin(const GameObject & other, WorldState & worldState)
	{
		Enemy* enemy = other.AssertiveAs<Enemy>();

		enemy->EnemyDeath(worldState, true);

		mPlayerOwner->Get<Scope>().AssertiveAs<Player>()->AddScore( enemy->Score() );
	}

	void Bullet::ResetAttributePointers()
	{
		(*this)[ATTRIBUTE_VELOCITY].SetStorage(&mVelocity, 1);
		(*this)[ATTRIBUTE_ISDEAD].SetStorage(&mIsDead, 1);
		(*this)[ATTRIBUTE_CHANNEL].SetStorage(&mCollisionChannel, 1);
		(*this)[ATTRIBUTE_PLAYEROWNER].SetStorage(&mPlayerOwner, 1);
	}

}