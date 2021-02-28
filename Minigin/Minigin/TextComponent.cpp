#include "MiniginPCH.h"
#include <SDL.h>
#include <SDL_ttf.h>

#include "TextComponent.h"
#include "Renderer.h"
#include "Font.h"
#include "RenderComponent.h"
#include "Texture2D.h"
#include "GameTime.h"
#include "SceneObject.h"

dae::TextComponent::TextComponent(const std::string& text, const std::shared_ptr<Font>& font, const SDL_Color& color)
	: BaseComponent()
	, m_Text(text)
	, m_Font(font)
	, m_Color(color)
{
}

void dae::TextComponent::SetText(const std::string& text)
{
	m_Text = text;
	
	const auto surface = TTF_RenderText_Blended(m_Font->GetFont(), text.c_str(), m_Color);

	if (surface == nullptr)
		throw std::runtime_error(std::string("Rendering text failed: ") + SDL_GetError());

	auto* pTexture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surface);

	if (pTexture == nullptr)
		throw std::runtime_error(std::string("Creating texture from surface failed: ") + SDL_GetError());

	SDL_FreeSurface(surface);
	m_pRenderComponentRef->SetTexture(pTexture);
	
}

void dae::TextComponent::Init(SceneObject& object)
{
	m_pRenderComponentRef = object.GetFirstComponentOfType<RenderComponent>();
	if (!m_pRenderComponentRef)
		throw std::exception("TextComponents require a renderComponent to work, please add one to the corresponding SceneObject");

	SetText(m_Text);
}

void dae::TextComponent::Receive(int message)
{
	std::cout << "received message: " << message << std::endl;
}
