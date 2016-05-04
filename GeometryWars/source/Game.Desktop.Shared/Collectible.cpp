#include "pch.h"
#include "../../source/Library.Desktop/Game.h"
#include "../../source/Library.Desktop/PolygonRenderer.h"
#include "Collectible.h"
#include "Player.h"

namespace Library
{
	RTTI_DEFINITIONS(Collectible, GameObject);

	const std::string Collectible::ATTRIBUTE_ISCOLLECTED = "iscollected";
	const std::string Collectible::ATTRIBUTE_CHANNEL = "collectiblechannel";
	const std::string Collectible::ATTRIBUTE_MOVE_CHANNEL = "movechannel";

	Collectible::Collectible()
		: mIsCollected(false), mCollisionChannel(), mMoveChannel()
	{
		AddExternalAttribute(ATTRIBUTE_ISCOLLECTED, 1, &mIsCollected);
		AddExternalAttribute(ATTRIBUTE_CHANNEL, 1, &mCollisionChannel);
		AddExternalAttribute(ATTRIBUTE_MOVE_CHANNEL, 1, &mMoveChannel);
	}

	Collectible::Collectible(const Collectible & rhs)
		: GameObject::GameObject(rhs), mIsCollected(rhs.mIsCollected), mCollisionChannel(rhs.mCollisionChannel), mMoveChannel(rhs.mMoveChannel)
	{
		ResetAttributePointers();
	}

	Scope * Collectible::Clone(const Scope & rhs) const
	{
		Collectible& entity = *rhs.AssertiveAs<Collectible>();
		return new Collectible(entity);
	}

	void Collectible::SetIsCollected(bool isCollected)
	{
		mIsCollected = isCollected;
	}

	void Collectible::BeginPlay(WorldState & worldState)
	{
		CircleColliderComponent::sCollidableEntitiesByChannel.Insert(mCollisionChannel, Player::TypeIdClass());
		CircleColliderComponent::sCollidableEntitiesByChannel.Insert(mMoveChannel, Player::TypeIdClass());

		Entity* worldStateEntityCache = worldState.entity;
		worldState.entity = this;

		GameObject::BeginPlay(worldState);

		worldState.entity = worldStateEntityCache;
	}

	void Collectible::Update(WorldState & worldState)
	{
		SetDirection(glm::vec4(0, 0, 0, 0));

		GameObject::Update(worldState);
	}

	void Collectible::OnDestroy(WorldState & worldState)
	{
		GameObject::OnDestroy(worldState);

		PolygonRenderer* renderer = GetComponent(PolygonRenderer::TypeName())->AssertiveAs<PolygonRenderer>();
		Renderer::GetInstance()->RemoveRenderable(renderer);
	}

	void Collectible::OnOverlapBegin(const GameObject & other, const std::string& channel, WorldState& worldState)
	{
		UNREFERENCED_PARAMETER(worldState);

		if (channel == mCollisionChannel && !mIsCollected)
		{
			Player* player = other.AssertiveAs<Player>();
			player->IncrementMultiplier();

			mIsCollected = true;
			
			GetComponent(CircleColliderComponent::TypeName())->AssertiveAs<CircleColliderComponent>()->SetEnabled(false);
		}
		else if (channel == mMoveChannel && !mIsCollected)
		{
			Player* player = other.AssertiveAs<Player>();
			SetDirection(glm::normalize(player->Position() - mPosition));
		}
	}

	void Collectible::ResetAttributePointers()
	{
		(*this)[ATTRIBUTE_ISCOLLECTED].SetStorage(&mIsCollected, 1);
		(*this)[ATTRIBUTE_CHANNEL].SetStorage(&mCollisionChannel, 1);
		(*this)[ATTRIBUTE_MOVE_CHANNEL].SetStorage(&mMoveChannel, 1);
	}
}