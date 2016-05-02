#pragma once
#include "Action.h"

namespace Library
{
	class ActionDestroyEntity : public Action
	{
		RTTI_DECLARATIONS(ActionDestroyEntity, Action);

	public:
		ActionDestroyEntity();
		virtual ~ActionDestroyEntity() = default;

		ActionDestroyEntity(const ActionDestroyEntity& rhs);
		ActionDestroyEntity(ActionDestroyEntity&& rhs);

		ActionDestroyEntity& operator=(const ActionDestroyEntity& rhs);
		ActionDestroyEntity& operator=(ActionDestroyEntity&& rhs);

		virtual void Update(WorldState& worldState) override;

		virtual Scope* Clone(const Scope& rhs) const override;

		static const std::string ATTRIBUTE_ENTITY_INSTANCE_NAME;

	private:
		std::string mEntityToDestroy;
	};

	CONCRETE_ACTION_FACTORY(ActionDestroyEntity);
}

