#include "pch.h"
#include "World.h"
#include "EventMessageAttributed.h"

namespace Library
{
	RTTI_DEFINITIONS(EventMessageAttributed, Attributed);

	const std::string EventMessageAttributed::ATTRIBUTE_SUBTYPE = "subtype";
	const std::string EventMessageAttributed::ATTRIBUTE_ARGUMENTS = "arguments";

	EventMessageAttributed::EventMessageAttributed() :
		mSubtype(), mWorldState(nullptr)
	{
		AddExternalAttribute(ATTRIBUTE_SUBTYPE, 1, &mSubtype);
	}

	EventMessageAttributed::EventMessageAttributed(const EventMessageAttributed& rhs) :
		mSubtype(rhs.mSubtype), mWorldState(rhs.mWorldState), Attributed::Attributed(rhs)
	{
		(*this)[ATTRIBUTE_SUBTYPE].SetStorage(&mSubtype, 1);
	}

	EventMessageAttributed::EventMessageAttributed(EventMessageAttributed&& rhs) :
		mSubtype(std::move(rhs.mSubtype)), mWorldState(rhs.mWorldState), Attributed::Attributed(std::move(rhs))
	{
		(*this)[ATTRIBUTE_SUBTYPE].SetStorage(&mSubtype, 1);
		rhs.mWorldState = nullptr;
	}

	EventMessageAttributed& EventMessageAttributed::operator=(const EventMessageAttributed& rhs)
	{
		if (this != &rhs)
		{
			mSubtype = rhs.mSubtype;
			mWorldState = rhs.mWorldState;

			Attributed::operator=(rhs);

			(*this)[ATTRIBUTE_SUBTYPE].SetStorage(&mSubtype, 1);
		}
		return *this;
	}

	EventMessageAttributed& EventMessageAttributed::operator=(EventMessageAttributed&& rhs)
	{
		if (this != &rhs)
		{
			mSubtype = std::move(rhs.mSubtype);
			mWorldState = rhs.mWorldState;

			Attributed::operator=(rhs);

			(*this)[ATTRIBUTE_SUBTYPE].SetStorage(&mSubtype, 1);

			rhs.mWorldState = nullptr;
		}
		return *this;
	}

	void EventMessageAttributed::SetSubtype(const std::string& subtype)
	{
		mSubtype = subtype;
	}

	const std::string& EventMessageAttributed::GetSubtype() const
	{
		return mSubtype;
	}

	void EventMessageAttributed::SetWorldState(WorldState& worldState)
	{
		mWorldState = &worldState;
	}

	WorldState& EventMessageAttributed::GetWorldState() const
	{
		assert(mWorldState != nullptr);
		return *mWorldState;
	}

}
