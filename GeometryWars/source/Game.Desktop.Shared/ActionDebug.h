#pragma once
#include "Game.h"
#include "Action.h"

namespace Library
{
	class ActionDebug : public ActionList
	{
		RTTI_DECLARATIONS(ActionDebug, ActionList)
	private:
		std::string mMessage;
	public:
		ActionDebug();
		~ActionDebug();
		void Update(WorldState& state) override;
	};
	CONCRETE_ACTION_FACTORY(ActionDebug)
}

