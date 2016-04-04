#pragma once

namespace Library
{
	class EventPublisher;

	class IEventSubscriber
	{
	public:
		IEventSubscriber() = default;
		virtual ~IEventSubscriber() = default;

		virtual void Notify(const EventPublisher& eventMessage) = 0;
	};

}
