#include "pch.h"
#include "ActionListSwitch.h"

namespace Library
{

#pragma region ActionListSwitch

	RTTI_DEFINITIONS(ActionListSwitch, ActionList);

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

	ActionListSwitch::ActionListSwitch(const ActionListSwitch& rhs) :
		ActionList::ActionList(rhs), mCaseMap(nullptr)
	{
		if (rhs.mCaseMap != nullptr)
			mCaseMap = new Hashmap<Datum, ActionListSwitchCase*>(*rhs.mCaseMap);
	}

	ActionListSwitch::ActionListSwitch(ActionListSwitch&& rhs) :
		ActionList::ActionList(std::move(rhs)), mCaseMap(rhs.mCaseMap)
	{
		rhs.mCaseMap = nullptr;
	}

	ActionListSwitch& ActionListSwitch::operator=(const ActionListSwitch& rhs)
	{
		if (this != &rhs)
		{
			if(rhs.mCaseMap != nullptr)
				*mCaseMap = *rhs.mCaseMap;
			else
			{
				delete mCaseMap;
				mCaseMap = nullptr;
			}
			ActionList::operator=(rhs);
		}
		return *this;
	}

	ActionListSwitch& ActionListSwitch::operator=(ActionListSwitch&& rhs)
	{
		if (this != &rhs)
		{
			mCaseMap = rhs.mCaseMap;
			rhs.mCaseMap = nullptr;
			ActionList::operator=(std::move(rhs));
		}
		return *this;
	}

	void ActionListSwitch::GenerateCaseMap()
	{
		Datum& cases = Actions();
		Datum::DatumType switchType = Datum::DatumType::UNKNOWN;
		if (cases.Size() > 0)
		{
			delete mCaseMap;
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
			ActionListSwitchCase* switchCase = cases.Get<Scope>(i).As<ActionListSwitchCase>();
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

	void ActionListSwitch::BeginPlay(WorldState& worldState)
	{
		ActionList::BeginPlay(worldState);
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
				Scope& defaultActionScope = (*this)[ActionListSwitchCase::ATTRIBUTE_DEFAULT].Get<Scope>();
				ActionListSwitchCase* defaultCase = defaultActionScope.AssertiveAs<ActionListSwitchCase>();

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
					if (&cases.Get<Scope>(i) == matchingCase)
					{
						++i;
						break;
					}
				}
				for (; i < cases.Size(); i++)
				{
					Scope& nextCase = cases.Get<Scope>(i);
					assert(nextCase.Is(ActionListSwitchCase::TypeIdClass()));
					matchingCase = static_cast<ActionListSwitchCase*>(&nextCase);
					worldState.action = matchingCase;
					matchingCase->Update(worldState);
					if (matchingCase->MustBreak)
						break;
				}
			}
		}
	}

	Scope* ActionListSwitch::Clone(const Scope& rhs) const
	{
		ActionListSwitch& action = *rhs.AssertiveAs<ActionListSwitch>();
		return new ActionListSwitch(action);
	}

#pragma endregion


#pragma region ActionListSwitchCase

	RTTI_DEFINITIONS(ActionListSwitch::ActionListSwitchCase, ActionList);

	const std::string ActionListSwitch::ActionListSwitchCase::ATTRIBUTE_CASE_VALUE	= "caseValue";
	const std::string ActionListSwitch::ActionListSwitchCase::ATTRIBUTE_BREAK		= "break";
	const std::string ActionListSwitch::ActionListSwitchCase::ATTRIBUTE_DEFAULT		= "default";

	ActionListSwitch::ActionListSwitchCase::ActionListSwitchCase() :
		MustBreak(false), DefaultCase(false)
	{
		AddInternalAttribute(ATTRIBUTE_CASE_VALUE, "");
	}

	ActionListSwitch::ActionListSwitchCase::ActionListSwitchCase(const ActionListSwitchCase& rhs) :
		ActionList::ActionList(rhs), MustBreak(rhs.MustBreak), DefaultCase(rhs.DefaultCase)
	{}

	ActionListSwitch::ActionListSwitchCase::ActionListSwitchCase(ActionListSwitchCase&& rhs) :
		ActionList::ActionList(std::move(rhs)), MustBreak(rhs.MustBreak), DefaultCase(rhs.DefaultCase)
	{}

	ActionListSwitch::ActionListSwitchCase& ActionListSwitch::ActionListSwitchCase::operator=(const ActionListSwitchCase& rhs)
	{
		if (this != &rhs)
		{
			MustBreak = rhs.MustBreak;
			DefaultCase = rhs.DefaultCase;

			ActionList::operator=(rhs);
		}
		return *this;
	}

	ActionListSwitch::ActionListSwitchCase& ActionListSwitch::ActionListSwitchCase::operator=(ActionListSwitchCase&& rhs)
	{
		if (this != &rhs)
		{
			MustBreak = rhs.MustBreak;
			DefaultCase = rhs.DefaultCase;

			ActionList::operator=(std::move(rhs));
		}
		return *this;
	}

	Scope* ActionListSwitch::ActionListSwitchCase::Clone(const Scope& rhs) const
	{
		ActionListSwitchCase& action = *rhs.AssertiveAs<ActionListSwitchCase>();
		return new ActionListSwitchCase(action);
	}

#pragma endregion
}
