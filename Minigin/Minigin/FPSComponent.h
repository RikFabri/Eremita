#pragma once
#include "BaseComponent.h"

namespace dae
{
	class TextComponent;
	
	class FPSComponent final : public BaseComponent
	{
	public:
		void Init(SceneObject& object) override;
		
		void Update(SceneObject& object) override;

	private:
		//No ownership
		TextComponent* m_pTextComponentRef = nullptr;
	};
}

