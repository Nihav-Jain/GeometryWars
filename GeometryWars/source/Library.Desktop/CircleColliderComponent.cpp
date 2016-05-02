#include "pch.h"
#include "CircleColliderComponent.h"

namespace Library
{
	RTTI_DEFINITIONS(CircleColliderComponent, Action);

	const std::string CircleColliderComponent::ATTRIBUTE_RADIUS = "radius";
	const std::string CircleColliderComponent::ATTRIBUTE_ENABLED = "enabled";

	CircleColliderComponent::CircleColliderComponent()
		: mRadius(), mEnabled(true), mOwner(nullptr), mCollidableEntities(nullptr)
	{
		AddExternalAttribute(ATTRIBUTE_RADIUS, 1, &mRadius);
		AddExternalAttribute(ATTRIBUTE_ENABLED, 1, &mEnabled);
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

	const GameObject & CircleColliderComponent::Owner() const
	{
		return *mOwner;
	}

	void CircleColliderComponent::Update(WorldState & worldState)
	{
		UNREFERENCED_PARAMETER(worldState);

		if (mCollidableEntities != nullptr)
		{
			for (std::uint32_t i = mCollidableEntities->Size(); i < mCollidableEntities->Size(); ++i)
			{
				GameObject *other = (*mCollidableEntities)[i].As<GameObject>();
				if (other != nullptr && IsColliding(*other))
				{
					mOwner->OnOverlapBegin(*other);
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
		switch (mOwner->Type())
		{
			case GameObject::GameObjectType::ENEMY:
				mCollidableEntities = &mOwner->GetSector()->GetWorld()->FindSector(GameObject::SECTOR_PLAYER)->Entities();
				break;
			case GameObject::GameObjectType::BULLET:
				mCollidableEntities = &mOwner->GetSector()->GetWorld()->FindSector(GameObject::SECTOR_ENEMIES)->Entities();
				break;
			default:
				break;
		}
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
			std::float_t radii = otherCollider->mRadius - mRadius;

			isColliding = ((distX * distX) + (distY * distY) < (radii * radii));
		}

		return isColliding;
	}

}
