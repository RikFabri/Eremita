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

dae::TextComponent::TextComponent(const std::string& text, const std::shared_ptr<Font>& font) 
	: BaseComponent()
	, m_Text(text)
	, m_Font(font)
{ }

//To-Do: remove comments
//void dae::TextComponent::FixedUpdate()
//{
//	if (m_NeedsUpdate)
//	{
//		const SDL_Color color = { 255,255,255 }; // only white text is supported now
//		const auto surf = TTF_RenderText_Blended(m_Font->GetFont(), m_Text.c_str(), color);
//		if (surf == nullptr) 
//		{
//			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
//		}
//		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
//		if (texture == nullptr) 
//		{
//			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
//		}
//		SDL_FreeSurface(surf);
//		m_pTexture = std::make_shared<Texture2D>(texture);
//		m_NeedsUpdate = false;
//	}
//}
//
//void dae::TextComponent::Render() const
//{
//	if (m_pTexture != nullptr)
//	{
//		const auto pos = m_Transform.GetPosition();
//		Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);
//	}
//}
//
// This implementation uses the "dirty flag" pattern
void dae::TextComponent::SetText(const std::string& text)
{
	m_Text = text;

	const auto color = SDL_Color{ 255,255,0 };
	const auto surface = TTF_RenderText_Blended(m_Font->GetFont(), text.c_str(), color);

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
}

void dae::TextComponent::Receive(int message)
{
	std::cout << "received message: " << message << std::endl;
}
