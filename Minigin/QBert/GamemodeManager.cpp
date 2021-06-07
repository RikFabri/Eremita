#include "GamemodeManager.h"
#include "Logger.h"
#include "imgui.h"

#include <vector>
#include <SceneManager.h>
#include <BaseComponent.h>
#include <ResourceManager.h>
#include <RenderComponent.h>
#include <TextComponent.h>
#include <FPSComponent.h>
#include <HealthDisplayComponent.h>
#include <ScoreDisplayComponent.h>
#include <HealthComponent.h>
#include <ScoreComponent.h>
#include <SubjectComponent.h>
#include <SceneObject.h>
#include <Scene.h>
#include "TileComponent.h"
#include "TileMapComponent.h"
#include "QBertBehaviourComponent.h"
#include "InputComponent.h"
#include "CoilyBehaviourComponent.h"
#include "DestroyOnPlayerDamageComponent.h"
#include "SlickAndSamBehaviourComponent.h"
#include "KillPlayerOnTouchComponent.h"
#include "WrongwayMovementComponent.h"
#include "UggMovementComponent.h"
#include "DefaultMovement.h"
#include "InputManager.h"
#include "EnemySpawner.h"
#include "PosessedMovementComponent.h"
#include "DestroyOnLevelChange.h"

using namespace dae;

GamemodeManager::GamemodeManager()
	: m_CurrentGamemode(Gamemode::eNone)
	, m_IsOpen(true)
	, m_GameOver(false)
{
}

void GamemodeManager::Render()
{
	if (m_GameOver)
		LoadGameOver();

	//if (!m_IsOpen)
	//	return;

	ImGui::Begin("Gamemode", &m_IsOpen);

	if (ImGui::Button("Single player", { 100,40 }))
	{
		StopPlaying();
		LoadAI();
	}

	if (ImGui::Button("Co-op", { 100,40 }))
	{
		StopPlaying();
		LoadCoop();
	}
	
	if (ImGui::Button("Versus", { 100,40 }))
	{
		StopPlaying();
		LoadVersus();
	}
	
	if (ImGui::Button("Reset scenes", { 100,40 }))
		StopPlaying();

	ImGui::End();
}

void GamemodeManager::LoadAI()
{
	if (m_CurrentGamemode == Gamemode::eAI)
		return;

	m_CurrentGamemode = Gamemode::eAI;

#pragma region Load AI level
	// -------------- LOAD LEVEL ----------------
	using pComponentVec = std::vector<BaseComponent*>;
	const auto sceneName = "AI";

	// If scene already exists, set it and return
	if (SceneManager::GetInstance().SetActiveScene(sceneName))
		return;

	auto& scene = SceneManager::GetInstance().CreateScene(sceneName);

	const auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 27);

	// Map
	const auto map = std::make_shared<SceneObject>(pComponentVec{}, glm::vec3{ 288,64,0 });
	map->AddComponent(new TileMapComponent(), true);
	map->AddComponent(new SubjectComponent());
	map->SetTag("tileMap");
	map->RegisterAsObserver(this);
	scene.Add(map);

	// FPS display
	auto* const pRenderComponent = new RenderComponent();
	auto* const pTextComponent = new TextComponent("60", font, SDL_Color{ 255,255,255 });
	auto* const pFPSComponent = new FPSComponent();

	const auto fpsObject = std::make_shared<SceneObject>(pComponentVec{ pFPSComponent, pTextComponent });
	fpsObject->AddComponent(pRenderComponent, true);

	scene.Add(fpsObject);

	// Lives display
	const auto livesDisplay = std::make_shared<SceneObject>(pComponentVec{}, glm::vec3{ 0,30,0 });
	livesDisplay->AddComponent(new TextComponent("3 lives", font));
	livesDisplay->AddComponent(new RenderComponent(), true);
	livesDisplay->AddComponent(new HealthDisplayComponent(0));
	scene.Add(livesDisplay);

	// Score display
	const auto scoreDisplay = std::make_shared<SceneObject>(pComponentVec{}, glm::vec3{ 0, 60, 0 });
	scoreDisplay->AddComponent(new TextComponent("Score: 0", font));
	scoreDisplay->AddComponent(new RenderComponent(), true);
	scoreDisplay->AddComponent(new ScoreDisplayComponent(0));

	scene.Add(scoreDisplay);

	// Qbert
	const auto qbertMovement = new PosessedMovementComponent();
	qbertMovement->SetBlockIndex({ 0,0 });
	const auto qBert = std::make_shared<SceneObject>(pComponentVec{}, glm::vec3{ 100, 100, 0 }, glm::vec2{ 2, 2 }, "player");
	qBert->AddComponent(new dae::RenderComponent("QBert.png", { 16, -16 }), true);
	qBert->AddComponent(new HealthComponent());
	qBert->AddComponent(new SubjectComponent());
	qBert->AddComponent(new ScoreComponent());
	qBert->AddComponent(new InputComponent());
	qBert->AddComponent(new TimerComponent(0.35f));
	qBert->AddComponent(qbertMovement);
	qBert->AddComponent(new QBertBehaviourComponent());
	scene.Add(qBert);

	// Enemy spawner
	const auto spawner = std::make_shared<SceneObject>();
	spawner->AddComponent(new TimerComponent(4.f));
	spawner->AddComponent(new EnemySpawner());
	scene.Add(spawner);

	scene.Init();
	// ------------------------------------------
