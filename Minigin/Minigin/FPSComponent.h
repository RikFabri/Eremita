#pragma once
#include "BaseComponent.h"

namespace dae
{
	class TextComponent;
	
	class FPSComponent final : public BaseComponent
	{
	public:
		FPSComponent();
		
		void Init(SceneObject& object) override;
		
		void Update(SceneObject& object) override;

	private:
		const float m_RefreshRate;
		float m_ElapsedTime;
		
		//No ownership
		TextComponent* m_pTextComponentRef = nullptr;
	};
}

