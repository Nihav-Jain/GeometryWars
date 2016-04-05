#include "pch.h"
#include "EventQueue.h"

namespace Library
{
	EventQueue::EventQueue() :
		mPublishers(), mExpiredEvents()
	{}

	void EventQueue::Enqueue(const std::shared_ptr<EventPublisher>& publisher, const GameTime& gameTime, std::chrono::milliseconds delay)
	{
		publisher->SetTime(gameTime.ElapsedGameTime(), delay);
		mPublishers.PushBack(publisher);
	}

	void EventQueue::Send(const std::shared_ptr<EventPublisher>& publisher)
	{
		publisher->Deliver();
		mPublishers.Remove(publisher);
	}

	void EventQueue::Update(const GameTime& gameTime)
	{
		DeliverExpiredEvents(gameTime);

		for (auto& expiredEvent : mExpiredEvents)
		{
			mPublishers.Remove(expiredEvent);
		}

		mExpiredEvents.Clear();
	}

	void EventQueue::Clear(const GameTime& gameTime)
	{
		DeliverExpiredEvents(gameTime);
		mPublishers.Clear();
		mExpiredEvents.Clear();
	}

	bool EventQueue::IsEmpty() const
	{
		return mPublishers.IsEmpty();
	}

	std::uint32_t EventQueue::Size() const
	{
		return mPublishers.Size();
	}

	void EventQueue::CancelEvent(const std::shared_ptr<EventPublisher>& publisher)
	{
		mPublishers.Remove(publisher);
	}

	void EventQueue::DeliverExpiredEvents(const GameTime& gameTime)
	{
		for (auto& publisher : mPublishers)
		{
			if (publisher->IsExpired(gameTime.ElapsedGameTime()))
			{
				publisher->Deliver();
				mExpiredEvents.PushBack(publisher);
			}
		}
	}

}
