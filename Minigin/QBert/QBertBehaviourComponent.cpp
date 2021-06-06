#include "QBertBehaviourComponent.h"
#include "PosessedMovementComponent.h"
#include "SoundServiceLocator.h"
#include "TileMapComponent.h"
#include "RenderComponent.h"
#include "HealthComponent.h"
#include "TimerComponent.h"
#include "GameTime.h"
#include "Scene.h"
#include "SDL.h"

void QBertBehaviourComponent::Init(dae::SceneObject& parent)
{
	m_Alive = true;
	m_pParent = &parent;

	auto tileMapObj = parent.GetScene()->GetObjectsByTag("tileMap");
	m_pTileMapRef = tileMapObj[0]->GetFirstComponentOfType<TileMapComponent>();
	tileMapObj[0]->RegisterAsObserver(this);

	m_pTransformRef = parent.GetFirstComponentOfType<dae::Transform>();
	m_pHealthCompRef = parent.GetFirstComponentOfType<dae::HealthComponent>();
	m_pMovementCompRef = parent.GetFirstComponentOfType<PosessedMovementComponent>();

	m_pMovementCompRef->SetJumpedOffCallback(std::bind(&QBertBehaviourComponent::Damage, this));
	m_pMovementCompRef->UpdateVisualLocation();

	m_pTimerCompRef = parent.GetFirstComponentOfType<dae::TimerComponent>();
}

void QBertBehaviourComponent::Update(dae::SceneObject& parent)
{
	if (m_Alive)
		return;

	const auto dt = dae::GameTime::GetInstance().GetDeltaTime();
	const auto pos = parent.GetTransform()->GetPosition();

	parent.SetPosition({pos.x, pos.y + dt * 400, pos.z});

	if (m_pTimerCompRef->TimerCompleted())
	{
		m_Alive = true;
		m_pMovementCompRef->SetEnabled(true);
		m_pTimerCompRef->Reset();
		m_pMovementCompRef->SetBlockIndex({ 0, 0 });
		m_pMovementCompRef->UpdateVisualLocation();
		m_pTimerCompRef->SetWaitTime(m_PrevWaitTime);
	}
}

void QBertBehaviourComponent::Damage()
{
	m_pHealthCompRef->Die();

	m_Alive = false;
	m_pMovementCompRef->SetEnabled(false);
	dae::SoundServiceLocator::GetSoundService()->PlaySound("../Data/Death.wav", SDL_MIX_MAXVOLUME / 2);
	m_PrevWaitTime = m_pTimerCompRef->GetWaitTime();
	m_pTimerCompRef->SetWaitTime(1.6f);
	m_pTimerCompRef->Reset();
}

void QBertBehaviourComponent::OnNotify(const BaseComponent*, const std::string& message)
{
	if (message == "level Changed")
	{
		m_pMovementCompRef->SetBlockIndex({ 0, 0 });
		m_pMovementCompRef->UpdateVisualLocation();
	}
}
