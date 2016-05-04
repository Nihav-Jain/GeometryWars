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
	const CollectibleFactory Collectible::cf;

	Collectible::Collectible()
		: mIsCollected(false), mCollisionChannel()
	{
		AddExternalAttribute(ATTRIBUTE_ISCOLLECTED, 1, &mIsCollected);
		AddExternalAttribute(ATTRIBUTE_CHANNEL, 1, &mCollisionChannel);
	}

	Collectible::Collectible(const Collectible & rhs)
		: GameObject::GameObject(rhs), mIsCollected(rhs.mIsCollected), mCollisionChannel(rhs.mCollisionChannel)
	{
		ResetAttributePointers();
	}

	Scope * Collectible::Clone(const Scope & rhs) const
	{
		Collectible& entity = *rhs.AssertiveAs<Collectible>();
		return new Collectible(entity);
	}

	void Collectible::BeginPlay(WorldState & worldState)
	{
		CircleColliderComponent::sCollidableEntitiesByChannel.Insert(mCollisionChannel, Player::TypeIdClass());

		Entity* worldStateEntityCache = worldState.entity;
		worldState.entity = this;

		GameObject::BeginPlay(worldState);

		worldState.entity = worldStateEntityCache;
	}

	void Collectible::OnDestroy(WorldState & worldState)
	{
		GameObject::OnDestroy(worldState);

		// TODO: find a better way to do this
		PolygonRenderer* renderer = GetComponent(PolygonRenderer::TypeName())->AssertiveAs<PolygonRenderer>();
		Renderer::GetInstance()->RemoveRenderable(renderer);
	}

	void Collectible::OnOverlapBegin(const GameObject & other, const std::string& channel, WorldState &)
	{
		if (channel == mCollisionChannel)
		{
			Player* player = other.AssertiveAs<Player>();
			player->IncrementMultiplier();

			mIsCollected = true;
			GetComponent(CircleColliderComponent::TypeName())->AssertiveAs<CircleColliderComponent>()->SetEnabled(false);
		}
	}

	void Collectible::ResetAttributePointers()
	{
		(*this)[ATTRIBUTE_ISCOLLECTED].SetStorage(&mIsCollected, 1);
		(*this)[ATTRIBUTE_CHANNEL].SetStorage(&mCollisionChannel, 1);
	}
}