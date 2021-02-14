#pragma once
#include "BaseComponent.h"

#pragma warning(disable : 4201)
#include "glm/common.hpp"
#pragma warning(default : 4201)

struct SDL_Texture;

namespace dae
{
	class Texture2D;
	
	class RenderComponent : public BaseComponent
	{
	public:
		RenderComponent(const std::string& texturePath, const glm::fvec3& localPosition);

		void Receive(int message) override;
		void FixedUpdate() override;
		void Update() override;

		void SetTexture(SDL_Texture* texture);

		glm::fvec3 m_LocalPosition; //Vec-3 since we'll probably want to take z-values into account later on
	private:
		std::shared_ptr<Texture2D> m_pTexture;
	};
}
