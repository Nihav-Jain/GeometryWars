#include "pch.h"
#include "GameObject.h"

namespace Library
{
	RTTI_DEFINITIONS(GameObject);

	const std::string GameObject::ATTRIBUTE_POSITION = "position";
	const std::string GameObject::ATTRIBUTE_ROTATION = "rotation";
	const std::string GameObject::ATTRIBUTE_SCALE = "scale";
	const std::string GameObject::ATTRIBUTE_MOVESPEED = "speed";

	GameObject::GameObject()
		: mPosition(), mRotation(), mScale(1.0f), mMoveSpeed()
	{
		AddExternalAttribute(ATTRIBUTE_POSITION, 1, &mPosition);
		AddExternalAttribute(ATTRIBUTE_ROTATION, 1, &mRotation);
		AddExternalAttribute(ATTRIBUTE_SCALE, 1, &mScale);
		AddExternalAttribute(ATTRIBUTE_MOVESPEED, 1, &mMoveSpeed);
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

	const std::float_t & GameObject::MoveSpeed() const
	{
		return mMoveSpeed;
	}

	void GameObject::SetMoveSpeed(const std::float_t & moveSpeed)
	{
		mMoveSpeed = moveSpeed;
	}

	Action* GameObject::GetComponent(const std::string & typeName) const
	{		
		return FindAction(typeName);
	}

	bool GameObject::HasComponent(const std::string & typeName) const
	{
		return (GetComponent(typeName) != nullptr);
	}

	void GameObject::OnOverlapBegin(const GameObject & other, WorldState& worldState)
	{
		UNREFERENCED_PARAMETER(other);
		UNREFERENCED_PARAMETER(worldState);
	}
}


