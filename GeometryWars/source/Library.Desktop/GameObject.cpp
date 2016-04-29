#include "pch.h"
#include "GameObject.h"

namespace Library
{
	RTTI_DEFINITIONS(GameObject, Entity);

	const std::string GameObject::ATTRIBUTE_POSITION = "position";
	const std::string GameObject::ATTRIBUTE_ROTATION = "rotation";
	const std::string GameObject::ATTRIBUTE_SCALE = "scale";

	// TODO : Move these somewhere else?
	const std::string GameObject::SECTOR_PLAYER = "playerSector";
	const std::string GameObject::SECTOR_ENEMIES = "enemiesSector";
	const std::string GameObject::SECTOR_BULLETS = "bulletsSector";


#pragma warning (disable:4592)
	const Hashmap<std::string, GameObject::GameObjectType> GameObject::SectorTypeStrings =
	{
		{ GameObject::SECTOR_PLAYER,	GameObject::GameObjectType::PLAYER },
		{ GameObject::SECTOR_ENEMIES,	GameObject::GameObjectType::ENEMY },
		{ GameObject::SECTOR_BULLETS,	GameObject::GameObjectType::BULLET }
	};
#pragma warning (default:4592)

	GameObject::GameObject()
		: mType(GameObjectType::INVALID), mPosition(), mRotation(), mScale(1.0f)
	{
		AddExternalAttribute(ATTRIBUTE_POSITION, 1, &mPosition);
		AddExternalAttribute(ATTRIBUTE_ROTATION, 1, &mRotation);
		AddExternalAttribute(ATTRIBUTE_SCALE, 1, &mScale);
	}

	const glm::vec4 & GameObject::Position() const
	{
		return mPosition;
	}

	void GameObject::SetPosition(const glm::vec4 & position)
	{
		mPosition = position;
	}

	const glm::vec4 & GameObject::Rotation() const
	{
		return mRotation;
	}

	void GameObject::SetRotation(const glm::vec4 & rotation)
	{
		mRotation = rotation;
	}

	const glm::vec4 & GameObject::Scale() const
	{
		return mScale;
	}

	void GameObject::SetScale(const glm::vec4 & scale)
	{
		mScale = scale;
	}

	GameObject::GameObjectType GameObject::Type() const
	{
		return mType;
	}

	void GameObject::SetType(const std::string & type)
	{
		mType = SectorTypeStrings[type];
	}

	void GameObject::SetType(GameObjectType type)
	{
		mType = type;
	}

	Action* GameObject::GetComponent(const std::string & typeName) const
	{		
		return FindAction(typeName);
	}

	bool GameObject::HasComponent(const std::string & typeName) const
	{
		return (GetComponent(typeName) != nullptr);
	}

	void GameObject::OnOverlapBegin(const GameObject & other)
	{
		UNREFERENCED_PARAMETER(other);
	}
}


