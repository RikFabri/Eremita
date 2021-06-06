#include "WrongwayMovementComponent.h"
#include "SoundServiceLocator.h"
#include "TileMapComponent.h"
#include "TimerComponent.h"
#include "SceneObject.h"
#include "Logger.h"
#include "Scene.h"
#include "SDL.h"

void WrongwayMovementComponent::Init(dae::SceneObject& parent)
{
	auto tileMapObj = parent.GetScene()->GetObjectsByTag("tileMap");
	m_pTileMapRef = tileMapObj[0]->GetFirstComponentOfType<TileMapComponent>();

	m_pTimerCompRef = parent.GetFirstComponentOfType<dae::TimerComponent>();

	m_Index = { 7, 7 };
	const auto pos = m_pTileMapRef->IndexToTilePosition(m_Index);
	parent.SetPosition(pos.x, pos.y);
}

void WrongwayMovementComponent::Update(dae::SceneObject& parent)
{
	if (!m_pTimerCompRef->TimerCompleted())
		return;

	m_pTimerCompRef->Reset();

	// Semi-random movement
	auto movement = std::rand() & 0b01 ? int2{ -1, 0 } : int2{ -1, -1 };

	const auto nextIndex = int2{ m_Index.first + movement.first, m_Index.second + movement.second };
	const auto isNextLocationValid = m_pTileMapRef->IsBlockIndexValid({ nextIndex.first - 1, nextIndex.second - 1 });

	dae::SoundServiceLocator::GetSoundService()->PlaySound("../Data/UggJmp.wav", SDL_MIX_MAXVOLUME / 2);

	if (isNextLocationValid)
	{
		m_Index = nextIndex;
	}
	else
	{
		parent.GetScene()->Remove(&parent);
	}

	const auto pos = m_pTileMapRef->IndexToTilePosition(m_Index);
	parent.SetPosition(pos.x, pos.y);

}
