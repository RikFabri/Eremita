#pragma once
#include "BaseComponent.h"
#include "SDL.h"

namespace dae
{
	class Font;
	class RenderComponent;
	
	class TextComponent final : public BaseComponent
	{
	public:
		void Init(SceneObject& object) override;
		void SetText(const std::string& text);

		void Receive(int message) override;

		explicit TextComponent(const std::string& text, const std::shared_ptr<Font>& font, const SDL_Color& color = {255,255,255});
		~TextComponent() = default;
		TextComponent(const TextComponent& other) = default;
		TextComponent(TextComponent&& other) = default;
		TextComponent& operator=(const TextComponent& other) = default;
		TextComponent& operator=(TextComponent&& other) = default;

	private:
		SDL_Color m_Color;
		std::string m_Text;
		std::shared_ptr<Font> m_Font;

		//Not owned
		RenderComponent* m_pRenderComponentRef = nullptr;
	};
}
