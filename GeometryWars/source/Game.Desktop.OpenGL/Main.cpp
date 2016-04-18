#include "pch.h"
#include "Game.h"

using namespace Library;


#pragma region sound_FMOD

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

	const glm::vec4 CornflowerBlue = glm::vec4(0.392f, 0.584f, 0.929f, 1.0f);

	if (!glfwInit())
	{
		return -1;
	}

	GLFWwindow* window = glfwCreateWindow(800, 600, "Geometry Wars OpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (gl3wInit() != 0)
	{
		return -1;
	}

	Game game;
	game.Start();

	glViewport(0, 0, 800, 600);


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


	while (!glfwWindowShouldClose(window))
	{
		glClearBufferfv(GL_COLOR, 0, &CornflowerBlue[0]);

		glfwSwapBuffers(window);
		glfwPollEvents();

		game.Update();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

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