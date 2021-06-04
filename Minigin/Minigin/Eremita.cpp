#include "MiniginPCH.h"
#include "Eremita.h"
#include <chrono>
#include <fstream>
#include <thread>
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include <SDL.h>

#include "audio.h"
#include "TextComponent.h"
#include "Scene.h"
#include "SceneObject.h"
#include "RenderComponent.h"
#include "HealthComponent.h"
#include "HealthDisplayComponent.h"
#include "SubjectComponent.h"
#include "FPSComponent.h"
#include "GameTime.h"
#include "ScoreDisplayComponent.h"
#include "ScoreComponent.h"
#include "Logger.h"
#include "SoundServiceLocator.h"
#include "SimpleSoundService.h"
#include "SoundLogService.h"

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#include <vld.h>
#endif

using namespace std;
using namespace std::chrono;

void dae::Eremita::Initialize()
{
	srand((unsigned int)time(NULL));

	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Data/");


	_putenv("SDL_AUDIODRIVER=DirectSound");
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	if(SDL_Init(SDL_INIT_AUDIO) != 0)
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	initAudio();

	m_Window = SDL_CreateWindow(
		"Q*Bert",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);
	if (m_Window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(m_Window);
	SoundServiceLocator::RegisterSoundService(new SoundLogService());
	SoundServiceLocator::RegisterSoundService(new SimpleSoundService());
	SoundServiceLocator::RegisterSoundService(new SoundLogService());
	SoundServiceLocator::RegisterSoundService(new SimpleSoundService());
}

void dae::Eremita::Cleanup()
{
	SoundServiceLocator::RegisterSoundService(nullptr);
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	endAudio();
	SDL_Quit();
}

void dae::Eremita::Run()
{
	//Initialize();

	//// tell the resource manager where he can find the game data
	//ResourceManager::GetInstance().Init("../Data/");

	//LoadGame();

	{
		auto& renderer = Renderer::GetInstance();
		auto& sceneManager = SceneManager::GetInstance();
		auto& input = InputManager::GetInstance();
		auto& gameTime = GameTime::GetInstance();
		
		auto doContinue = true;
		
		auto previousTime = steady_clock::now();
		auto lag = duration<float>::zero();
				
		while (doContinue)
		{
			const auto currentTime = steady_clock::now();
			const auto elapsedTime = currentTime - previousTime;
			previousTime = currentTime;
			lag += elapsedTime;

			gameTime.SetDeltaTime(elapsedTime);
			doContinue = input.ProcessInput();
			
			while(lag >= milliseconds(GameTime::TimeStep))
			{
				sceneManager.FixedUpdate();
				lag -= milliseconds(GameTime::TimeStep);
			}

			sceneManager.Update();
			
			renderer.Render();

			const auto sleepTime = previousTime + milliseconds(GameTime::MsPerFrame) - steady_clock::now();
			this_thread::sleep_for(sleepTime);
		}
	}

	Cleanup();
}
