#include "pch.h"
#include "BarSubscriber.h"
#include "Bar.h"

using namespace Library;

BarSubscriber::BarSubscriber() :
	wasNotified(false), data(0)
{}

void BarSubscriber::Notify(const EventPublisher& publisher)
{
	assert(publisher.Is(Event<Bar>::TypeIdClass()));

	const Bar& foo = publisher.As<Event<Bar>>()->Message();
	data = foo.GetData();
	wasNotified = true;
}
