#include "pch.h"
#include "Game.h"

namespace Library
{

	Game::Game() :
		mGameClock(), mGameTime(), mWorld(mGameTime), 
		mSharedData(), mParseMaster(mSharedData),
		mWorldParser(), mSectorParser(), mEntityParser(), mActionParser(),
		mTableParser(), mPrimitivesParser(), mNameValueParser(),
		mEventQueue()
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
		Datum::ClearStaticMembers();
		SharedDataTable::ClearStateGraph();
		XmlParseHelperPrimitives::ClearStaticMembers();
	}

	World& Game::GetWorld()
	{
		return mWorld;
	}

	XmlParseMaster& Game::ParseMaster()
	{
		return mParseMaster;
	}

	EventQueue& Game::GameEventQueue()
	{
		return mEventQueue;
	}

	const GameTime& Game::GetGameTime() const
	{
		return mGameTime;
	}

	void Game::Start()
	{
		mGameClock.Reset();
	}

	void Game::Update()
	{
		mGameClock.UpdateGameTime(mGameTime);
		mEventQueue.Update(mGameTime);
		mWorld.Update();
	}

	void Game::Destroy()
	{}
}
