#include "CoilyMovementComponent.h"
#include "PosessedMovementComponent.h"
#include "SoundServiceLocator.h"
#include "TileMapComponent.h"
#include "TimerComponent.h"
#include "SceneObject.h"
#include "Scene.h"
#include "SDL.h"
#include <utility>

void CoilyMovementComponent::Init(dae::SceneObject& parent)
{
	const auto players = parent.GetScene()->GetObjectsByTag("player");
	m_QBertRef = players[std::rand() % players.size()];

	auto tileMapObj = parent.GetScene()->GetObjectsByTag("tileMap");
	m_pTileMapRef = tileMapObj[0]->GetFirstComponentOfType<TileMapComponent>();

	m_pTimerCompRef = parent.GetFirstComponentOfType<dae::TimerComponent>();

	m_pQBertMovementCompRef = m_QBertRef.lock()->GetFirstComponentOfType<PosessedMovementComponent>();
}

void CoilyMovementComponent::Update(dae::SceneObject& parent)
{
	if (!m_pTimerCompRef->TimerCompleted())
		return;

	m_pTimerCompRef->Reset();


	if (m_QBertRef.expired())
		return;

	const auto qbert = m_QBertRef.lock();
	auto qbertPos = m_pQBertMovementCompRef->GetPreviousPos();
	const auto pos = parent.GetTransform()->GetPosition();

	// Move towards qbert's previous position, or towards him if you're already there
	if (qbertPos == pos)
	{
		// If he used a disk, die instead
		if (m_pQBertMovementCompRef->IsOnDisk())
		{
			parent.GetScene()->Remove(&parent);
		}
		qbertPos = qbert->GetTransform()->GetPosition();
	}

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

	const auto isIndexValid = m_pTileMapRef->IsBlockIndexValid(nextIndex);
	if (isIndexValid)
	{
		//Play sound
		auto* const service = dae::SoundServiceLocator::GetSoundService();
		service->PlaySound("../Data/CoilyJmp.wav", SDL_MIX_MAXVOLUME / 2);

		m_Index = nextIndex;
		const auto newPos = m_pTileMapRef->IndexToTilePosition(nextIndex);
		parent.SetPosition(newPos.x, newPos.y);
	}
}

void CoilyMovementComponent::SetIndex(const int2& idx)
{
	m_Index = idx;
}
