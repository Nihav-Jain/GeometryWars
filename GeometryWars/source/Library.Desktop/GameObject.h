#pragma once
#include "Entity.h"

namespace Library
{
	class GameObject :	public Entity
	{
		RTTI_DECLARATIONS(GameObject, Entity);

	public:

		GameObject();
		virtual ~GameObject() = default;

		const glm::vec4& Position() const;
		void SetPosition(const glm::vec4& position);

		const glm::vec4& Rotation() const;
		void SetRotation(const glm::vec4& rotation);

		const glm::vec4& Scale() const;
		void SetScale(const glm::vec4& scale);

		Action* GetComponent(const std::string& typeName) const;
		bool HasComponent(const std::string& typeName) const;

		virtual void OnOverlapBegin(const GameObject& other);

		static const std::string ATTRIBUTE_POSITION;
		static const std::string ATTRIBUTE_ROTATION;
		static const std::string ATTRIBUTE_SCALE;

	protected:
		glm::vec4 mPosition, mRotation, mScale;

	};

	CONCRETE_ENTITY_FACTORY(GameObject);

}