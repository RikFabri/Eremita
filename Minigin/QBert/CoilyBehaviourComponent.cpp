#include "CoilyBehaviourComponent.h"
#include "QBertBehaviourComponent.h"
#include "CoilyMovementComponent.h"
#include "TileMapComponent.h"
#include "RenderComponent.h"
#include "DefaultMovement.h"
#include "SceneObject.h"
#include "Scene.h"

CoilyBehaviourComponent::CoilyBehaviourComponent()
	: m_Index({0, 0})
	, m_pTileMapRef(nullptr)
	, m_QBertRef()
	, m_pTimerCompRef(nullptr)
{
}

void CoilyBehaviourComponent::Init(dae::SceneObject& parent)
{
	using namespace std::placeholders;

	m_pTimerCompRef = parent.GetFirstComponentOfType<dae::TimerComponent>();

	auto tileMapObj = parent.GetScene()->GetObjectsByTag("tileMap");
	m_pTileMapRef = tileMapObj[0]->GetFirstComponentOfType<TileMapComponent>();

	const auto players = parent.GetScene()->GetObjectsByTag("player");
	m_QBertRef = players[0];

	m_pRenderCompRef = parent.GetFirstComponentOfType<dae::RenderComponent>();

	const auto defaultMovement = parent.GetFirstComponentOfType<DefaultMovement>();
	defaultMovement->SetReachedEnd(std::bind(&CoilyBehaviourComponent::HatchEgg, this, _1));
	m_pDefaultMovement = defaultMovement;
}

void CoilyBehaviourComponent::Update(dae::SceneObject& parent)
{
	KillQbertIfClose(parent);
}

void CoilyBehaviourComponent::KillQbertIfClose(dae::SceneObject& parent)
{
	const int killDistance = 8;

	if (m_QBertRef.expired())
		return;

	const auto qbert = m_QBertRef.lock();
	const auto qbertPos = qbert->GetTransform()->GetPosition();
	const auto pos = parent.GetTransform()->GetPosition();


	const auto coilyToQbertDistance = glm::distance(qbertPos, pos);

	if (coilyToQbertDistance < killDistance)
	{
		const auto qbertBehaviour = qbert->GetFirstComponentOfType<QBertBehaviourComponent>();
		qbertBehaviour->Damage();
	}
}

void CoilyBehaviourComponent::HatchEgg(dae::SceneObject& parent)
{
	m_pRenderCompRef->SetTexture("Coily.png");

	const auto defaultMovement = static_cast<DefaultMovement*>(m_pDefaultMovement);
	auto coilyMovement = new CoilyMovementComponent();
	// Set the new movement's position on the tiles grid
	coilyMovement->SetIndex(defaultMovement->GetBlockIndex());
	coilyMovement->Init(parent);

	// Swap movement components
	parent.RemoveComponent(defaultMovement);
	parent.AddComponentAfterUpdate(coilyMovement);

	m_pDefaultMovement = coilyMovement;
}