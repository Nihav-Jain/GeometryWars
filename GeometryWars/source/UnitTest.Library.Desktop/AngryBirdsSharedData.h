#pragma once

#include "XmlParseMaster.h"
#include "Scope.h"

class AngryBirdsSharedData : public Library::XmlParseMaster::SharedData
{
	RTTI_DECLARATIONS(AngryBirdsSharedData, Library::XmlParseMaster::SharedData);
public:
	AngryBirdsSharedData();
	AngryBirdsSharedData(const AngryBirdsSharedData& rhs) = delete;

	virtual ~AngryBirdsSharedData() = default;

	AngryBirdsSharedData& operator=(const AngryBirdsSharedData& rhs) = delete;

	virtual Library::XmlParseMaster::SharedData* Clone() const override;
	Library::Scope mAngryBirdsData;
};

