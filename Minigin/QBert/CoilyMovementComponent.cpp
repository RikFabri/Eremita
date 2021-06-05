#include "CoilyMovementComponent.h"
#include "TileMapComponent.h"
#include "TimerComponent.h"
#include "SceneObject.h"
#include "Scene.h"
#include <utility>


void CoilyMovementComponent::Init(dae::SceneObject& parent)
{
	const auto players = parent.GetScene()->GetObjectsByTag("player");
	m_QBertRef = players[0];

	auto tileMapObj = parent.GetScene()->GetObjectsByTag("tileMap");
	m_pTileMapRef = tileMapObj[0]->GetFirstComponentOfType<TileMapComponent>();

	m_pTimerCompRef = parent.GetFirstComponentOfType<dae::TimerComponent>();
}

void CoilyMovementComponent::Update(dae::SceneObject& parent)
{
	if (!m_pTimerCompRef->TimerCompleted())
		return;

	m_pTimerCompRef->Reset();


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

void CoilyMovementComponent::SetIndex(const int2& idx)
{
	m_Index = idx;
}
