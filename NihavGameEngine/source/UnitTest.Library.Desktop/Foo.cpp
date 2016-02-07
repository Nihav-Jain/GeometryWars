#include "pch.h"
#include "Foo.h"


Foo::Foo(int data) :
	mData(data), mPointerData(new int(data))
{
}

Foo::Foo(const Foo& rhs):
	mData(rhs.mData), mPointerData(new int(rhs.mData))
{
}

Foo::~Foo()
{
	delete mPointerData;
}

Foo& Foo::operator=(const Foo& rhs)
{
	if (this != &rhs)
	{
		mData = rhs.mData;

		delete mPointerData;
		mPointerData = new int(rhs.mData);
	}

	return *this;
}

bool Foo::operator==(const Foo & rhs) const
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

bool Foo::operator!=(const Foo & rhs) const
{
	return !(*this == rhs);
}

 int Foo::GetData() const
{
	return mData;
}

void Foo::SetData(int data)
{
	mData = data;
	*mPointerData = data;
}
int* Foo::GetPointerData() const
{
	return mPointerData;
}
