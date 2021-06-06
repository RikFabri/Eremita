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
#include "PosessedMovementComponent.h"
#include "DestroyOnLevelChange.h"

using namespace dae;

GamemodeManager::GamemodeManager()
	: m_CurrentGamemode(Gamemode::eNone)
	, m_IsOpen(true)
{
}

void GamemodeManager::Render()
{
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
	qBert->AddComponent(new TimerComponent(0.5f));
	qBert->AddComponent(qbertMovement);
	qBert->AddComponent(new QBertBehaviourComponent());
	scene.Add(qBert);

	// Coily
	const auto coilyRenderer = new RenderComponent("Coily_egg.png", { 16, -16 });
	const auto coilyBehaviour = new CoilyBehaviourComponent();
	const auto coilyTimer = new TimerComponent(1);
	const auto destroyOnReset = new DestroyOnPlayerDamageComponent();
	const auto eggMovement = new DefaultMovement();
	auto dmgPlayerComp = new KillPlayerOnTouchComponent();
	const auto coily = std::make_shared<SceneObject>(pComponentVec{ coilyTimer, eggMovement, coilyBehaviour, destroyOnReset, dmgPlayerComp }, glm::vec3{ -100, -100, 0 }, glm::vec2{ 2, 2 }, "coily");
	coily->AddComponent(coilyRenderer, true);
	coily->AddComponent(new DestroyOnLevelChange());
	scene.Add(coily);

	// Slick
	const auto slickRenderer = new RenderComponent("Slick.png", { 16, -16 });
	const auto destroySlickOnReset = new DestroyOnPlayerDamageComponent();
	const auto slickTimer = new TimerComponent(1);
	const auto slickBehaviour = new SlickAndSamBehaviourComponent();
	const auto slickMovement = new DefaultMovement(false, true, true);
	const auto slick = std::make_shared<SceneObject>(pComponentVec{ slickTimer, slickMovement, slickBehaviour, destroySlickOnReset }, glm::vec3{ -100, -100, 0 }, glm::vec2{ 2, 2 }, "slick");
	slick->AddComponent(slickRenderer, true);
	slick->AddComponent(new DestroyOnLevelChange());
	scene.Add(slick);

	// Sam
	const auto samRenderer = new RenderComponent("Sam.png", { 16, -16 });
	const auto destroySamOnReset = new DestroyOnPlayerDamageComponent();
	const auto samTimer = new TimerComponent(1);
	const auto samBehaviour = new SlickAndSamBehaviourComponent();
	const auto samMovement = new DefaultMovement(false, true, true);
	const auto sam = std::make_shared<SceneObject>(pComponentVec{ samTimer, samMovement, samBehaviour, destroySamOnReset }, glm::vec3{ -100, -100, 0 }, glm::vec2{ 2, 2 }, "sam");
	sam->AddComponent(samRenderer, true);
	sam->AddComponent(new DestroyOnLevelChange());
	scene.Add(sam);

	// Ugg
	const auto uggRenderer = new RenderComponent("Ugg.png", { 16, -16 });
	const auto destroyUggOnReset = new DestroyOnPlayerDamageComponent();
	const auto uggTimer = new TimerComponent(1);
	dmgPlayerComp = new KillPlayerOnTouchComponent();
	const auto uggMovement = new UggMovementComponent();
	const auto ugg = std::make_shared<SceneObject>(pComponentVec{ uggTimer, uggMovement, destroyUggOnReset, dmgPlayerComp }, glm::vec3{ -100, -100, 0 }, glm::vec2{ 2, 2 }, "ugg");
	ugg->AddComponent(uggRenderer, true);
	ugg->AddComponent(new DestroyOnLevelChange());
	scene.Add(ugg);

	// Wrongway
	const auto wrongWayRenderer = new RenderComponent("Wrongway.png", { 16, -16 });
	const auto destroywrongWayOnReset = new DestroyOnPlayerDamageComponent();
	const auto wrongWayTimer = new TimerComponent(1);
	const auto wrongWayMovement = new WrongwayMovementComponent();
	dmgPlayerComp = new KillPlayerOnTouchComponent();
	const auto wrongWay = std::make_shared<SceneObject>(pComponentVec{ wrongWayTimer, wrongWayMovement, destroywrongWayOnReset, dmgPlayerComp }, glm::vec3{ -100, -100, 0 }, glm::vec2{ 2, 2 }, "wrongWay");
	wrongWay->AddComponent(wrongWayRenderer, true);
	wrongWay->AddComponent(new DestroyOnLevelChange());
	scene.Add(wrongWay);

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
	map->SetTag("tileMap");
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
		qBert->AddComponent(new TimerComponent(0.5f));
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
		qBert->AddComponent(new TimerComponent(0.5f));
		qBert->AddComponent(qbertMovement);
		qBert->AddComponent(new QBertBehaviourComponent());
		scene.Add(qBert);
	}

	// Coily
	const auto coilyRenderer = new RenderComponent("Coily_egg.png", { 16, -16 });
	const auto coilyBehaviour = new CoilyBehaviourComponent();
	const auto coilyTimer = new TimerComponent(1);
	const auto destroyOnReset = new DestroyOnPlayerDamageComponent();
	const auto eggMovement = new DefaultMovement();
	auto dmgPlayerComp = new KillPlayerOnTouchComponent();
	const auto coily = std::make_shared<SceneObject>(pComponentVec{ coilyTimer, eggMovement, coilyBehaviour, destroyOnReset, dmgPlayerComp }, glm::vec3{ -100, -100, 0 }, glm::vec2{ 2, 2 }, "coily");
	coily->AddComponent(coilyRenderer, true);
	scene.Add(coily);

	// Slick
	const auto slickRenderer = new RenderComponent("Slick.png", { 16, -16 });
	const auto destroySlickOnReset = new DestroyOnPlayerDamageComponent();
	const auto slickTimer = new TimerComponent(1);
	const auto slickBehaviour = new SlickAndSamBehaviourComponent();
	const auto slickMovement = new DefaultMovement(false, true, true);
	const auto slick = std::make_shared<SceneObject>(pComponentVec{ slickTimer, slickMovement, slickBehaviour, destroySlickOnReset }, glm::vec3{ -100, -100, 0 }, glm::vec2{ 2, 2 }, "slick");
	slick->AddComponent(slickRenderer, true);
	scene.Add(slick);

	// Sam
	const auto samRenderer = new RenderComponent("Sam.png", { 16, -16 });
	const auto destroySamOnReset = new DestroyOnPlayerDamageComponent();
	const auto samTimer = new TimerComponent(1);
	const auto samBehaviour = new SlickAndSamBehaviourComponent();
	const auto samMovement = new DefaultMovement(false, true, true);
	const auto sam = std::make_shared<SceneObject>(pComponentVec{ samTimer, samMovement, samBehaviour, destroySamOnReset }, glm::vec3{ -100, -100, 0 }, glm::vec2{ 2, 2 }, "sam");
	sam->AddComponent(samRenderer, true);
	scene.Add(sam);

	//// Ugg
	//const auto uggRenderer = new RenderComponent("Ugg.png", { 16, -16 });
	//const auto destroyUggOnReset = new DestroyOnPlayerDamageComponent();
	//const auto uggTimer = new TimerComponent(1);
	//dmgPlayerComp = new KillPlayerOnTouchComponent();
	//const auto uggMovement = new UggMovementComponent();
	//const auto ugg = std::make_shared<SceneObject>(pComponentVec{ uggTimer, uggMovement, destroyUggOnReset, dmgPlayerComp }, glm::vec3{ -100, -100, 0 }, glm::vec2{ 2, 2 }, "ugg");
	//ugg->AddComponent(uggRenderer, true);
	//scene.Add(ugg);

	//// Wrongway
	//const auto wrongWayRenderer = new RenderComponent("Wrongway.png", { 16, -16 });
	//const auto destroywrongWayOnReset = new DestroyOnPlayerDamageComponent();
	//const auto wrongWayTimer = new TimerComponent(1);
	//const auto wrongWayMovement = new WrongwayMovementComponent();
	//dmgPlayerComp = new KillPlayerOnTouchComponent();
	//const auto wrongWay = std::make_shared<SceneObject>(pComponentVec{ wrongWayTimer, wrongWayMovement, destroywrongWayOnReset, dmgPlayerComp }, glm::vec3{ -100, -100, 0 }, glm::vec2{ 2, 2 }, "wrongWay");
	//wrongWay->AddComponent(wrongWayRenderer, true);
	//scene.Add(wrongWay);

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
	map->SetTag("tileMap");
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
	qBert->AddComponent(new TimerComponent(0.5f));
	qBert->AddComponent(qbertMovement);
	qBert->AddComponent(new QBertBehaviourComponent());
	scene.Add(qBert);

	// Coily
	const auto coilyRenderer = new RenderComponent("Coily_egg.png", { 16, -16 });
	const auto coilyBehaviour = new CoilyBehaviourComponent(true);
	const auto coilyTimer = new TimerComponent(0.5f);
	const auto destroyOnReset = new DestroyOnPlayerDamageComponent();
	const auto eggMovement = new DefaultMovement();
	const auto possessedMovement = new PosessedMovementComponent(false, false, false, false);
	auto dmgPlayerComp = new KillPlayerOnTouchComponent();
	const auto coily = std::make_shared<SceneObject>(pComponentVec{ possessedMovement, coilyTimer, eggMovement, coilyBehaviour, destroyOnReset, dmgPlayerComp }, glm::vec3{ -100, -100, 0 }, glm::vec2{ 2, 2 }, "coily");
	coily->AddComponent(coilyRenderer, true);
	scene.Add(coily);

	// Slick
	const auto slickRenderer = new RenderComponent("Slick.png", { 16, -16 });
	const auto destroySlickOnReset = new DestroyOnPlayerDamageComponent();
	const auto slickTimer = new TimerComponent(1);
	const auto slickBehaviour = new SlickAndSamBehaviourComponent();
	const auto slickMovement = new DefaultMovement(false, true, true);
	const auto slick = std::make_shared<SceneObject>(pComponentVec{ slickTimer, slickMovement, slickBehaviour, destroySlickOnReset }, glm::vec3{ -100, -100, 0 }, glm::vec2{ 2, 2 }, "slick");
	slick->AddComponent(slickRenderer, true);
	scene.Add(slick);

	// Sam
	const auto samRenderer = new RenderComponent("Sam.png", { 16, -16 });
	const auto destroySamOnReset = new DestroyOnPlayerDamageComponent();
	const auto samTimer = new TimerComponent(1);
	const auto samBehaviour = new SlickAndSamBehaviourComponent();
	const auto samMovement = new DefaultMovement(false, true, true);
	const auto sam = std::make_shared<SceneObject>(pComponentVec{ samTimer, samMovement, samBehaviour, destroySamOnReset }, glm::vec3{ -100, -100, 0 }, glm::vec2{ 2, 2 }, "sam");
	sam->AddComponent(samRenderer, true);
	scene.Add(sam);

	// Ugg
	const auto uggRenderer = new RenderComponent("Ugg.png", { 16, -16 });
	const auto destroyUggOnReset = new DestroyOnPlayerDamageComponent();
	const auto uggTimer = new TimerComponent(1);
	dmgPlayerComp = new KillPlayerOnTouchComponent();
	const auto uggMovement = new UggMovementComponent();
	const auto ugg = std::make_shared<SceneObject>(pComponentVec{ uggTimer, uggMovement, destroyUggOnReset, dmgPlayerComp }, glm::vec3{ -100, -100, 0 }, glm::vec2{ 2, 2 }, "ugg");
	ugg->AddComponent(uggRenderer, true);
	scene.Add(ugg);

	// Wrongway
	const auto wrongWayRenderer = new RenderComponent("Wrongway.png", { 16, -16 });
	const auto destroywrongWayOnReset = new DestroyOnPlayerDamageComponent();
	const auto wrongWayTimer = new TimerComponent(1);
	const auto wrongWayMovement = new WrongwayMovementComponent();
	dmgPlayerComp = new KillPlayerOnTouchComponent();
	const auto wrongWay = std::make_shared<SceneObject>(pComponentVec{ wrongWayTimer, wrongWayMovement, destroywrongWayOnReset, dmgPlayerComp }, glm::vec3{ -100, -100, 0 }, glm::vec2{ 2, 2 }, "wrongWay");
	wrongWay->AddComponent(wrongWayRenderer, true);
	scene.Add(wrongWay);

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
