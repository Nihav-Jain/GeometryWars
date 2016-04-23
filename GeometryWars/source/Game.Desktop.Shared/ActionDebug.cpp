#include "pch.h"
#include "ActionDebug.h"

namespace Library {

	RTTI_DEFINITIONS(ActionDebug)
	ActionDebug::ActionDebug() : mMessage() 
	{
		AddInternalAttribute("message","", 1);
		mMessage = &((*this)["message"].Get<std::string>());
	}
	ActionDebug::~ActionDebug() {}
	void ActionDebug::Update(WorldState & state) 
	{
		ActionList::Update(state);
		OutputDebugStringA((*this)["message"].Get<std::string>().c_str());
	}
}
