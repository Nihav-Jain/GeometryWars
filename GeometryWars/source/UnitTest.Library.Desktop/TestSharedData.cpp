#include "pch.h"
#include "TestSharedData.h"

using namespace Library;

RTTI_DEFINITIONS(TestSharedData, XmlParseMaster::SharedData);

TestSharedData::TestSharedData() :
	mData(std::string())
{}

XmlParseMaster::SharedData* TestSharedData::Clone() const
{
	return new TestSharedData();
}