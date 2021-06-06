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
	tileMapObj[0]->RegisterAsObserver(this);

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
	m_pMovementCompRef->UpdateVisualLocation();
}

void QBertBehaviourComponent::OnNotify(const BaseComponent*, const std::string& message)
{
	if (message == "level Changed")
	{
		m_pMovementCompRef->SetBlockIndex({ 0, 0 });
		m_pMovementCompRef->UpdateVisualLocation();
	}
}
