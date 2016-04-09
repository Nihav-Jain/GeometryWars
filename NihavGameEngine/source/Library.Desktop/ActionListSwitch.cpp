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
		AddInternalAttribute(ATTRIBUTE_SWITCH_VALUE, "");
	}

	ActionListSwitch::~ActionListSwitch()
	{
		if (mCaseMap != nullptr)
			delete mCaseMap;
	}

	void ActionListSwitch::GenerateCaseMap()
	{
		Datum& cases = Actions();
		Datum::DatumType switchType = Datum::DatumType::UNKNOWN;
		if (cases.Size() > 0)
		{
			mCaseMap = new Hashmap<Datum, ActionListSwitchCase*>(cases.Size());
			Datum* conditionDatum = Search((*this)[ATTRIBUTE_SWITCH_VALUE].Get<std::string>());
			if (conditionDatum == nullptr)
			{
				std::stringstream str;
				str << "Undefined variable: " << (*this)[ATTRIBUTE_SWITCH_VALUE].Get<std::string>();
				throw std::exception(str.str().c_str());
			}
			switchType = conditionDatum->Type();
		}
		for (uint32_t i = 0; i < cases.Size(); i++)
		{
			ActionListSwitchCase* switchCase = cases.Get<Scope*>(i)->As<ActionListSwitchCase>();
			if (switchCase == nullptr)
				continue;
			if (switchCase->DefaultCase)
			{
				Adopt(ActionListSwitchCase::ATTRIBUTE_DEFAULT, *switchCase);
			}
			else
			{
				Datum d;
				d.SetType(switchType);
				d.SetFromString(switchCase->operator[](ActionListSwitchCase::ATTRIBUTE_CASE_VALUE).Get<std::string>());
				mCaseMap->Insert(d, switchCase);
			}
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

		Hashmap<Datum, ActionListSwitchCase*>::Iterator caseIterator = mCaseMap->Find(*conditionDatum);
		if (caseIterator == mCaseMap->end())
		{
			// execute default case
			if (IsAttribute(ActionListSwitchCase::ATTRIBUTE_DEFAULT))
			{
				Scope* defaultActionScope = (*this)[ActionListSwitchCase::ATTRIBUTE_DEFAULT].Get<Scope*>();
				ActionListSwitchCase* defaultCase = defaultActionScope->AssertiveAs<ActionListSwitchCase>();

				worldState.action = defaultCase;
				defaultCase->Update(worldState);
			}
		}
		else
		{
			ActionListSwitchCase* matchingCase = (*caseIterator).second;
			worldState.action = matchingCase;
			matchingCase->Update(worldState);

			if (!matchingCase->MustBreak)
			{
				Datum& cases = Actions();
				std::uint32_t i;
				for (i = 0; i < cases.Size(); i++)
				{
					if (cases.Get<Scope*>(i) == matchingCase)
					{
						++i;
						break;
					}
				}
				for (; i < cases.Size(); i++)
				{
					Scope* nextCase = cases.Get<Scope*>(i);
					assert(nextCase->Is(ActionListSwitchCase::TypeIdClass()));
					matchingCase = static_cast<ActionListSwitchCase*>(nextCase);
					worldState.action = matchingCase;
					matchingCase->Update(worldState);
					if (matchingCase->MustBreak)
						break;
				}
			}
		}
	}

#pragma endregion


#pragma region ActionListSwitchCase

	RTTI_DEFINITIONS(ActionListSwitch::ActionListSwitchCase);

	const std::string ActionListSwitch::ActionListSwitchCase::ATTRIBUTE_CASE_VALUE	= "caseValue";
	const std::string ActionListSwitch::ActionListSwitchCase::ATTRIBUTE_BREAK		= "break";
	const std::string ActionListSwitch::ActionListSwitchCase::ATTRIBUTE_DEFAULT		= "default";

	ActionListSwitch::ActionListSwitchCase::ActionListSwitchCase() :
		MustBreak(false), DefaultCase(false)
	{
		AddInternalAttribute(ATTRIBUTE_CASE_VALUE, "");
	}

#pragma endregion
}
