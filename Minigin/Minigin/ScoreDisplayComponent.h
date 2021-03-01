#pragma once
#include "BaseComponent.h"
#include "ObserverInterface.h"

namespace dae
{
    class TextComponent;
	
    class ScoreDisplayComponent final : public BaseComponent, public ObserverInterface
    {
    public:
        ScoreDisplayComponent(int playerIndex);
        void Init(SceneObject& parent) override;

        void OnNotify(const BaseComponent*, const std::string& message) override;
    private:
        int m_PlayerIndex;

    	//No ownership
        TextComponent* m_pTextComponentRef;
    	
    };

}
