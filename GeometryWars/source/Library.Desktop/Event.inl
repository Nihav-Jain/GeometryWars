#include "pch.h"
#include "Event.h"

namespace Library
{
	RTTI_DEFINITIONS_T(Event<T>);

	template <typename T>
	Vector<IEventSubscriber*> Event<T>::mSubscriberList(0);

	template <typename T>
	Event<T>::Event(const T& payload) :
		mMessage(payload), EventPublisher(mSubscriberList)
	{}

	template<typename T>
	Event<T>::Event(T&& payload) :
		mMessage(std::move(payload)), EventPublisher(mSubscriberList)
	{}

	template<typename T>
	Event<T>::Event(const Event& rhs) :
		mMessage(rhs.mMessage), EventPublisher(rhs)
	{}

	template<typename T>
	Event<T>::Event(Event&& rhs) :
		mMessage(std::move(rhs.mMessage)), EventPublisher(std::move(rhs))
	{}

	template<typename T>
	Event<T>& Event<T>::operator=(const Event& rhs)
	{
		if (this != &rhs)
		{
			mMessage = rhs.mMessage;
			EventPublisher::operator=(rhs);
		}
		return *this;
	}

	template<typename T>
	Event<T>& Event<T>::operator=(Event&& rhs)
	{
		if (this != &rhs)
		{
			mMessage = std::move(rhs.mMessage);
			EventPublisher::operator=(std::move(rhs));
		}
		return *this;
	}

	template<typename T>
	void Event<T>::Subscribe(IEventSubscriber& subscriber)
	{
		mSubscriberList.PushBack(&subscriber);
	}

	template<typename T>
	void Event<T>::Unsubscribe(IEventSubscriber& subscriber)
	{
		mSubscriberList.Remove(&subscriber);
	}

	template<typename T>
	void Event<T>::UnsubscriberAll()
	{
		mSubscriberList.Clear();
	}

	template<typename T>
	const T& Event<T>::Message() const
	{
		return mMessage;
	}
}
