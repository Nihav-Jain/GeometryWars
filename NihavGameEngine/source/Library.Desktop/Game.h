#pragma once

#include "GameClock.h"
#include "GameTime.h"

#include "Scope.h"
#include "World.h"
#include "Sector.h"
#include "Entity.h"
#include "Action.h"
#include "ActionList.h"

#include "SharedDataTable.h"
#include "XmlParseMaster.h"
#include "XmlParseHelperWorld.h"
#include "XmlParseHelperSector.h"
#include "XmlParseHelperEntity.h"
#include "XmlParseHelperAction.h"
#include "XmlParseHelperTable.h"
#include "XmlParseHelperPrimitives.h"
#include "XmlParseHelperNameValue.h"

namespace Library
{
	class Game final
	{
	public:
		Game();
		Game(const Game& rhs) = delete;
		~Game();

		Game& operator=(const Game& rhs) = delete;

		World& GetWorld();
		void AddXmlParseHelper(IXmlParseHelper& helper);
		bool ParseFromFile(const std::string& fileName);

		void Start();
		void Update();

	private:

		GameClock mGameClock;
		GameTime mGameTime;
		World mWorld;
		
		SharedDataTable mSharedData;
		XmlParseMaster mParseMaster;

		XmlParseHelperWorld mWorldParser;
		XmlParseHelperSector mSectorParser;
		XmlParseHelperEntity mEntityParser;
		XmlParseHelperAction mActionParser;
		XmlParseHelperTable mTableParser;
		XmlParseHelperPrimitives mPrimitivesParser;
		XmlParseHelperNameValue mNameValueParser;
	};
}

