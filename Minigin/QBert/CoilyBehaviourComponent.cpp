#include "CoilyBehaviourComponent.h"
#include "QBertBehaviourComponent.h"
#include "TileMapComponent.h"
#include "RenderComponent.h"
#include "SceneObject.h"
#include "Scene.h"

CoilyBehaviourComponent::CoilyBehaviourComponent()
	: m_Index({0, 0})
	, m_IsEgg(true)
	, m_pTileMapRef(nullptr)
	, m_QBertRef()
	, m_pTimerCompRef(nullptr)
{
}

void CoilyBehaviourComponent::Init(dae::SceneObject& parent)
{
	m_pTimerCompRef = parent.GetFirstComponentOfType<dae::TimerComponent>();

	auto tileMapObj = parent.GetScene()->GetObjectsByTag("tileMap");
	m_pTileMapRef = tileMapObj[0]->GetFirstComponentOfType<TileMapComponent>();

	const auto players = parent.GetScene()->GetObjectsByTag("player");
	m_QBertRef = players[0];

	m_pRenderCompRef = parent.GetFirstComponentOfType<dae::RenderComponent>();
}

void CoilyBehaviourComponent::Update(dae::SceneObject& parent)
{
	KillQbertIfClose(parent);

	if (!m_pTimerCompRef->TimerCompleted())
		return;

	m_pTimerCompRef->Reset();

	m_IsEgg ? UpdateEgg(parent) : UpdateCoily(parent);
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

void CoilyBehaviourComponent::UpdateEgg(dae::SceneObject& parent)
{
	// Semi-random movement
	const auto movement = int2(std::rand() & 0b01 ? 1 : 0, 1);
	const auto nextIndex = int2{ m_Index.first + movement.first, m_Index.second + movement.second };
	const auto isNextLocationValid = m_pTileMapRef->IsBlockIndexValid(nextIndex);

	if (isNextLocationValid)
		m_Index = nextIndex;
	else
	{
		m_IsEgg = false;
		m_pRenderCompRef->SetTexture("Coily.png");
	}

	const auto pos = m_pTileMapRef->IndexToTilePosition(m_Index);
	parent.SetPosition(pos.x, pos.y);
}

void CoilyBehaviourComponent::UpdateCoily(dae::SceneObject& parent)
{
	if (m_QBertRef.expired())
		return;

	const auto qbert = m_QBertRef.lock();
	const auto qbertPos = qbert->GetTransform()->GetPosition();
	const auto pos = parent.GetTransform()->GetPosition();

	// Move towards qbert 
	bool goLeft = qbertPos.x < pos.x;
	bool goUp = qbertPos.y < pos.y;

	int2 movement;
	movement.second = goUp ? -1 : 1;

	if (goUp)
		movement.first = goLeft ? -1 : 0;
	else
		movement.first = goLeft ? 0 : 1;


	const auto nextIndex = int2{ m_Index.first + movement.first, m_Index.second + movement.second };


	m_Index = nextIndex;
	const auto newPos = m_pTileMapRef->IndexToTilePosition(nextIndex);
	parent.SetPosition(newPos.x, newPos.y);
}
