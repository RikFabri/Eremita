#pragma once
#include "BaseComponent.h"

#pragma warning(disable : 4201)
#include "glm/common.hpp"
#pragma warning(default : 4201)

struct SDL_Texture;

namespace dae
{
	class Texture2D;
	
	class RenderComponent final : public BaseComponent
	{
	public:
		RenderComponent(const std::string& texturePath, const glm::fvec3& localPosition);
		RenderComponent(const glm::fvec3& localPosition);

		void Update(SceneObject& object) override;

		void Receive(int message) override;
		void SetTexture(SDL_Texture* texture);

	private:
		glm::fvec3 m_LocalPosition; //Vec-3 since we'll probably want to take z-values into account later on
		std::shared_ptr<Texture2D> m_pTexture;
	};
}
