#include "EnemySpawner.h"
#include "SceneObject.h"
#include "Scene.h"

#include "TimerComponent.h"
#include "RenderComponent.h"
#include "CoilyBehaviourComponent.h"
#include "DestroyOnPlayerDamageComponent.h"
#include "DefaultMovement.h"
#include "KillPlayerOnTouchComponent.h"
#include "DestroyOnLevelChange.h"
#include "SlickAndSamBehaviourComponent.h"
#include "UggMovementComponent.h"
#include "WrongwayMovementComponent.h"
#include "PosessedMovementComponent.h"

using namespace dae;
using pComponentVec = std::vector<BaseComponent*>;


EnemySpawner::EnemySpawner(bool isCoilyPlayer)
	: m_CoilyIsPlayer(isCoilyPlayer)
{
}

void EnemySpawner::Init(SceneObject& parent)
{
	m_pTimerCompRef = parent.GetFirstComponentOfType<TimerComponent>();
	m_pSceneRef = SceneManager::GetInstance().GetActiveScene();
}

void EnemySpawner::Update(dae::SceneObject& )
{
	if (!m_pTimerCompRef->TimerCompleted())
		return;

	m_pTimerCompRef->Reset();

	if (m_pSceneRef.expired())
		return;

	// Always respawn coily if he's being controlled by a player
	if (m_CoilyIsPlayer && m_pCoilyRef.expired())
	{
		SpawnCoily(m_CoilyIsPlayer);
		return;
	}

	int Nr = std::rand() % 3;
	switch (Nr)
	{
	case 0:
		SpawnCoily();
		break;
	case 1:
		SpawnUggOrWrongway();
		break;
	case 2:
		SpawnSlickOrSam();
		break;
	}
}

void EnemySpawner::SpawnCoily(bool posessed)
{
	if (m_pSceneRef.expired())
		return;

	const auto coilyRenderer = new RenderComponent("Coily_egg.png", { 16, -16 });
	const auto coilyBehaviour = new CoilyBehaviourComponent(posessed);
	const auto coilyTimer = new TimerComponent(posessed ? 0.25f : 1);
	const auto destroyOnReset = new DestroyOnPlayerDamageComponent();
	const auto eggMovement = new DefaultMovement();
	const auto dmgPlayerComp = new KillPlayerOnTouchComponent();
	const auto coily = std::make_shared<SceneObject>(pComponentVec{ coilyTimer, eggMovement, coilyBehaviour, destroyOnReset, dmgPlayerComp }, glm::vec3{ -100, -100, 0 }, glm::vec2{ 2, 2 }, "coily");
	coily->AddComponent(coilyRenderer, true);
	coily->AddComponent(new DestroyOnLevelChange());

	if (posessed)
		coily->AddComponent(new PosessedMovementComponent(false, false, false, false));

	m_pSceneRef.lock()->AddAfterInitialize(coily);

	if (m_CoilyIsPlayer)
		m_pCoilyRef = coily;

}

void EnemySpawner::SpawnUggOrWrongway()
{
	std::rand() & 0b01 ? SpawnUgg() : SpawnWrongway();
}

void EnemySpawner::SpawnSlickOrSam()
{
	if (m_pSceneRef.expired())
		return;

	const auto name = std::string(std::rand() & 0b01 ? "Slick" : "Sam");

	// Slick or sam
	const auto renderer = new RenderComponent(name + ".png", { 16, -16 });
	const auto destroyOnReset = new DestroyOnPlayerDamageComponent();
	const auto timer = new TimerComponent(1);
	const auto behaviour = new SlickAndSamBehaviourComponent();
	const auto movement = new DefaultMovement(false, true, true);
	const auto obj = std::make_shared<SceneObject>(pComponentVec{ timer, movement, behaviour, destroyOnReset }, glm::vec3{ -100, -100, 0 }, glm::vec2{ 2, 2 }, name);
	obj->AddComponent(renderer, true);
	obj->AddComponent(new DestroyOnLevelChange());
	m_pSceneRef.lock()->AddAfterInitialize(obj);
}

void EnemySpawner::SpawnWrongway()
{
	if (m_pSceneRef.expired())
		return;

	const auto wrongWayRenderer = new RenderComponent("Wrongway.png", { 16, -16 });
	const auto destroywrongWayOnReset = new DestroyOnPlayerDamageComponent();
	const auto wrongWayTimer = new TimerComponent(1);
	const auto wrongWayMovement = new WrongwayMovementComponent();
	const auto dmgPlayerComp = new KillPlayerOnTouchComponent();
	const auto wrongWay = std::make_shared<SceneObject>(pComponentVec{ wrongWayTimer, wrongWayMovement, destroywrongWayOnReset, dmgPlayerComp }, glm::vec3{ -100, -100, 0 }, glm::vec2{ 2, 2 }, "wrongWay");
	wrongWay->AddComponent(wrongWayRenderer, true);
	wrongWay->AddComponent(new DestroyOnLevelChange());
	m_pSceneRef.lock()->AddAfterInitialize(wrongWay);
}

void EnemySpawner::SpawnUgg()
{
	if (m_pSceneRef.expired())
		return;

	const auto uggRenderer = new RenderComponent("Ugg.png", { 16, -16 });
	const auto destroyUggOnReset = new DestroyOnPlayerDamageComponent();
	const auto uggTimer = new TimerComponent(1);
	const auto dmgPlayerComp = new KillPlayerOnTouchComponent();
	const auto uggMovement = new UggMovementComponent();
	const auto ugg = std::make_shared<SceneObject>(pComponentVec{ uggTimer, uggMovement, destroyUggOnReset, dmgPlayerComp }, glm::vec3{ -100, -100, 0 }, glm::vec2{ 2, 2 }, "ugg");
	ugg->AddComponent(uggRenderer, true);
	ugg->AddComponent(new DestroyOnLevelChange());
	m_pSceneRef.lock()->AddAfterInitialize(ugg);
}
