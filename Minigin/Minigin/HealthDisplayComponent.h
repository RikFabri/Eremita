#pragma once
#include "BaseComponent.h"
#include "ObserverInterface.h"

namespace dae
{
	class TextComponent;
	
	class HealthDisplayComponent final : public BaseComponent, public ObserverInterface
	{
	public:
		HealthDisplayComponent(int playerId = 0);
		void OnNotify(const BaseComponent*, const std::string& message) override;

		void Init(SceneObject& parent) override;

	private:
		int m_PlayerId;

		//No ownership
		TextComponent* m_pTextComponentRef;
	};
}

