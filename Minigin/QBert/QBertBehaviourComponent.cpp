#include "QBertBehaviourComponent.h"
#include "PosessedMovementComponent.h"
#include "TileMapComponent.h"
#include "RenderComponent.h"
#include "HealthComponent.h"
#include "TimerComponent.h"
#include "Scene.h"

void QBertBehaviourComponent::Init(dae::SceneObject& parent)
{
	m_pParent = &parent;

	auto tileMapObj = parent.GetScene()->GetObjectsByTag("tileMap");
	m_pTileMapRef = tileMapObj[0]->GetFirstComponentOfType<TileMapComponent>();

	m_pTransformRef = parent.GetFirstComponentOfType<dae::Transform>();
	m_pHealthCompRef = parent.GetFirstComponentOfType<dae::HealthComponent>();
	m_pMovementCompRef = parent.GetFirstComponentOfType<PosessedMovementComponent>();

	m_pMovementCompRef->SetJumpedOffCallback(std::bind(&QBertBehaviourComponent::Damage, this));
	m_pMovementCompRef->UpdateVisualLocation();
}

void QBertBehaviourComponent::Update(dae::SceneObject&)
{
}

void QBertBehaviourComponent::Damage()
{
	m_pHealthCompRef->Die();

	m_pMovementCompRef->SetBlockIndex({ 0, 0 });
	const auto pos = m_pTileMapRef->IndexToTilePosition({ 0, 0 });
	m_pTransformRef->SetPosition(pos.x, pos.y, 0);
}