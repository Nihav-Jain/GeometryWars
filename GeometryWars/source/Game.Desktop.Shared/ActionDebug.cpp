#include "pch.h"
#include "ActionDebug.h"

namespace Library {

	RTTI_DEFINITIONS(ActionDebug)
	ActionDebug::ActionDebug() : mMessage() 
	{
		AddExternalAttribute("message", 1, &mMessage);
	}
	ActionDebug::~ActionDebug() {}
	void ActionDebug::Update(WorldState & state) 
	{
		ActionList::Update(state);
		OutputDebugStringA(mMessage.c_str());
	}
}
