#pragma once

#include "XmlParseMaster.h"
#include "Scope.h"

class TestSharedData : public Library::XmlParseMaster::SharedData
{
	RTTI_DECLARATIONS(TestSharedData, Library::XmlParseMaster::SharedData);
public:
	TestSharedData();
	TestSharedData(const TestSharedData& rhs) = delete;

	virtual ~TestSharedData() = default;

	TestSharedData& operator=(const TestSharedData& rhs) = delete;

	virtual Library::XmlParseMaster::SharedData* Clone() const override;
	std::string mData;
};

