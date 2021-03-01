#pragma once
#include "BaseComponent.h"
#include "ObserverInterface.h"

namespace dae
{
    class TextComponent;
	
    class ScoreDisplayComponent final : public BaseComponent, public ObserverInterface
    {
    public:
        void Init(SceneObject& parent) override;

        void OnNotify(const BaseComponent*, const std::string& message) override;
    private:

    	//No ownership
        TextComponent* m_pTextComponentRef;
    	
    };

}
