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

		ActionCreateEntityFromFile(const ActionCreateEntityFromFile& rhs);
		ActionCreateEntityFromFile(ActionCreateEntityFromFile&& rhs);

		ActionCreateEntityFromFile& operator=(const ActionCreateEntityFromFile& rhs);
		ActionCreateEntityFromFile& operator=(ActionCreateEntityFromFile&& rhs);

		virtual void Update(WorldState& worldState) override;

		virtual Scope* Clone(const Scope& rhs) const override;

		static const std::string ATTRIBUTE_FILE_PATH;
	};

	CONCRETE_ACTION_FACTORY(ActionCreateEntityFromFile);
}

