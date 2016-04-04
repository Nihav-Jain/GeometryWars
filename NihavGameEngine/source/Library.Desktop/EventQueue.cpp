#include "pch.h"
#include "EventQueue.h"

namespace Library
{
	EventQueue::EventQueue() :
		mPublishers(), mExpiredEvents()
	{}

	void EventQueue::Enqueue(EventPublisher& publisher, const GameTime& gameTime, std::chrono::milliseconds delay)
	{
		publisher.SetTime(gameTime.ElapsedGameTime(), delay);
		mPublishers.PushBack(std::shared_ptr<EventPublisher>(&publisher));
	}

	void EventQueue::Send(EventPublisher& publisher)
	{
		publisher.Deliver();
		mPublishers.Remove(std::shared_ptr<EventPublisher>(&publisher));
	}

	void EventQueue::Update(const GameTime& gameTime)
	{
		for (auto& publisher : mPublishers)
		{
			if (publisher->IsExpired(gameTime.ElapsedGameTime()))
			{
				publisher->Deliver();
				mExpiredEvents.PushBack(publisher);
			}
		}

		for (auto& expiredEvent : mExpiredEvents)
		{
			mPublishers.Remove(expiredEvent);
		}

		mExpiredEvents.Clear();
	}

	void EventQueue::Clear(const GameTime& gameTime)
	{
		for (auto& publisher : mPublishers)
		{
			if (publisher->IsExpired(gameTime.ElapsedGameTime()))
			{
				publisher->Deliver();
				mExpiredEvents.PushBack(publisher);
			}
		}
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

	void EventQueue::CancelEvent(EventPublisher& publisher)
	{
		mPublishers.Remove(std::shared_ptr<EventPublisher>(&publisher));
	}

}
