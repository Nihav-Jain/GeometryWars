#pragma once
#include "Action.h"
#include "SharedDataTable.h"

namespace Library
{
	class ActionCreateEntityFromFile : public Action
	{
		RTTI_DECLARATIONS(ActionCreateEntityFromFile, Action);

	public:
		ActionCreateEntityFromFile();
		virtual ~ActionCreateEntityFromFile() = default;

		virtual void Update(WorldState& worldState) override;

		static const std::string ATTRIBUTE_FILE_PATH;
	};

	CONCRETE_ACTION_FACTORY(ActionCreateEntityFromFile);
}

