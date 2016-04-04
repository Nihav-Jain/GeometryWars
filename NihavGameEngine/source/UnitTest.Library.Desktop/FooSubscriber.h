#pragma once
#include "IEventSubscriber.h"
#include "EventPublisher.h"
#include "Event.h"

class FooSubscriber : public Library::IEventSubscriber
{
public:
	FooSubscriber();
	virtual ~FooSubscriber() = default;

	virtual void Notify(const Library::EventPublisher& publisher) override;

	bool wasNotified;
	std::int32_t data;
};

