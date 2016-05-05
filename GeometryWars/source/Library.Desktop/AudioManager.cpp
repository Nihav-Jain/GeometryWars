#include "pch.h"

namespace Library
{

	AudioManager::AudioManager() :
		mAudioMap(5),
		mFmodSystem(nullptr),
		mNumberOfDrivers(0),
		mChannelCount(0)
	{
		InitMusic();
	}


	AudioManager::~AudioManager()
	{
		// TODO: Kishor Fix me!

		//release sound and system
		//for (auto pair : mAudioMap)
		//{
		//	
		//	//AudioData *audioDataAddress = &pair.second;
		//	//audioDataAddress->mAudio->release();
		//	//delete audioDataAddress;
		//}
		mFmodSystem->release();
	}

	void AudioManager::InitMusic()
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
	}

	void AudioManager::FMODErrorCheck(const FMOD_RESULT& result)
	{
		if (result != FMOD_OK)
		{
			throw std::exception("Sound Error, verify the names!");
		}
	}

	void AudioManager::CreateMusic(const std::string & audioNameWithExtension)
	{
		//create music	
		AudioData *newAudioData = new AudioData;
		newAudioData->mChannel = &mChannels[mChannelCount++];
		std::string completePath = PATH + audioNameWithExtension;
		mFmodResult = mFmodSystem->createSound(completePath.c_str(), FMOD_DEFAULT, 0, &newAudioData->mAudio);
		FMODErrorCheck(mFmodResult);

		mAudioMap.Insert(std::make_pair(audioNameWithExtension, *newAudioData));
	}

	void AudioManager::LoadMusic(const std::string & audioNameWithExtension)
	{
		Hashmap <std::string, AudioData>::Iterator foundElement = mAudioMap.Find(audioNameWithExtension);
		//music not created
		if ( foundElement == mAudioMap.end())
		{
			CreateMusic(audioNameWithExtension);
		}
	}

	void AudioManager::PlayMusic(const std::string & audioNameWithExtension, std::int32_t loopsOneToN, std::float_t volumeZeroToOne)
	{
		Hashmap <std::string, AudioData>::Iterator foundElement = mAudioMap.Find(audioNameWithExtension);

		if (foundElement == mAudioMap.end())
		{
			throw std::exception("Audio not declared in BeginPlay!");
		}

		// Fixing FMOD bug, all channels will stop if it randomly pick a low prioirty channel
		// Use resumeChannel to set higher priority for the channel that is playing
		ResumeChannel();

		mFmodResult = mFmodSystem->playSound(((*foundElement).second.mAudio), nullptr, false, ((*foundElement).second.mChannel));

		//cache the channel
		FMOD::Channel *channel = *((*foundElement).second.mChannel);
	
		// An old channel handle will become invalid if the sound system pick a channel, it makes the old channel handle invalid.
		// So we need to store the new channel handle even the channel is included in our list
		PutChannel(*channel);

		FMODErrorCheck(mFmodResult);
		channel->setMode(FMOD_LOOP_NORMAL);

		//set the number of times audio should loop 
		channel->setLoopCount(loopsOneToN - 1);

		// Set volume of audio
		channel->setVolume(volumeZeroToOne);
	}


	void AudioManager::TogglePauseMusic(const std::string & audioNameWithExtension)
	{
		Hashmap <std::string, AudioData>::Iterator foundElement = mAudioMap.Find(audioNameWithExtension);

		if (foundElement == mAudioMap.end())
		{
			throw std::exception("Audio not declared in BeginPlay!");
		}

		bool isPaused;
		(*(*foundElement).second.mChannel)->getPaused(&isPaused);
		(*(*foundElement).second.mChannel)->setPaused(!isPaused);
	}

	void AudioManager::StopMusic(const std::string & audioNameWithExtension)
	{
		Hashmap <std::string, AudioData>::Iterator foundElement = mAudioMap.Find(audioNameWithExtension);
		
		if (foundElement == mAudioMap.end())
		{
			throw std::exception("Audio not declared in BeginPlay!");
		}

		(*(*foundElement).second.mChannel)->stop();
	}

	void AudioManager::ResumeChannel()
	{
		// Find all channel and modify their priority, the one that is curreny playing has high priority
		for (int i = 0; i < MAX_CHANNELS; i++) 
		{
			if (mChannels[i] != NULL) 
			{
				bool isPlaying = false;
				mChannels[i]->isPlaying(&isPlaying);
				if (isPlaying) 
				{
					mChannels[i]->setPriority(MIN_PRIORITY);
				}
				else 
				{
					mChannels[i]->setPriority(MAX_PRIORITY);
					mChannels[i]->stop();
				}
			}
		}
	}

	void AudioManager::PutChannel(FMOD::Channel & channel)
	{
		int index = 0;
		channel.setPriority(0);
		// Find empty or low priority, modify the priority, and put the new channel handle into our array
		for (int i = 0; i < MAX_CHANNELS; i++) 
		{
			if (mChannels[i] == NULL) 
			{
				index = i;
				break;
			}
			else 
			{
				bool isPlaying = false;
				mChannels[i]->isPlaying(&isPlaying);
				if (isPlaying) 
				{
					mChannels[i]->setPriority(MIN_PRIORITY);
				}
				else 
				{
					index = i;
					break;
				}
			}
		}
		mChannels[index] = &channel;
	}

	void AudioManager::Update()
	{
		mFmodSystem->update();
	}

}
