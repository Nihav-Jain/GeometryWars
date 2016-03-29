#include "pch.h"
#include "ActionListSwitch.h"

namespace Library
{

#pragma region ActionListSwitch

	RTTI_DEFINITIONS(ActionListSwitch);

	const std::string ActionListSwitch::ATTRIBUTE_SWITCH_VALUE = "switchValue";

	ActionListSwitch::ActionListSwitch() :
		mCaseMap(nullptr)
	{
		Populate();
	}

	ActionListSwitch::ActionListSwitch(const ActionListSwitch& rhs) :
		mCaseMap(nullptr), ActionList(rhs)
	{
		GenerateCaseMap();
	}

	ActionListSwitch::ActionListSwitch(ActionListSwitch&& rhs) :
		mCaseMap(nullptr), ActionList(std::move(rhs))
	{
		if (rhs.mCaseMap != nullptr)
			*mCaseMap = std::move(*rhs.mCaseMap);
		rhs.mCaseMap = nullptr;
	}

	ActionListSwitch::~ActionListSwitch()
	{
		if (mCaseMap != nullptr)
			delete mCaseMap;
	}

	ActionListSwitch& ActionListSwitch::operator=(const ActionListSwitch& rhs)
	{
		if (this != &rhs)
		{
			mCaseMap = nullptr;
			ActionList::operator=(rhs);

			GenerateCaseMap();
		}
		return *this;
	}

	ActionListSwitch& ActionListSwitch::operator=(ActionListSwitch&& rhs)
	{
		if (this != &rhs)
		{
			ActionList::operator=(std::move(rhs));
			if (rhs.mCaseMap != nullptr)
				*mCaseMap = std::move(*rhs.mCaseMap);
			rhs.mCaseMap = nullptr;
		}
		return *this;
	}

	void ActionListSwitch::GenerateCaseMap()
	{
		Datum& cases = Actions();
		if (cases.Size() > 0)
			mCaseMap = new Hashmap<std::string, ActionListSwitchCase*>(cases.Size());

		for (uint32_t i = 0; i < cases.Size(); i++)
		{
			ActionListSwitchCase* switchCase = cases.Get<Scope*>(i)->As<ActionListSwitchCase>();
			if (switchCase == nullptr)
				continue;
			mCaseMap->Insert(switchCase->operator[](ActionListSwitchCase::ATTRIBUTE_CASE_VALUE).Get<std::string>(), switchCase);
		}
	}

	void ActionListSwitch::PostParsingProcess()
	{
		GenerateCaseMap();
	}

	void ActionListSwitch::Update(WorldState& worldState)
	{
		if (mCaseMap == nullptr)
			return;

		Datum* conditionDatum = Search((*this)[ATTRIBUTE_SWITCH_VALUE].Get<std::string>());
		assert(conditionDatum != nullptr);

		Hashmap<std::string, ActionListSwitchCase*>::Iterator caseIterator = mCaseMap->Find(conditionDatum->ToString());
		if (caseIterator == mCaseMap->end())
		{
			// execute default case
		}
		else
		{
			// TODO: handle break and fall through
			ActionListSwitchCase* matchingCase = (*caseIterator).second;
			worldState.action = matchingCase;
			matchingCase->Update(worldState);
		}
	}

	Scope* ActionListSwitch::Clone(const Scope& rhs) const
	{
		if (!rhs.Is(ActionListSwitch::TypeIdClass()))
			throw std::exception("Given Scope reference is not an ActionListSwitch.");
		return new ActionListSwitch(*(rhs.As<ActionListSwitch>()));
	}

	void ActionListSwitch::DefinePrescribedAttributes()
	{
		AddInternalAttribute(ATTRIBUTE_SWITCH_VALUE, "");
	}

#pragma endregion


#pragma region ActionListSwitchCase

	RTTI_DEFINITIONS(ActionListSwitch::ActionListSwitchCase);

	const std::string ActionListSwitch::ActionListSwitchCase::ATTRIBUTE_CASE_VALUE = "caseValue";

	ActionListSwitch::ActionListSwitchCase::ActionListSwitchCase()
	{
		Populate();
	}

	void ActionListSwitch::ActionListSwitchCase::DefinePrescribedAttributes()
	{
		AddInternalAttribute(ATTRIBUTE_CASE_VALUE, "");
	}

#pragma endregion
}
