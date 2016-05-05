#include "pch.h"
#include "CircleColliderComponent.h"
#include <limits>

namespace Library
{
	RTTI_DEFINITIONS(CircleColliderComponent, Action);

	const std::string CircleColliderComponent::ATTRIBUTE_RADIUS = "radius";
	const std::string CircleColliderComponent::ATTRIBUTE_ENABLED = "enabled";
	const std::string CircleColliderComponent::ATTRIBUTE_CHANNEL = "channel";

	Hashmap<std::string, std::uint64_t> CircleColliderComponent::sCollidableEntitiesByChannel;

	CircleColliderComponent::CircleColliderComponent()
		: mRadius(), mEnabled(true), mCollisionChannel(), mOwner(nullptr), mCollidableEntities(nullptr)
	{
		AddExternalAttribute(ATTRIBUTE_RADIUS, 1, &mRadius);
		AddExternalAttribute(ATTRIBUTE_ENABLED, 1, &mEnabled);
		AddExternalAttribute(ATTRIBUTE_CHANNEL, 1, &mCollisionChannel);
	}

	CircleColliderComponent::CircleColliderComponent(const CircleColliderComponent & rhs)
		: Action::Action(rhs), mRadius(rhs.mRadius), mEnabled(rhs.mEnabled), mCollisionChannel(rhs.mCollisionChannel), mOwner(nullptr), mCollidableEntities(nullptr)
	{	
		ResetAttributePointers();
	}

	const std::float_t & CircleColliderComponent::Radius() const
	{
		return mRadius;
	}

	void CircleColliderComponent::SetRadius(const std::float_t& radius)
	{
		if (radius >= 0.0f)
		{
			mRadius = radius;
		}
	}

	bool CircleColliderComponent::IsEnabled() const
	{
		return mEnabled;
	}

	void CircleColliderComponent::SetEnabled(bool isEnabled)
	{
		mEnabled = isEnabled;
	}

	const GameObject& CircleColliderComponent::Owner() const
	{
		return *mOwner;
	}

	void CircleColliderComponent::Update(WorldState & worldState)
	{
		if (mEnabled && mCollidableEntities != nullptr)
		{
			// TODO: Optimize this and/or allow option for searching for closest collision
			GameObject *closestCollidee = nullptr;
			std::float_t closestDist = std::numeric_limits<std::float_t>::max();
			for (Entity* entity : *mCollidableEntities)
			{
				GameObject *other = entity->AssertiveAs<GameObject>();
				if (IsColliding(*other))
				{
					std::float_t dist = glm::distance(mOwner->Position(), other->Position());
					if (dist < closestDist)
					{
						closestCollidee = other;
						closestDist = dist;
					}				
				}
			}

			if (closestCollidee != nullptr)
			{
				mOwner->OnOverlapBegin(*closestCollidee, mCollisionChannel, worldState);
			}
		}		
	}

	Scope* CircleColliderComponent::Clone(const Scope & rhs) const
	{
		CircleColliderComponent& action = *rhs.AssertiveAs<CircleColliderComponent>();
		return new CircleColliderComponent(action);
	}

	void CircleColliderComponent::BeginPlay(WorldState & worldState)
	{
		// Get owning GameObject which contains this component
		GameObject *owner = GetParent()->As<GameObject>();
		assert(owner != nullptr);
		mOwner = owner;

		// Get entities that can be collided with
		if (sCollidableEntitiesByChannel.ContainsKey(mCollisionChannel))
		{
			std::uint64_t id = sCollidableEntitiesByChannel[mCollisionChannel];
			mCollidableEntities = &worldState.sector->GetAllEntitiesOfType(id);
		}
	}

	bool CircleColliderComponent::IsColliding(const GameObject& other) const
	{
		bool isColliding = false;

		Action *otherComponent = other.GetComponent(CircleColliderComponent::TypeName());
		if (otherComponent != nullptr)
		{
			CircleColliderComponent *otherCollider = otherComponent->AssertiveAs<CircleColliderComponent>();

			if (otherCollider->mEnabled)
			{
				std::float_t distX = other.Position().x - mOwner->Position().x;
				std::float_t distY = other.Position().y - mOwner->Position().y;

				// Note: right now this only scales with X
				std::float_t radii = (otherCollider->mRadius * other.Scale().x) - (mRadius * mOwner->Scale().x);

				isColliding = ((distX * distX) + (distY * distY) < (radii * radii));
			}
		}

		return isColliding;
	}

	void CircleColliderComponent::ResetAttributePointers()
	{
		(*this)[ATTRIBUTE_RADIUS].SetStorage(&mRadius, 1);
		(*this)[ATTRIBUTE_ENABLED].SetStorage(&mEnabled, 1);
		(*this)[ATTRIBUTE_CHANNEL].SetStorage(&mCollisionChannel, 1);
	}
}
