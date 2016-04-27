#include "pch.h"
#include "FooRTTI.h"
using namespace Library;

RTTI_DEFINITIONS(FooRTTI, RTTI);

FooRTTI::FooRTTI(int data) :
	mData(data), mPointerData(new int(data))
{
}

FooRTTI::FooRTTI(const FooRTTI& rhs) :
	mData(rhs.mData), mPointerData(new int(rhs.mData))
{
}

FooRTTI::~FooRTTI()
{
	if(mPointerData != nullptr)
	{
		delete mPointerData;
		mPointerData = nullptr;
	}
}

FooRTTI& FooRTTI::operator=(const FooRTTI& rhs)
{
	if (this != &rhs)
	{
		mData = rhs.mData;

		delete mPointerData;
		mPointerData = new int(rhs.mData);
	}

	return *this;
}

bool FooRTTI::operator==(const FooRTTI & rhs) const
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

bool FooRTTI::operator!=(const FooRTTI & rhs) const
{
	return !(*this == rhs);
}

int FooRTTI::GetData() const
{
	return mData;
}

void FooRTTI::SetData(int data)
{
	mData = data;
	*mPointerData = data;
}
int* FooRTTI::GetPointerData() const
{
	return mPointerData;
}
