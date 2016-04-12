#include "pch.h"
#include "AngryBirdsSharedData.h"

using namespace Library;

RTTI_DEFINITIONS(AngryBirdsSharedData);

AngryBirdsSharedData::AngryBirdsSharedData() :
	mAngryBirdsData(1)
{}

XmlParseMaster::SharedData* AngryBirdsSharedData::Clone() const
{
	return new AngryBirdsSharedData();
}
