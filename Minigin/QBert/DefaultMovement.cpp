#include "DefaultMovement.h"
#include "TileMapComponent.h"
#include "TimerComponent.h"
#include "SceneObject.h"
#include "Scene.h"
#include <utility>
#include <random>

DefaultMovement::DefaultMovement(bool moveUp)
	: m_MoveUp(moveUp)
{
}

void DefaultMovement::Init(dae::SceneObject& parent)
{
	auto tileMapObj = parent.GetScene()->GetObjectsByTag("tileMap");
	m_pTileMapRef = tileMapObj[0]->GetFirstComponentOfType<TileMapComponent>();

	m_pTimerCompRef = parent.GetFirstComponentOfType<dae::TimerComponent>();
}

void DefaultMovement::Update(dae::SceneObject& parent)
{
	if (!m_pTimerCompRef->TimerCompleted())
		return;

	m_pTimerCompRef->Reset();

	// Semi-random movement
	auto movement = int2(std::rand() & 0b01 ? 1 : 0, 1);
	if (m_MoveUp)
	{
		movement.first *= -1;
		movement.second *= -1;
	}
	const auto nextIndex = int2{ m_Index.first + movement.first, m_Index.second + movement.second };
	const auto isNextLocationValid = m_pTileMapRef->IsBlockIndexValid(nextIndex);

	if (isNextLocationValid)
		m_Index = nextIndex;
	else
	{
		if (m_ReachedEndCallback)
			m_ReachedEndCallback(parent);
	}

	const auto pos = m_pTileMapRef->IndexToTilePosition(m_Index);
	parent.SetPosition(pos.x, pos.y);

}

void DefaultMovement::SetReachedEnd(std::function<void(dae::SceneObject& parent)> callback)
{
	m_ReachedEndCallback = callback;
}

int2 DefaultMovement::GetBlockIndex() const
{
	return m_Index;
}
