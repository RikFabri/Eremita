#pragma once
#include "BaseComponent.h"

namespace dae
{
	class TextComponent;
	
	class FPSComponent final : public BaseComponent
	{
	public:
		FPSComponent(TextComponent* pTextComponent);
		
		void Receive(int message) override;
		void FixedUpdate() override;
		void Update() override;

	private:
		//No ownership
		TextComponent* m_pTextComponentRef;
	};
}

