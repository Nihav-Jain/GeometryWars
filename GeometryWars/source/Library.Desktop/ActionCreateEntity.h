#pragma once
#include "Action.h"

namespace Library
{
	class ActionCreateEntity : public Action
	{
		RTTI_DECLARATIONS(ActionCreateEntity, Action);

	public:
		ActionCreateEntity();
		virtual ~ActionCreateEntity() = default;

		ActionCreateEntity(const ActionCreateEntity& rhs);
		ActionCreateEntity(ActionCreateEntity&& rhs);

		ActionCreateEntity& operator=(const ActionCreateEntity& rhs);
		ActionCreateEntity& operator=(ActionCreateEntity&& rhs);

		virtual void Update(WorldState& worldState) override;
		virtual Scope* Clone(const Scope& rhs) const override;

		static const std::string ATTRIBUTE_ENTITY_CLASS_NAME;
		static const std::string ATTRIBUTE_ENTITY_INSTANCE_NAME;
	};

	CONCRETE_ACTION_FACTORY(ActionCreateEntity);
}

