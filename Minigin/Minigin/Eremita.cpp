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
#include "HealthComponent.h"
#include "HealthDisplayComponent.h"
#include "SubjectComponent.h"
#include "FPSComponent.h"
#include "GameTime.h"
#include "ScoreDisplayComponent.h"
#include "ScoreComponent.h"

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
	using pComponentVec = std::vector<BaseComponent*>;
	
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");

	const auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 27);

	// FPS display
	auto* const pRenderComponent = new RenderComponent();
	auto* const pTextComponent = new TextComponent("60", font);
	auto* const pFPSComponent = new FPSComponent();
	
	const auto fpsObject = std::make_shared<SceneObject>(pComponentVec{pFPSComponent, pTextComponent});
	fpsObject->AddComponent(pRenderComponent, true);

	scene.Add(fpsObject);

	// Lives display
	const auto livesDisplay = std::make_shared<SceneObject>(pComponentVec{}, glm::vec3{0,30,0});
	livesDisplay->AddComponent(new TextComponent("3 lives", font));
	livesDisplay->AddComponent(new RenderComponent(), true);
	livesDisplay->AddComponent(new HealthDisplayComponent());
	scene.Add(livesDisplay);

	// Score display
	const auto scoreDisplay = std::make_shared<SceneObject>(pComponentVec{}, glm::vec3{ 0, 60, 0 });
	scoreDisplay->AddComponent(new TextComponent("Score: 0", font));
	scoreDisplay->AddComponent(new RenderComponent(), true);
	scoreDisplay->AddComponent(new ScoreDisplayComponent());
	
	scene.Add(scoreDisplay);

	// Qbert
	const auto qBert = std::make_shared<SceneObject>();
	qBert->AddComponent(new HealthComponent());
	qBert->AddComponent(new SubjectComponent());
	qBert->AddComponent(new ScoreComponent());
	qBert->SetTag("player");
	scene.Add(qBert);

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
