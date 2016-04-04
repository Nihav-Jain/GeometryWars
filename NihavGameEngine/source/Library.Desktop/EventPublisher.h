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
		explicit EventPublisher(const Vector<IEventSubscriber*>& subscriberList);
		EventPublisher(const EventPublisher& rhs);
		EventPublisher(EventPublisher&& rhs);

		virtual ~EventPublisher() = default;

		EventPublisher& operator=(const EventPublisher& rhs);
		EventPublisher& operator=(EventPublisher&& rhs);

		void SetTime(const std::chrono::milliseconds& timeEnqueued, std::chrono::milliseconds delay = std::chrono::milliseconds());
		const std::chrono::milliseconds& TimeEnqueued() const;
		const std::chrono::milliseconds& Delay() const;
		bool IsExpired(const std::chrono::milliseconds& currentTime) const;
		void Deliver();

	private:
		const Vector<IEventSubscriber*>* mSubscriberList;
		std::chrono::milliseconds mTimeEnqueued;
		std::chrono::milliseconds mDelay;
	};

}

