#include "pch.h"
#include "Game.h"

namespace Library
{

	Game::Game() :
		mSharedData(), mParseMaster(mSharedData),
		mGameClock(), mGameTime(), mWorld(mGameTime, mParseMaster), mRenderer(nullptr), mAudioManager()
	{
		mSharedData.SetRootScope(mWorld);
		AddParseHelpers();
	}

	Game::~Game()
	{
		SharedDataTable::ClearStateGraph();
		ActionExpression::ClearStaticMemebers();
		Attributed::ClearStaticMembers();
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

	void Game::Start()
	{
		mGameClock.Reset();
		mGameClock.UpdateGameTime(mGameTime); 
		mWorld.SetAudioManager(mAudioManager);
		mWorld.BeginPlay();
	}

	void Game::Start(const std::string & config)
	{
		mParseMaster.ParseFromFile(config);

		mGameClock.Reset();
		mGameClock.UpdateGameTime(mGameTime);
		mWorld.SetAudioManager(mAudioManager);
		mWorld.BeginPlay();
	}

	void Game::Update()
	{
		mGameClock.UpdateGameTime(mGameTime);
		mWorld.Update();
		if (mRenderer != nullptr)
			mRenderer->Update();
		mAudioManager.Update();
	}

	void Game::SetRenderer(Renderer* renderer)
	{
		mRenderer = renderer;
	}

	void Game::Destroy()
	{
		mWorld.OnDestroy();
	}

	void Game::AddParseHelpers()
	{
		mParseMaster.AddHelper(mDefineParser);
		mParseMaster.AddHelper(mDefineSectorParser);

		mParseMaster.AddHelper(mWorldParser);
		mParseMaster.AddHelper(mSectorParser);
		mParseMaster.AddHelper(mEntityParser);
		mParseMaster.AddHelper(mGameObjectParser);
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
		mParseMaster.AddHelper(mActionOnDestroy);
		mParseMaster.AddHelper(mSpriteParser);
		mParseMaster.AddHelper(mPolygonParser);
		mParseMaster.AddHelper(mImageParser);
		mParseMaster.AddHelper(mAnimatorParser);
		mParseMaster.AddHelper(mAnimationStateParser);
		mParseMaster.AddHelper(mAnimationFrameParser);
		mParseMaster.AddHelper(mCircleColliderComponent);
	}
}
