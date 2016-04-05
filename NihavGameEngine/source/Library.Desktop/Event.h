#pragma once
#include "EventPublisher.h"
#include "Vector.h"

namespace Library
{
	/**
	 *	Templated Event class where T is the data type of the payload this event will deliver
	 *	@inherits EventPublisher
	 */
	template <typename T>
	class Event final : public EventPublisher
	{
		RTTI_DECLARATIONS(Event, EventPublisher);
	public:

		/**
		 *	Constructor
		 *	@param constant reference to the payload to be carried by this Event
		 */
		explicit Event(const T& payload);

		/**
		 *	Constructor (overload), moves the payload instad of copying it, thus possibly improving performance
		 *	@param R value reference to the payload to be carried by this Event
		 */
		explicit Event(T&& payload);

		/**
		 *	Copy Constructor
		 *	@param constant reference to the Event to be copied
		 */
		Event(const Event& rhs);

		/**
		 *	Move Constructor
		 *	@param R value reference to the Event to be moved
		 */
		Event(Event&& rhs);

		/**
		 *	Default destructor
		 */
		virtual ~Event() = default;

		/**
		 *	copy assigment operator
		 *	@param constant reference to the Event to be copied
		 *	@return reference to the copied Event
		 */
		Event& operator=(const Event& rhs);

		/**
		 *	move assigment operator
		 *	@param R value reference to the Event to be moved
		 *	@return reference to the mvoed Event
		 */
		Event& operator=(Event&& rhs);

		/**
		 *	registers the given subscriber to recieve notifications from this Event
		 *	@param reference to the Subscriber
		 */
		static void Subscribe(IEventSubscriber& subscriber);

		/**
		 *	deregisters the given subscriber from this Event
		 *	the subscriber will no longer be notified by this Event
		 *	@param reference to the Subscriber
		 */
		static void Unsubscribe(IEventSubscriber& subscriber);

		/**
		 *	deregisters all subscribers of this Event
		 */
		static void UnsubscriberAll();

		/**
		 *	The payload carried by this Event for its subscribers
		 *	@return constant reference to the payload object
		 */
		const T& Message() const;

	private:
		T mMessage;
		static Vector<IEventSubscriber*> mSubscriberList;
	};
}

#include "Event.inl"
