#pragma once
#include <chrono>
#include "RTTI.h"
#include "Vector.h"
#include "IEventSubscriber.h"

namespace Library
{
	class EventPublisher : public RTTI
	{
		RTTI_DECLARATIONS(EventPublisher, RTTI);
	
	public:
		EventPublisher(const Vector<IEventSubscriber*>& subscriberList);
		EventPublisher(const EventPublisher& rhs);
		EventPublisher(EventPublisher&& rhs);

		virtual ~EventPublisher() = default;

		EventPublisher& operator=(const EventPublisher& rhs);
		EventPublisher& operator=(EventPublisher&& rhs);

		void SetTime(const std::chrono::high_resolution_clock::time_point& timeEnqueued, std::chrono::milliseconds delay = std::chrono::milliseconds());
		const std::chrono::high_resolution_clock::time_point& TimeEnqueued() const;
		const std::chrono::milliseconds& Delay() const;
		bool IsExpired(const std::chrono::high_resolution_clock::time_point& currentTime) const;
		void Deliver();

	private:
		const Vector<IEventSubscriber*>* mSubscriberList;
		std::chrono::high_resolution_clock::time_point mTimeEnqueued;
		std::chrono::milliseconds mDelay;
	};

}

