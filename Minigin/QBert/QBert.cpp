#include <Eremita.h>
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

#include <Logger.h>

#include "TileComponent.h"
#include "TileMapComponent.h"
#include "QBertBehaviourComponent.h"
#include "InputComponent.h"
#include "CoilyBehaviourComponent.h"
#include "DestroyOnPlayerDamageComponent.h"

using namespace dae;

void LoadGame();

int main(int, char* []) {
	Eremita engine;
	engine.Initialize();
	LoadGame();
	engine.Run();

	return 0;
}

void LoadGame()
{
	using pComponentVec = std::vector<BaseComponent*>;

	auto& scene = SceneManager::GetInstance().CreateScene("Demo");

	const auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 27);

	// Map
	const auto map = std::make_shared<SceneObject>(pComponentVec{}, glm::vec3{288,64,0});
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

	// Qbert
	const auto qBert = std::make_shared<SceneObject>(pComponentVec{}, glm::vec3{ 100, 100, 0 }, glm::vec2{ 2, 2 }, "player");
	qBert->AddComponent(new HealthComponent());
	qBert->AddComponent(new SubjectComponent());
	qBert->AddComponent(new ScoreComponent());
	qBert->AddComponent(new InputComponent());
	qBert->AddComponent(new QBertBehaviourComponent());
	qBert->SetTag("player");
	scene.Add(qBert);

	// Qbert 2 
	const auto qBert2 = std::make_shared<SceneObject>();
	qBert2->AddComponent(new HealthComponent());
	qBert2->AddComponent(new SubjectComponent());
	qBert2->AddComponent(new ScoreComponent());
	qBert2->AddComponent(new InputComponent());
	qBert2->SetTag("player");
	scene.Add(qBert2);

	for (int i = 0; i < 100; ++i)
	{
		// Coily
		const auto coilyRenderer = new RenderComponent("Coily_egg.png", { 16, -16 });
		const auto coilyBehaviour = new CoilyBehaviourComponent();
		const auto coilyTimer = new TimerComponent(1);
		const auto destroyOnReset = new DestroyOnPlayerDamageComponent();
		const auto coily = std::make_shared<SceneObject>(pComponentVec{ coilyTimer, coilyBehaviour, destroyOnReset }, glm::vec3{ -100, -100, 0 }, glm::vec2{ 2, 2 }, "coily");
		coily->AddComponent(coilyRenderer, true);
		scene.Add(coily);
	}
	scene.Init();

	//To-Do: remove this
	//Logger::GetInstance().ClearLog(); // To clear the warning in case the teachers only plug in one controller.
	Logger::GetInstance().Print("\n-----------------------------------------------------------");
	Logger::GetInstance().Print("All D-Pad buttons add a certain amount of points, \nthe A button costs one life. Keyboard input isn't used yet");
	Logger::GetInstance().Print("controller-B plays a sound, X mutes/unmutes");
	Logger::GetInstance().Print("-----------------------------------------------------------");

}