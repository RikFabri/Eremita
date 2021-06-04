#include "QBertBehaviourComponent.h"
#include "RenderComponent.h"
#include "TileMapComponent.h"
#include "HealthComponent.h"
#include "TimerComponent.h"
#include "Scene.h"

void QBertBehaviourComponent::Init(dae::SceneObject& parent)
{
	const auto renderComp = new dae::RenderComponent("QBert.png", {16, -16});
	renderComp->Init(parent);
	parent.AddComponent(renderComp, true);

	auto tileMapObj = parent.GetScene()->GetObjectsByTag("tileMap");

	m_pTimerCompRef = new dae::TimerComponent(0.5f);
	m_pTimerCompRef->Init(parent);
	parent.AddComponent(m_pTimerCompRef);

	if (tileMapObj.begin() == tileMapObj.end())
		throw std::exception("QBertBehaviour couldn't find a tileMap in the level, did you forget to add it?");

	m_pTileMapRef = tileMapObj[0]->GetFirstComponentOfType<TileMapComponent>();

	if(!m_pTileMapRef)
		throw std::exception("QBertBehaviour::init > tileMap didn't have a tileMapComponent");

	m_pTransformRef = parent.GetFirstComponentOfType<dae::Transform>();
	m_pHealthCompRef = parent.GetFirstComponentOfType<dae::HealthComponent>();

	const auto pos = m_pTileMapRef->IndexToTilePosition({ 0, 0 });
	m_pTransformRef->SetPosition(pos.x, pos.y, 0);
}

void QBertBehaviourComponent::Update(dae::SceneObject&)
{
}

void QBertBehaviourComponent::Move(int x, int y)
{
	if (!m_pTimerCompRef->TimerCompleted())
	{
		return;
	}

	m_pTimerCompRef->Reset();

	m_X += x;
	m_Y += y;

	const auto isValid = m_pTileMapRef->IsBlockIndexValid({ m_X, m_Y });
	
	if (isValid)
	{
		m_pTileMapRef->HoppedOnTile({ m_X, m_Y });
	}
	else
	{
		m_X = m_Y = 0;
		m_pHealthCompRef->Die();
	}

	const auto pos = m_pTileMapRef->IndexToTilePosition({ m_X, m_Y });
	m_pTransformRef->SetPosition(pos.x, pos.y, 0);
}