#pragma endregion
}

void GamemodeManager::LoadCoop()
{
	if (m_CurrentGamemode == Gamemode::eCoop)
		return;

	m_CurrentGamemode = Gamemode::eCoop;

#pragma region Load Co-op Level
	// -------------- LOAD LEVEL ----------------
	using pComponentVec = std::vector<BaseComponent*>;
	const auto sceneName = "Co-op";

	// If scene already exists, set it and return
	if (SceneManager::GetInstance().SetActiveScene(sceneName))
		return;

	auto& scene = SceneManager::GetInstance().CreateScene(sceneName);

	const auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 27);

	// Map
	const auto map = std::make_shared<SceneObject>(pComponentVec{}, glm::vec3{ 288,64,0 });
	map->AddComponent(new TileMapComponent(), true);
	map->AddComponent(new SubjectComponent());
	map->SetTag("tileMap");
	map->RegisterAsObserver(this);
	scene.Add(map);

	// FPS display
	auto* const pRenderComponent = new RenderComponent();
	auto* const pTextComponent = new TextComponent("60", font, SDL_Color{ 255,255,255 });
	auto* const pFPSComponent = new FPSComponent();

	const auto fpsObject = std::make_shared<SceneObject>(pComponentVec{ pFPSComponent, pTextComponent });
	fpsObject->AddComponent(pRenderComponent, true);

	scene.Add(fpsObject);

	// Lives display
	const auto livesDisplay = std::make_shared<SceneObject>(pComponentVec{}, glm::vec3{ 0,30,0 });
	livesDisplay->AddComponent(new TextComponent("3 lives", font));
	livesDisplay->AddComponent(new RenderComponent(), true);
	livesDisplay->AddComponent(new HealthDisplayComponent(0));
	scene.Add(livesDisplay);

	// Lives display right
	const auto livesDisplayRight = std::make_shared<SceneObject>(pComponentVec{}, glm::vec3{ 400,30,0 });
	livesDisplayRight->AddComponent(new TextComponent(" ", font));
	livesDisplayRight->AddComponent(new RenderComponent(), true);
	livesDisplayRight->AddComponent(new HealthDisplayComponent(1));
	scene.Add(livesDisplayRight);

	// Score display left
	const auto scoreDisplay = std::make_shared<SceneObject>(pComponentVec{}, glm::vec3{ 0, 60, 0 });
	scoreDisplay->AddComponent(new TextComponent("Score: 0", font));
	scoreDisplay->AddComponent(new RenderComponent(), true);
	scoreDisplay->AddComponent(new ScoreDisplayComponent(0));

	scene.Add(scoreDisplay);

	// Score display right
	const auto scoreDisplayRight = std::make_shared<SceneObject>(pComponentVec{}, glm::vec3{ 400, 60, 0 });
	scoreDisplayRight->AddComponent(new TextComponent(" ", font));
	scoreDisplayRight->AddComponent(new RenderComponent(), true);
	scoreDisplayRight->AddComponent(new ScoreDisplayComponent(1));

	scene.Add(scoreDisplayRight);

	{	// Qbert
		const auto qbertMovement = new PosessedMovementComponent();
		qbertMovement->SetBlockIndex({ 0,6 });
		const auto qBert = std::make_shared<SceneObject>(pComponentVec{}, glm::vec3{ 100, 100, 0 }, glm::vec2{ 2, 2 }, "player");
		qBert->AddComponent(new dae::RenderComponent("QBert.png", { 16, -16 }), true);
		qBert->AddComponent(new HealthComponent());
		qBert->AddComponent(new SubjectComponent());
		qBert->AddComponent(new ScoreComponent());
		qBert->AddComponent(new InputComponent());
		qBert->AddComponent(new TimerComponent(0.35f));
		qBert->AddComponent(qbertMovement);
		qBert->AddComponent(new QBertBehaviourComponent());
		scene.Add(qBert);
	}
	{	// Qbert 2
		const auto qbertMovement = new PosessedMovementComponent();
		qbertMovement->SetBlockIndex({ 6,6 });
		const auto qBert = std::make_shared<SceneObject>(pComponentVec{}, glm::vec3{ 100, 100, 0 }, glm::vec2{ 2, 2 }, "player");
		qBert->AddComponent(new dae::RenderComponent("QBert.png", { 16, -16 }), true);
		qBert->AddComponent(new HealthComponent());
		qBert->AddComponent(new SubjectComponent());
		qBert->AddComponent(new ScoreComponent());
		qBert->AddComponent(new InputComponent(false));
		qBert->AddComponent(new TimerComponent(0.35f));
		qBert->AddComponent(qbertMovement);
		qBert->AddComponent(new QBertBehaviourComponent());
		scene.Add(qBert);
	}

	// Enemy spawner
	const auto spawner = std::make_shared<SceneObject>();
	spawner->AddComponent(new TimerComponent(4.f));
	spawner->AddComponent(new EnemySpawner());
	scene.Add(spawner);

	scene.Init();
	// ------------------------------------------
