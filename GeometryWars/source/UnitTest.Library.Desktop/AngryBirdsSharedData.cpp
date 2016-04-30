#include "pch.h"
#include "AngryBirdsSharedData.h"

using namespace Library;

RTTI_DEFINITIONS(AngryBirdsSharedData, XmlParseMaster::SharedData);

AngryBirdsSharedData::AngryBirdsSharedData() :
	mAngryBirdsData(1)
{}

XmlParseMaster::SharedData* AngryBirdsSharedData::Clone() const
{
	return new AngryBirdsSharedData();
}
