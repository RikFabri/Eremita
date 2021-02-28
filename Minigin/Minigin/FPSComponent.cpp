#include "MiniginPCH.h"
#include "FPSComponent.h"

#include "TextComponent.h"
#include "GameTime.h"
#include "SceneObject.h"

dae::FPSComponent::FPSComponent()
	: m_RefreshRate(0.7f)
{
}

void dae::FPSComponent::Init(SceneObject& object)
{
	m_pTextComponentRef = object.GetFirstComponentOfType<TextComponent>();
	if (!m_pTextComponentRef)
		throw std::exception("FPSComponents require a TextComponent, please add one to the corresponding gameObject");
}

void dae::FPSComponent::Update(SceneObject&)
{
	m_ElapsedTime += (float)GameTime::GetInstance().GetDeltaTime();

	if (m_ElapsedTime >= m_RefreshRate)
	{
		m_pTextComponentRef->SetText(std::to_string(int(GameTime::GetInstance().GetFPS())) + " FPS");
		m_ElapsedTime = 0;
	}
}