#pragma endregion

}

void GamemodeManager::LoadVersus()
{
	if (m_CurrentGamemode == Gamemode::eVersus)
		return;

	m_CurrentGamemode = Gamemode::eVersus;

#pragma region Load versus level
	// -------------- LOAD LEVEL ----------------
	using pComponentVec = std::vector<BaseComponent*>;
	const auto sceneName = "Versus";

	// If scene already exists, set it and return
	if (SceneManager::GetInstance().SetActiveScene(sceneName))
		return;

	auto& scene = SceneManager::GetInstance().CreateScene(sceneName);

	const auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 27);

	// Map
	const auto map = std::make_shared<SceneObject>(pComponentVec{}, glm::vec3{ 288,64,0 });
	map->AddComponent(new TileMapComponent(), true);
	map->AddComponent(new SubjectComponent());
	map->SetTag("tileMap");
	map->RegisterAsObserver(this);
	scene.Add(map);

	// FPS display
	auto* const pRenderComponent = new RenderComponent();
	auto* const pTextComponent = new TextComponent("60", font, SDL_Color{ 255,255,255 });
	auto* const pFPSComponent = new FPSComponent();

	const auto fpsObject = std::make_shared<SceneObject>(pComponentVec{ pFPSComponent, pTextComponent });
	fpsObject->AddComponent(pRenderComponent, true);

	scene.Add(fpsObject);

	// Lives display
	const auto livesDisplay = std::make_shared<SceneObject>(pComponentVec{}, glm::vec3{ 0,30,0 });
	livesDisplay->AddComponent(new TextComponent("3 lives", font));
	livesDisplay->AddComponent(new RenderComponent(), true);
	livesDisplay->AddComponent(new HealthDisplayComponent(0));
	scene.Add(livesDisplay);

	// Score display
	const auto scoreDisplay = std::make_shared<SceneObject>(pComponentVec{}, glm::vec3{ 0, 60, 0 });
	scoreDisplay->AddComponent(new TextComponent("Score: 0", font));
	scoreDisplay->AddComponent(new RenderComponent(), true);
	scoreDisplay->AddComponent(new ScoreDisplayComponent(0));

	scene.Add(scoreDisplay);

	// Qbert
	const auto qbertMovement = new PosessedMovementComponent();
	qbertMovement->SetBlockIndex({ 0,0 });
	const auto qBert = std::make_shared<SceneObject>(pComponentVec{}, glm::vec3{ 100, 100, 0 }, glm::vec2{ 2, 2 }, "player");
	qBert->AddComponent(new dae::RenderComponent("QBert.png", { 16, -16 }), true);
	qBert->AddComponent(new HealthComponent());
	qBert->AddComponent(new SubjectComponent());
	qBert->AddComponent(new ScoreComponent());
	qBert->AddComponent(new InputComponent());
	qBert->AddComponent(new TimerComponent(0.35f));
	qBert->AddComponent(qbertMovement);
	qBert->AddComponent(new QBertBehaviourComponent());
	scene.Add(qBert);

	// Enemy spawner
	const auto spawner = std::make_shared<SceneObject>();
	spawner->AddComponent(new TimerComponent(4.f));
	spawner->AddComponent(new EnemySpawner(true));
	scene.Add(spawner);

	scene.Init();
	// ------------------------------------------

