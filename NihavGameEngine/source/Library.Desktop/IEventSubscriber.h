#pragma once

namespace Library
{
	class EventPublisher;

	/**
	 *	Abstract interface for an Event Subscriber
	 */
	class IEventSubscriber
	{
	public:

		/**
		 *	Default constructor
		 */
		IEventSubscriber() = default;

		/**
		 *	Default destructor
		 */
		virtual ~IEventSubscriber() = default;

		/**
		 *	Called by Event publishers for notifying the subscriber
		 *	NOTE: pure virtual method - needs to be overriden by the child
		 *	@param constant reference to the event publisher
		 */
		virtual void Notify(const EventPublisher& eventMessage) = 0;
	};

}
