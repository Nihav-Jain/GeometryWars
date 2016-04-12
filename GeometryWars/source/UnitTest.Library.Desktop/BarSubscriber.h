#pragma once
#include "IEventSubscriber.h"
#include "EventPublisher.h"
#include "Event.h"

class BarSubscriber : public Library::IEventSubscriber
{
public:
	BarSubscriber();
	virtual ~BarSubscriber() = default;

	virtual void Notify(const Library::EventPublisher& publisher) override;

	bool wasNotified;
	std::int32_t data;
};

