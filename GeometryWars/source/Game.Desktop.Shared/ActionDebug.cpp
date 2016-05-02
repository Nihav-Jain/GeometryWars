#include "pch.h"
#include <Windows.h>
#include "ActionDebug.h"

namespace Library {

	RTTI_DEFINITIONS(ActionDebug, ActionList)
	ActionDebug::ActionDebug()
	{
		AddInternalAttribute("message", "", 1);
	}
	ActionDebug::~ActionDebug() {}
	void ActionDebug::Update(WorldState & state) 
	{
		ActionList::Update(state);
		Datum& messages = (*this)["message"];
		for (std::uint32_t i = 0; i < messages.Size(); ++i)
		{
			std::string& message = messages.Get<std::string>(i);
			if (!message.empty())
			{
				OutputDebugStringA(message.c_str());
				OutputDebugStringA("\n");
			}
		}
	}
}
