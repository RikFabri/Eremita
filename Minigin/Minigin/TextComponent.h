#pragma once
#include "BaseComponent.h"

namespace dae
{
	class Font;
	class RenderComponent;
	
	class TextComponent final : public BaseComponent
	{
	public:
		void SetText(const std::string& text);

		void Receive(int message) override;
		void FixedUpdate() override;
		void Update() override;

		explicit TextComponent(const std::string& text, const std::shared_ptr<Font>& font, RenderComponent* pRenderComponent);
		~TextComponent() = default;
		TextComponent(const TextComponent& other) = default;
		TextComponent(TextComponent&& other) = default;
		TextComponent& operator=(const TextComponent& other) = default;
		TextComponent& operator=(TextComponent&& other) = default;

	private:
		std::string m_Text;
		std::shared_ptr<Font> m_Font;

		//Not owned
		RenderComponent* m_pRenderComponentRef;
	};
}
