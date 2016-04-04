#pragma once
#include <memory>
#include "Event.h"
#include "Vector.h"
#include "GameTime.h"

namespace Library
{
	class EventQueue final
	{
	public:
		EventQueue();
		~EventQueue() = default;

		EventQueue(const EventQueue& rhs) = delete;
		EventQueue& operator=(const EventQueue& rhs) = delete;

		void Enqueue(EventPublisher& publisher, const GameTime& gameTime, std::chrono::milliseconds delay = std::chrono::milliseconds());
		void Send(EventPublisher& publisher);
		void Update(const GameTime& gameTime);
		void Clear(const GameTime& gameTime);

		bool IsEmpty() const;
		std::uint32_t Size() const;
		void CancelEvent(EventPublisher& publisher);

	private:
		Vector<std::shared_ptr<EventPublisher>> mPublishers;
		Vector<std::shared_ptr<EventPublisher>> mExpiredEvents;
	};
}

