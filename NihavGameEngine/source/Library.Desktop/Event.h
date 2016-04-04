#pragma once
#include "EventPublisher.h"
#include "Vector.h"

namespace Library
{
	template <typename T>
	class Event : public EventPublisher
	{
		RTTI_DECLARATIONS(Event, EventPublisher);
	public:
		Event(const T& payload);
		Event(const Event& rhs);
		Event(Event&& rhs);

		virtual ~Event() = default;

		Event& operator=(const Event& rhs);
		Event& operator=(Event&& rhs);

		static void Subscribe(const IEventSubscriber& subscriber);
		static void Unsubscribe(const IEventSubscriber& subscriber);
		static void UnsubscriberAll();

		const T& Message() const;

	private:
		const T* mMessage;
		static Vector<IEventSubscriber*> mSubscriberList;
	};
}

#include "Event.inl"
