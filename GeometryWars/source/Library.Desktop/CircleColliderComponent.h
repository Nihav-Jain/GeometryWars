#pragma once
#include "Action.h"

namespace Library
{
	class CircleColliderComponent final : public Action
	{
		RTTI_DECLARATIONS(CircleColliderComponent, Action);

	public:
		CircleColliderComponent();
		~CircleColliderComponent() = default;

		const std::float_t& Radius() const;
		void SetRadius(const std::float_t& radius);

		bool IsEnabled() const;
		void SetEnabled(bool isEnabled);

		const GameObject& Owner() const;

		/**
		*	Will be called by the parent GameObject's Update method every frame
		*	Should be used to implement the main logic of a apecialized Action / Component
		*	Pure virual function, needs to be implemented by the children
		*	@param reference to the WorldState
		*/
		virtual void Update(WorldState& worldState) override;

		/**
		*	Will be called by the EndElementHandler of Action parser
		*	Can be used for processing the Action after the whole element has been parsed
		*/
		virtual void PostParsingProcess() override;

		static const std::string ATTRIBUTE_RADIUS;
		static const std::string ATTRIBUTE_ENABLED;
		static const std::string ATTRIBUTE_CHANNEL;

	private:
		std::float_t mRadius;
		bool mEnabled;
		std::string mCollisionChannel;

		GameObject *mOwner;
		const Vector<Entity*> *mCollidableEntities;

		static Hashmap<std::uint64_t, const Vector<Entity*>> sCollidableEntitiesByType;

		bool IsColliding(const GameObject& other) const;
	};

	CONCRETE_ACTION_FACTORY(CircleColliderComponent);
}
