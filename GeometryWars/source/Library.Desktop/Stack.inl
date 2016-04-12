#include "pch.h"
#include "Stack.h"

namespace Library
{
	template <typename T>
	Stack<T>::Stack(const Stack& rhs) :
		mDataList(rhs.mDataList)
	{}

	template <typename T>
	Stack<T>::Stack(Stack&& rhs) :
		mDataList(std::move(rhs.mDataList))
	{}

	template <typename T>
	Stack<T>& Stack<T>::operator=(const Stack& rhs)
	{
		if (this != &rhs)
		{
			Clear();
			mDataList = rhs.mDataList;
		}

		return *this;
	}

	template <typename T>
	Stack<T>& Stack<T>::operator=(Stack&& rhs)
	{
		if (this != &rhs)
		{
			Clear();
			mDataList = std::move(rhs.mDataList);
		}
		return *this;
	}

	template <typename T>
	Stack<T>::~Stack()
	{
		Clear();
	}

	template <typename T>
	void Stack<T>::Push(const T& data)
	{
		mDataList.PushFront(data);
	}

	template <typename T>
	void Stack<T>::Pop()
	{
		mDataList.PopFront();
	}

	template <typename T>
	T& Stack<T>::Top()
	{
		return mDataList.Front();
	}

	template <typename T>
	const T& Stack<T>::Top() const
	{
		return mDataList.Front();
	}

	template <typename T>
	std::uint32_t Stack<T>::Size() const
	{
		return mDataList.Size();
	}

	template <typename T>
	bool Stack<T>::IsEmpty() const
	{
		return mDataList.IsEmpty();
	}

	template <typename T>
	void Stack<T>::Clear()
	{
		mDataList.Clear();
	}
}