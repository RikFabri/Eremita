#include "MiniginPCH.h"
#include "FPSComponent.h"

#include "TextComponent.h"
#include "GameTime.h"

void dae::FPSComponent::Receive(int message)
{
	std::cout << "message received: " << message << std::endl;
}

void dae::FPSComponent::FixedUpdate()
{
	m_pTextComponentRef->SetText(std::to_string(int(GameTime::GetInstance().GetElapsedSec())) + " FPS");
}

void dae::FPSComponent::Update()
{
	
}

dae::FPSComponent::FPSComponent(TextComponent* pTextComponent)
	: m_pTextComponentRef(pTextComponent)
{
}
