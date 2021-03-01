#include "HealthDisplayComponent.h"
#include "HealthComponent.h"
#include "Scene.h"
#include "SceneObject.h"
#include "TextComponent.h"

dae::HealthDisplayComponent::HealthDisplayComponent(int playerId)
	: m_PlayerId(playerId)
{
}

void dae::HealthDisplayComponent::OnNotify(const BaseComponent* pSenderComponent, const std::string& message)
{
	if (message != "UpdateHealth")
		return;

	const auto* healthComponent = static_cast<const HealthComponent*>(pSenderComponent);

	m_pTextComponentRef->SetText(std::to_string(healthComponent->GetLives()) + " lives");
}

void dae::HealthDisplayComponent::Init(SceneObject& parent)
{
	m_pTextComponentRef = parent.GetFirstComponentOfType<TextComponent>();

	if (!m_pTextComponentRef)
		throw std::exception("HealthDisplayComponent requires a sibling textComponent to work");

	const auto sceneObjects = parent.GetScene()->GetObjectsByTag("player");

	if (sceneObjects.size() <= size_t(m_PlayerId))
	{
		const auto message = "HealthDisplayComponent: Couldn't find a " + std::to_string(m_PlayerId) + "th object with tag player";
		throw std::exception(message.c_str());
	}

	const auto wasSuccess = sceneObjects[m_PlayerId]->RegisterAsObserver(this);

	if (!wasSuccess)
		throw std::exception("HealthDisplayComponent couldn't register as observer, does subject have a subjectComponent?");
}
