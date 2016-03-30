#include "pch.h"
#include "Game.h"

namespace Library
{

	Game::Game() :
		mGameClock(), mGameTime(), mWorld(mGameTime), 
		mSharedData(), mParseMaster(mSharedData),
		mWorldParser(), mSectorParser(), mEntityParser(), mActionParser(),
		mTableParser(), mPrimitivesParser(), mNameValueParser()
	{
		mSharedData.SetRootScope(mWorld);

		mParseMaster.AddHelper(mWorldParser);
		mParseMaster.AddHelper(mSectorParser);
		mParseMaster.AddHelper(mEntityParser);
		mParseMaster.AddHelper(mActionParser);
		mParseMaster.AddHelper(mTableParser);
		mParseMaster.AddHelper(mPrimitivesParser);
		mParseMaster.AddHelper(mNameValueParser);
	}


	Game::~Game()
	{
		SharedDataTable::ClearStateGraph();
		XmlParseHelperPrimitives::ClearStaticMembers();
	}

	World& Game::GetWorld()
	{
		return mWorld;
	}

	void Game::AddXmlParseHelper(IXmlParseHelper& helper)
	{
		mParseMaster.AddHelper(helper);
	}

	bool Game::ParseFromFile(const std::string& fileName)
	{
		return mParseMaster.ParseFromFile(fileName);
	}

	void Game::Start()
	{

		mGameClock.Reset();
	}

	void Game::Update()
	{
		mGameClock.UpdateGameTime(mGameTime);
		mWorld.Update();
	}
}
