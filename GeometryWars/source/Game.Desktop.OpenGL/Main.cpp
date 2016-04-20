#include "pch.h"
#include "Game.h"
#include "Renderer.h"
#include "OpenGLRenderDevice.h"
#include "Sprite.h" // TODO just for testing

using namespace Library;


#pragma region SoundFMOD

#define MUSIC_BEEP "Content/Music/retire.mp3"

FMOD_RESULT FmodResult;
FMOD::System *FmodSystem;
int32_t numberOfDrivers;
FMOD::Sound *audios[4];
FMOD::Channel *channels[4];

void FMODErrorCheck(FMOD_RESULT result);
void PlayMusic(int32_t  soundId, int32_t toLoopZeroToN, float_t volumeZeroToOne);
bool FileExists(const std::string& name);

#pragma endregion



int WINAPI WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR commandLine, int showCommand)
{
	UNREFERENCED_PARAMETER(instance);
	UNREFERENCED_PARAMETER(previousInstance);
	UNREFERENCED_PARAMETER(commandLine);
	UNREFERENCED_PARAMETER(showCommand);

	Game game;
	game.Start();

#pragma region Fmodstuff

	// Create FMOD interface object
	FmodResult = FMOD::System_Create(&FmodSystem);
	FMODErrorCheck(FmodResult);

	// Get number of available sound cards
	FmodResult = FmodSystem->getNumDrivers(&numberOfDrivers);
	FMODErrorCheck(FmodResult);

	// No sound cards (disable sound)
	if (numberOfDrivers == 0)
	{
		FmodResult = FmodSystem->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
		FMODErrorCheck(FmodResult);
	}

	//check if there is any problem for the init function
	FmodResult = FmodSystem->init(100, FMOD_INIT_NORMAL, 0);
	FMODErrorCheck(FmodResult);

	FmodResult = FmodSystem->createSound(MUSIC_BEEP, FMOD_DEFAULT, 0, &audios[0]);
	FMODErrorCheck(FmodResult);

	PlayMusic(0, 2, 1);

#pragma endregion

	Sprite sprite;
	sprite.SetColor(glm::vec4(1, 0, 0, 0));
	sprite.SetImagePath("Content/resource/mushroom.png");

	OpenGLRenderDevice renderDevice;
	renderDevice.InitOpenGl();

	Renderer render(&renderDevice);
	render.AddRenderable(&sprite);

	glm::vec4 pos(1, 0, 0, 0);
#pragma warning(push)
#pragma warning(disable : 4127)
	while (true) { // TODO remove always true
#pragma warning(pop) 
		render.Update();
		pos.x += 1;
		sprite.SetPosition(pos);
	}

	return 0;
}



void FMODErrorCheck(FMOD_RESULT result)
{

	if (result != FMOD_OK)
	{
		std::cout << "\7 FMOD error! (" << result << ") " << FMOD_ErrorString(result) << std::endl;
		exit(-1);
	}
}


void PlayMusic(int32_t  soundId, int32_t toLoopZeroToN, float_t volumeZeroToOne)
{
	// play the sound attached to corresponing audios and channels.
	FmodResult = FmodSystem->playSound(audios[(int32_t)soundId], nullptr, false, &channels[(int32_t)soundId]);
	FMODErrorCheck(FmodResult);
	channels[(int32_t)soundId]->setMode(FMOD_LOOP_NORMAL);

	//set the number of times audio should loop 
	channels[(int32_t)soundId]->setLoopCount(toLoopZeroToN - 1);
	// Set volume of audio
	channels[(int32_t)soundId]->setVolume(volumeZeroToOne);

	//Speed can be set for MOD/S3M/XM/IT/MID sequenced formats only.
	//MP3, ogg, and wav are NOT such sequenced formats.
	//So if we find such audios, we can use this function for Speed of music.
	/*
	audios[(int32_t)soundId]->setMusicSpeed(0.1f);
	*/
}


bool FileExists(const std::string& name)
{
	std::ifstream myfile(name.c_str());

	if (myfile.good()) {

		myfile.close();
		return true;
	}
	else {
		myfile.close();
		return false;
	}
}