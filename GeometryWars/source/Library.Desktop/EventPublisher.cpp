#include "pch.h"
#include "EventPublisher.h"

namespace Library
{
	using namespace std::chrono;

	RTTI_DEFINITIONS(EventPublisher, RTTI);

	EventPublisher::EventPublisher(const Vector<IEventSubscriber*>& subscriberList) :
		mSubscriberList(&subscriberList), mTimeEnqueued(), mDelay()
	{}

	EventPublisher::EventPublisher(const EventPublisher& rhs) :
		mSubscriberList(rhs.mSubscriberList), mTimeEnqueued(rhs.mTimeEnqueued), mDelay(mDelay)
	{}

	EventPublisher::EventPublisher(EventPublisher&& rhs) :
		mSubscriberList(rhs.mSubscriberList), mTimeEnqueued(std::move(rhs.mTimeEnqueued)), mDelay(std::move(mDelay))
	{}

	EventPublisher& EventPublisher::operator=(const EventPublisher& rhs)
	{
		if (this != &rhs)
		{
			mSubscriberList = rhs.mSubscriberList;
			mTimeEnqueued = rhs.mTimeEnqueued;
			mDelay = rhs.mDelay;
		}
		return *this;
	}

	EventPublisher& EventPublisher::operator=(EventPublisher&& rhs)
	{
		if (this != &rhs)
		{
			mSubscriberList = rhs.mSubscriberList;
			mTimeEnqueued = std::move(rhs.mTimeEnqueued);
			mDelay = std::move(rhs.mDelay);
		}
		return *this;
	}

	void EventPublisher::SetTime(const std::chrono::milliseconds& timeEnqueued, std::chrono::milliseconds delay)
	{
		mTimeEnqueued = timeEnqueued;
		mDelay = delay;
	}

	const std::chrono::milliseconds& EventPublisher::TimeEnqueued() const
	{
		return mTimeEnqueued;
	}

	const std::chrono::milliseconds& EventPublisher::Delay() const
	{
		return mDelay;
	}

	bool EventPublisher::IsExpired(const std::chrono::milliseconds& currentTime) const
	{
		if ((currentTime - mTimeEnqueued) > mDelay)
			return true;
		return false;
	}

	void EventPublisher::Deliver()
	{
		for (auto& subscriber : *mSubscriberList)
		{
			subscriber->Notify(*this);
		}
	}

}
