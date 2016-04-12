#include "pch.h"
#include "TestSharedData.h"

using namespace Library;

RTTI_DEFINITIONS(TestSharedData);

TestSharedData::TestSharedData() :
	mData(std::string())
{}

XmlParseMaster::SharedData* TestSharedData::Clone() const
{
	return new TestSharedData();
}