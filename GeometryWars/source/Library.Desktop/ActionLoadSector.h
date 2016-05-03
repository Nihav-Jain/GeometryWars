#pragma once
#include "Action.h"

namespace Library
{
	class ActionLoadSector : public Action
	{
		RTTI_DECLARATIONS(ActionLoadSector, Action);

	public:
		ActionLoadSector();
		virtual ~ActionLoadSector() = default;

		ActionLoadSector(const ActionLoadSector& rhs);
		ActionLoadSector(ActionLoadSector&& rhs);

		ActionLoadSector& operator=(const ActionLoadSector& rhs);
		ActionLoadSector& operator=(ActionLoadSector&& rhs);

		virtual void Update(WorldState& worldState) override;
		virtual Scope* Clone(const Scope& rhs) const override;

		static const std::string ATTRIBUTE_SECTOR_NAME;
	};

	CONCRETE_ACTION_FACTORY(ActionLoadSector);
}

