#pragma once

#include "Hashmap.h"
#include <fstream>
#pragma warning(disable: 4505)
#include "fmod.hpp"
#include "fmod_errors.h"

namespace Library
{
	const std::int32_t MAX_CHANNELS = 256;

	/**
	 *	Class AudioManager
	 */
	class AudioManager
	{

	public:

		AudioManager();
		~AudioManager();

		/**
		 *	AudioData - each audio will run in its own channel and will be loading on a seperate address
		 */
		struct AudioData
		{
			FMOD::Sound *mAudio;
			FMOD::Channel **mChannel;
		};

		/**
		*	InitMusic - Do initialization for music i.e. for FMOD.
		*/
		void InitMusic();

		/**
		*	GetFmodSystem - get the FMOD System
		*	@return const reference of result 
		*/
		void FMODErrorCheck(const FMOD_RESULT & result);

		/**
		*	LoadMusic - Load music of given name with extension
		*	@param audioNameWithExtension	just name no path
		*/
		void LoadMusic (const std::string& audioNameWithExtension);

		/**
		*	PlayMusic - Play music of given name with extension
		*	@param audioNameWithExtension	just name no path
		*	@param loopsOneToN	zero for continous looping and N for N loops
		*	@param volumeZeroToOne	volume in float from zero to 1
		*/
		void PlayMusic (const std::string& audioNameWithExtension, std::int32_t loopsOneToN = 1, std::float_t volumeZeroToOne = 1.0f);

		/**
		*	TogglePauseMusic - Pause if playing and vice verca
		*	@param audioNameWithExtension	just name no path
		*/
		void TogglePauseMusic(const std::string& audioNameWithExtension);

		/**
		*	StopMusic - Stop Music
		*	@param audioNameWithExtension	just name no path
		*/
		void StopMusic(const std::string& audioNameWithExtension);

		/**
		*	ResumeChannel - 
		*/
		void ResumeChannel();

		/**
		*	PutChannel - 
		*	@param channel	the channel whose priority is to be changed
		*/
		void PutChannel(FMOD::Channel & channel);

		void Update();
	
	private:

		void CreateMusic(const std::string& audioNameWithExtension);
	
		const std::string PATH = "Content/Music/";
		const std::int32_t VIRTUAL_CHANNELS = 256;
		const std::int32_t MIN_PRIORITY = 0;
		const std::int32_t MAX_PRIORITY = 256;
		FMOD::Channel *mChannels[MAX_CHANNELS];

		FMOD_RESULT mFmodResult;
		int32_t mNumberOfDrivers;
		FMOD::System *mFmodSystem;
		int mChannelCount;

		Hashmap <std::string, AudioData> mAudioMap;
	};

}


