#pragma once
#include <chrono>
#include "RTTI.h"
#include "Vector.h"
#include "IEventSubscriber.h"

namespace Library
{
	/**
	 *	Base class for an Event
	 *	@inherits RTTI
	 */
	class EventPublisher : public RTTI
	{
		RTTI_DECLARATIONS(EventPublisher, RTTI);
	
	public:

		/**
		 *	Constructor
		 *	@param constant reference to the vector of pointers to subscribers of an Event
		 */
		explicit EventPublisher(const Vector<IEventSubscriber*>& subscriberList);

		/**
		 *	Copy Constructor
		 *	@param constant reference to the EventPublisher to be copied
		 */
		EventPublisher(const EventPublisher& rhs);

		/**
		 *	Move Constructor
		 *	@param R value reference to the EventPublisher to be moved
		 */
		EventPublisher(EventPublisher&& rhs);

		/**
		 *	default destructor
		 */
		virtual ~EventPublisher() = default;

		/**
		 *	Copy assignment operator
		 *	@param constant reference to the EventPublisher to be copied
		 *	@return reference to the EventPublisher
		 */
		EventPublisher& operator=(const EventPublisher& rhs);

		/**
		 *	Move assignment operator
		 *	@param R value reference to the EventPublisher to be moved
		 *	@return reference to the EventPublisher
		 */
		EventPublisher& operator=(EventPublisher&& rhs);

		/**
		 *	Sets the time this event was enqueued in the EventQueue
		 *	@param constant reference to chrono milliseconds of total elapsed game time
		 *	@param (optional) chrono milliseconds delay for this event to be fired after being enqueued
		 */
		void SetTime(const std::chrono::milliseconds& timeEnqueued, std::chrono::milliseconds delay = std::chrono::milliseconds());

		/**
		 *	Getter for the time this event was enqueued in the EventQueue
		 *	@return const reference to chrono milliseconds of the elapsed game time when the event was added to the queue
		 */
		const std::chrono::milliseconds& TimeEnqueued() const;

		/**
		 *	Getter for the millisecods to wait after enqueuing to fire the event
		 *	@return const reference to chrono milliseconds
		 */
		const std::chrono::milliseconds& Delay() const;

		/**
		 *	Checks if this event has expired or not - difference in current time and time enqueued must be more than the specified delay
		 *	@param const reference to chrono millisecond representing the current total elapsed game time
		 *	@return true if event has expired, false otherwise
		 */
		bool IsExpired(const std::chrono::milliseconds& currentTime) const;

		/**
		 *	Notifies all the subscribers of this event
		 */
		void Deliver();

	private:
		const Vector<IEventSubscriber*>* mSubscriberList;
		std::chrono::milliseconds mTimeEnqueued;
		std::chrono::milliseconds mDelay;
	};

}

