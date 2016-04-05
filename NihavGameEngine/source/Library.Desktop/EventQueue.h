#pragma once
#include <memory>
#include "Event.h"
#include "Vector.h"
#include "GameTime.h"

namespace Library
{
	/**
	 *	Class to manage a time based event queue
	 */
	class EventQueue final
	{
	public:
		/**
		 *	default constructor
		 */
		EventQueue();

		/**
		 *	default destructor
		 */
		~EventQueue() = default;

		/**
		 *	disallowed copy constructor
		 */
		EventQueue(const EventQueue& rhs) = delete;

		/**
		 *	disallowed copy constructor
		 */
		EventQueue& operator=(const EventQueue& rhs) = delete;

		void Enqueue(const std::shared_ptr<EventPublisher>& publisher, const GameTime& gameTime, std::chrono::milliseconds delay = std::chrono::milliseconds());
		void Send(const std::shared_ptr<EventPublisher>& publisher);
		void Update(const GameTime& gameTime);
		void Clear(const GameTime& gameTime);

		bool IsEmpty() const;
		std::uint32_t Size() const;
		void CancelEvent(const std::shared_ptr<EventPublisher>& publisher);

	private:
		void DeliverExpiredEvents(const GameTime& gameTime);

		Vector<std::shared_ptr<EventPublisher>> mPublishers;
		Vector<std::shared_ptr<EventPublisher>> mExpiredEvents;
	};
}

