#include "pch.h"
#include "FooSubscriber.h"

using namespace Library;

FooSubscriber::FooSubscriber() :
	wasNotified(false), data(0)
{}

void FooSubscriber::Notify(const EventPublisher& publisher)
{
	assert(publisher.Is(Event<Foo>::TypeIdClass()));

	const Foo& foo = publisher.As<Event<Foo>>()->Message();
	data = foo.GetData();
	wasNotified = true;
}
