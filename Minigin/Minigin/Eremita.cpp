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
#include "TextComponent.h"
#include "Scene.h"
#include "SceneObject.h"
#include "RenderComponent.h"
#include "FPSComponent.h"
#include "GameTime.h"

using namespace std;
using namespace std::chrono;

void dae::Eremita::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	m_Window = SDL_CreateWindow(
		"Programming 4 assignment",
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
}

/**
 * Code constructing the scene world starts here
 */
void dae::Eremita::LoadGame() const
{
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");

	const auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	auto* const pRenderComponent = new RenderComponent();
	auto* const pTextComponent = new TextComponent("60", font);
	auto* const pFPSComponent = new FPSComponent();
	
	const auto fpsObject = std::make_shared<SceneObject>(std::vector<BaseComponent*>{pFPSComponent, pTextComponent});
	fpsObject->AddComponent(pRenderComponent, true);

	scene.Add(fpsObject);
	
	//go->SetTexture("background.jpg");
	//scene.Add(go);

	//go = std::make_shared<GameObject>();
	//go->SetTexture("logo.png");
	//go->SetPosition(216, 180);
	//scene.Add(go);

	//auto to = std::make_shared<TextComponent>("Programming 4 Assignment", font);
	//to->SetPosition(80, 20);
	//scene.Add(to);

	scene.Init();
}

void dae::Eremita::Cleanup()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	SDL_Quit();
}

void dae::Eremita::Run()
{
	Initialize();

	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Data/");

	LoadGame();

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