#pragma endregion


	dae::Logger::GetInstance().Print("LoadVersus");
}

void GamemodeManager::StopPlaying()
{
	// Remove scenes
	m_CurrentGamemode = Gamemode::eNone;
	SceneManager::GetInstance().RemoveSceneByName("AI");
	SceneManager::GetInstance().RemoveSceneByName("Co-op");
	SceneManager::GetInstance().RemoveSceneByName("Versus");

	// All gamemodes reconfigure it themselves, remove old settings
	InputManager::GetInstance().Reset();

	// Create an empty scene, otherwise the current one gets kept alive
	const auto sceneName = "Empty";

	// If scene already exists, set it and return
	if (SceneManager::GetInstance().SetActiveScene(sceneName))
		return;

	SceneManager::GetInstance().CreateScene(sceneName);

}

void GamemodeManager::LoadGameOver()
{
	StopPlaying();
	m_GameOver = false;

	m_CurrentGamemode = Gamemode::eGameOver;

	// -------------- LOAD LEVEL ----------------
	using pComponentVec = std::vector<BaseComponent*>;
	const auto sceneName = "Versus";

	// If scene already exists, set it and return
	if (SceneManager::GetInstance().SetActiveScene(sceneName))
		return;

	auto& scene = SceneManager::GetInstance().CreateScene(sceneName);

	const auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 40);

	// FPS display
	auto* const pRenderComponent = new RenderComponent();
	auto* const pTextComponent = new TextComponent("Game over!", font, SDL_Color{ 255,255,255 });

	const auto fpsObject = std::make_shared<SceneObject>(pComponentVec{ pTextComponent }, glm::vec3{60, 200, 0});
	fpsObject->AddComponent(pRenderComponent, true);

	scene.Add(fpsObject);

	scene.Init();
}

void GamemodeManager::OnNotify(const BaseComponent*, const std::string& message)
{
	if (message == "game over")
	{
		m_GameOver = true;
	}
}
