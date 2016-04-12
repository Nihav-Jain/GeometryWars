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

		/**
		 *	Adds the given event to the queue, to be fired at the spcified delay from the time of enqueue
		 *	@param shared pointer (smart pointer) to the event to be published
		 *	@param reference to GameTime, will be used to set the time this event was enqueued
		 *	@param (optional) delay in milliseconds to fire the event [default: 0 milliseconds]
		 */
		void Enqueue(const std::shared_ptr<EventPublisher>& publisher, const GameTime& gameTime, std::chrono::milliseconds delay = std::chrono::milliseconds());

		/**
		 *	Publishes the given event immediately and removes it from the queue (if enqueued previously)
		 *	@param shared pointer (smart pointer) to the event to be published
		 */
		void Send(const std::shared_ptr<EventPublisher>& publisher);

		/**
		 *	Publishes all the expired events, called every frame by Game
		 *	@param reference to GameTime, will be used to check if the events in the queue have expired or not
		 */
		void Update(const GameTime& gameTime);

		/**
		 *	Fires all expired events and the removes all events from the queue
		 *	@param reference to GameTime, will be used to check if the events in the queue have expired or not
		 */
		void Clear(const GameTime& gameTime);

		/**
		 *	Checks if there are any events waiting to be fired
		 *	@return true if there is at least one event in the queue waiting to be fired
		 */
		bool IsEmpty() const;

		/**
		 *	Gets the number of events waiting to be fired
		 *	@return number of events waiting to be fired
		 */
		std::uint32_t Size() const;

		/**
		 *	Removes an event from the queue, thereby preventing the event from being fired
		 *	@param shared pointer (smart pointer) to the event to be cancelled
		 */
		void CancelEvent(const std::shared_ptr<EventPublisher>& publisher);

	private:
		void DeliverExpiredEvents(const GameTime& gameTime);

		Vector<std::shared_ptr<EventPublisher>> mPublishers;
		Vector<std::shared_ptr<EventPublisher>> mExpiredEvents;
	};
}

