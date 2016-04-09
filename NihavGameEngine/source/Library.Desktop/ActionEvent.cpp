#include "pch.h"
#include <memory>
#include "ActionEvent.h"
#include "Event.h"
#include "EventMessageAttributed.h"

namespace Library
{
	RTTI_DEFINITIONS(ActionEvent);

	const std::string ActionEvent::ATTRIBUTE_SUBTYPE = "subtype";
	const std::string ActionEvent::ATTRIBUTE_DELAY = "delay";
	const std::string ActionEvent::ATTRIBUTE_ARGUMENTS = "arguments";

	ActionEvent::ActionEvent()
	{
		Populate();
	}


	ActionEvent::~ActionEvent()
	{
	}

	void ActionEvent::Update(WorldState& worldState)
	{
		EventMessageAttributed message;
		message.SetSubtype((*this)[ATTRIBUTE_SUBTYPE].Get<std::string>());
		message.SetWorldState(worldState);

		//// copy attributes
		//Datum& attributes = (*this)[ATTRIBUTE_ARGUMENTS];
		//if (attributes.Size() > 0)
		//{
		//	Scope* tableOfAttributes = new Scope(*attributes.Get<Scope*>());
		//	message.Adopt(EventMessageAttributed::ATTRIBUTE_ARGUMENTS, *tableOfAttributes);
		//}

		std::uint32_t i = AuxiliaryBegin();
		std::uint32_t size = Size();
		for (; i < size; i++)
		{
			const SymbolPair& pair = GetPair(i);
			if (pair.second.Type() == Datum::DatumType::TABLE)
			{
				const Datum& nestedScopeDatum = pair.second;
				std::uint32_t j;
				for (j = 0; j < nestedScopeDatum.Size(); j++)
				{
					Scope* nestedScopeAttribute = new Scope(*nestedScopeDatum.Get<Scope*>(j));
					message.Adopt(pair.first, *nestedScopeAttribute);
				}
			}
			else
			{
				message.AppendAuxiliaryAttribute(pair.first) = pair.second;
			}
		}

		std::int32_t delay = (*this)[ATTRIBUTE_DELAY].Get<std::int32_t>();
		std::shared_ptr<Event<EventMessageAttributed>> attributedEvent = std::make_shared<Event<EventMessageAttributed>>(message);
		worldState.world->GetEventQueue().Enqueue(attributedEvent, *worldState.mGameTime, std::chrono::milliseconds(delay));

	}

	void ActionEvent::DefinePrescribedAttributes()
	{
		AddInternalAttribute(ATTRIBUTE_SUBTYPE, "", 0);
		AddInternalAttribute(ATTRIBUTE_DELAY, 0, 1);
	}

}
