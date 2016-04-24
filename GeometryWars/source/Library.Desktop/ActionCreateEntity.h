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

		virtual void Update(WorldState& worldState) override;

		static const std::string ATTRIBUTE_ENTITY_CLASS_NAME;
		static const std::string ATTRIBUTE_ENTITY_INSTANCE_NAME;
	};

	CONCRETE_ACTION_FACTORY(ActionCreateEntity);
}

