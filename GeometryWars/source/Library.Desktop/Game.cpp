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
		mActionWhile(), mActionWhileLoop(), mActionBeginPlay()
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
		InitMusic();
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

#pragma region SoundFmodMethodDefinitions

	void Game::InitMusic()
	{
		// Create FMOD interface object
		mFmodResult = FMOD::System_Create(&mFmodSystem);
		FMODErrorCheck(mFmodResult);

		// Get number of available sound cards
		mFmodResult = mFmodSystem->getNumDrivers(&mNumberOfDrivers);
		FMODErrorCheck(mFmodResult);

		// No sound cards (disable sound)
		if (mNumberOfDrivers == 0)
		{
			mFmodResult = mFmodSystem->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
			FMODErrorCheck(mFmodResult);
		}

		//check if there is any problem for the init function
		mFmodResult = mFmodSystem->init(100, FMOD_INIT_NORMAL, 0);
		FMODErrorCheck(mFmodResult);

		///for creating sound
		//mFmodResult = mFmodSystem->createSound(MUSIC_BEEP, FMOD_DEFAULT, 0, &mAudios[0]);
		//FMODErrorCheck(mFmodResult);
		///for playing music call this from game.cpp
		//PlayMusic(0, 2, 1);
	}

	void Game::FMODErrorCheck(const FMOD_RESULT& result)
	{
		if (result != FMOD_OK)
		{
			//std::cout << "\7 FMOD error! (" << result << ") " << FMOD_ErrorString(result) << std::endl;
			exit(-1);
		}
	}

	void Game::PlayMusic(int32_t soundId, int32_t toLoopZeroToN, float_t volumeZeroToOne)
	{
		// play the sound attached to corresponing audios and channels.
		mFmodResult = mFmodSystem->playSound(mAudios[(int32_t)soundId], nullptr, false, &mChannels[(int32_t)soundId]);
		FMODErrorCheck(mFmodResult);
		mChannels[(int32_t)soundId]->setMode(FMOD_LOOP_NORMAL);

		//set the number of times audio should loop 
		mChannels[(int32_t)soundId]->setLoopCount(toLoopZeroToN - 1);
		// Set volume of audio
		mChannels[(int32_t)soundId]->setVolume(volumeZeroToOne);

		//Speed can be set for MOD/S3M/XM/IT/MID sequenced formats only.
		//MP3, ogg, and wav are NOT such sequenced formats.
		//So if we find such audios, we can use this function for Speed of music.
		/**
		 *	audios[(int32_t)soundId]->setMusicSpeed(0.1f);
		 */
	}

	const FMOD::System & Game::GetFmodSystem()
	{
		return (*mFmodSystem);
	}

#pragma endregion
}
