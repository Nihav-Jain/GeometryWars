#include "pch.h"
#include "Game.h"

namespace Library
{

	Game::Game() :
		mGameClock(), mGameTime(), mWorld(mGameTime),
		mSharedData(), mParseMaster(mSharedData),
		mWorldParser(), mSectorParser(), mEntityParser(), mActionParser(),
		mTableParser(), mPrimitivesParser(), mNameValueParser(),
		mSwitchParser(), mCaseParser(), mExpressionParser(), mReactionParser(), mActionEvent(),
		mActionIf(), mActionIfCondition(), mActionIfThen(), mActionIfElse(),
		mActionWhile(), mActionWhileLoop(), mActionBeginPlay(), mAudioManager()
	{
		Init();
	}

	Game::~Game()
	{
		SharedDataTable::ClearStateGraph();
		ActionExpression::ClearStaticMemebers();
	}

	void Game::Init()
	{
		mSharedData.SetRootScope(mWorld);
		AddHelpers();
	}

	World& Game::GetWorld()
	{
		return mWorld;
	}

	XmlParseMaster& Game::ParseMaster()
	{
		return mParseMaster;
	}

	const GameTime& Game::GetGameTime() const
	{
		return mGameTime;
	}

	void Game::AddHelpers()
	{
		mParseMaster.AddHelper(mWorldParser);
		mParseMaster.AddHelper(mSectorParser);
		mParseMaster.AddHelper(mEntityParser);
		mParseMaster.AddHelper(mActionParser);
		mParseMaster.AddHelper(mTableParser);
		mParseMaster.AddHelper(mPrimitivesParser);
		mParseMaster.AddHelper(mNameValueParser);
		mParseMaster.AddHelper(mSwitchParser);
		mParseMaster.AddHelper(mCaseParser);
		mParseMaster.AddHelper(mExpressionParser);
		mParseMaster.AddHelper(mReactionParser);
		mParseMaster.AddHelper(mActionEvent);

		mParseMaster.AddHelper(mActionIf);
		mParseMaster.AddHelper(mActionIfCondition);
		mParseMaster.AddHelper(mActionIfThen);
		mParseMaster.AddHelper(mActionIfElse);

		mParseMaster.AddHelper(mActionWhile);
		mParseMaster.AddHelper(mActionWhileLoop);

		mParseMaster.AddHelper(mActionBeginPlay);
	}

	void Game::Start()
	{
		mGameClock.Reset();
		mGameClock.UpdateGameTime(mGameTime); 
		mWorld.BeginPlay();
	}

	void Game::Update()
	{
		mGameClock.UpdateGameTime(mGameTime);
		mWorld.Update();
	}

	void Game::Destroy()
	{}

}
