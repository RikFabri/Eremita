#include "DefaultMovement.h"
#include "SoundServiceLocator.h"
#include "TileMapComponent.h"
#include "TimerComponent.h"
#include "SceneObject.h"
#include "Scene.h"
#include <utility>
#include <random>
#include "SDL.h"

DefaultMovement::DefaultMovement(bool moveUp, bool interactWithTiles, bool reverseTiles)
	: m_MoveUp(moveUp)
	, m_InteractWithTiles(interactWithTiles)
	, m_ReverseTiles(reverseTiles)
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

	auto* const service = dae::SoundServiceLocator::GetSoundService();
	service->PlaySound("../Data/BallJmp.wav", SDL_MIX_MAXVOLUME / 2);

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
	{
		m_Index = nextIndex;
		if (m_InteractWithTiles)
			m_pTileMapRef->HoppedOnTile(m_Index, m_ReverseTiles);
	}
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

void DefaultMovement::SetBlockIndex(const int2& id)
{
	m_Index = id;
}
