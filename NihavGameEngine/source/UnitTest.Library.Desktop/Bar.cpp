#include "pch.h"
#include "Bar.h"


Bar::Bar(int data) :
	mData(data), mPointerData(new int(data))
{
}

Bar::Bar(const Bar& rhs):
	mData(rhs.mData), mPointerData(new int(rhs.mData))
{
}

Bar::Bar(Bar&& rhs) :
	mData(rhs.mData), mPointerData(rhs.mPointerData)
{
	rhs.mData = 0;
	rhs.mPointerData = nullptr;
}

Bar::~Bar()
{
	delete mPointerData;
}

Bar& Bar::operator=(const Bar& rhs)
{
	if (this != &rhs)
	{
		mData = rhs.mData;

		delete mPointerData;
		mPointerData = new int(rhs.mData);
	}

	return *this;
}

Bar& Bar::operator=(Bar&& rhs)
{
	if (this != &rhs)
	{
		mData = rhs.mData;
		mPointerData = rhs.mPointerData;

		rhs.mData = 0;
		rhs.mPointerData = nullptr;
	}

	return *this;
}

bool Bar::operator==(const Bar & rhs) const
{
	bool returnVal = true;
	if (mData != rhs.mData)
	{
		returnVal = false;
	}
	else if (*mPointerData != *rhs.mPointerData)
	{
		returnVal = false;
	}
	return returnVal;
}

bool Bar::operator!=(const Bar & rhs) const
{
	return !(*this == rhs);
}

 int Bar::GetData() const
{
	return mData;
}

void Bar::SetData(int data)
{
	mData = data;
	*mPointerData = data;
}
int* Bar::GetPointerData() const
{
	return mPointerData;
}
