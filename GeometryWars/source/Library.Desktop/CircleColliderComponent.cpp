#include "pch.h"
#include "CircleColliderComponent.h"

namespace Library
{
	RTTI_DEFINITIONS(CircleColliderComponent, Action);

	const std::string CircleColliderComponent::ATTRIBUTE_RADIUS = "radius";
	const std::string CircleColliderComponent::ATTRIBUTE_ENABLED = "enabled";
	const std::string CircleColliderComponent::ATTRIBUTE_CHANNEL = "channel";

	Hashmap<std::uint64_t, const Vector<Entity*>> CircleColliderComponent::sCollidableEntitiesByType;

	CircleColliderComponent::CircleColliderComponent()
		: mRadius(), mEnabled(true), mCollisionChannel(), mOwner(nullptr), mCollidableEntities(nullptr)
	{
		AddExternalAttribute(ATTRIBUTE_RADIUS, 1, &mRadius);
		AddExternalAttribute(ATTRIBUTE_ENABLED, 1, &mEnabled);
		AddExternalAttribute(ATTRIBUTE_CHANNEL, 1, &mCollisionChannel);
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
		if (mCollidableEntities != nullptr)
		{
			for (Entity* entity : *mCollidableEntities)
			{
				if (!entity->IsPendingDestroy())
				{
					GameObject *other = entity->AssertiveAs<GameObject>();
					if (IsColliding(*other))
					{
						mOwner->OnOverlapBegin(*other, worldState);
					}
				}
			}
		}		
	}

	void CircleColliderComponent::PostParsingProcess()
	{
		// Get owning GameObject which contains this component
		Scope *parent = GetParent();
		assert(parent != nullptr);
		GameObject *owner = parent->As<GameObject>();
		assert(owner != nullptr);
		mOwner = owner;

		// Get entities that can be collided with
		//std::uint64_t id = owner->TypeIdInstance();
		//if (sCollidableEntitiesByType.ContainsKey(id))
		//{
		//	mCollidableEntities = &sCollidableEntitiesByType[id];
		//}
		//else
		//{
			Sector *sector = parent->GetParent()->AssertiveAs<Sector>();
			Entity *collidableEntityType = sector->FindEntity(mCollisionChannel);

			if (collidableEntityType != nullptr)
			{
				mCollidableEntities = &sector->GetAllEntitiesOfType(collidableEntityType->TypeIdInstance());
				//sCollidableEntitiesByType.Insert(std::pair<std::uint64_t, const Vector<Entity*>>(id, *mCollidableEntities));
			}
		//}
	}

	bool CircleColliderComponent::IsColliding(const GameObject& other) const
	{
		bool isColliding = false;

		Action *otherComponent = other.GetComponent(CircleColliderComponent::TypeName());
		if (otherComponent != nullptr)
		{
			CircleColliderComponent *otherCollider = otherComponent->AssertiveAs<CircleColliderComponent>();

			std::float_t distX = other.Position().x - mOwner->Position().x;
			std::float_t distY = other.Position().y - mOwner->Position().y;

			// Note: right now this only scales with X
			std::float_t radii = (otherCollider->mRadius * other.Scale().x) - (mRadius * mOwner->Scale().x);

			isColliding = ((distX * distX) + (distY * distY) < (radii * radii));
		}

		return isColliding;
	}

}
