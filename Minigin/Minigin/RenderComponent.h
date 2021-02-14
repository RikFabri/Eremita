#pragma once
#include "BaseComponent.h"

#pragma warning(disable : 4201)
#include "glm/common.hpp"
#pragma warning(default : 4201)

struct SDL_Texture;

namespace dae
{
	class Texture2D;
	class Transform;
	
	class RenderComponent final : public BaseComponent
	{
	public:
		RenderComponent(const std::string& texturePath);
		RenderComponent();

		void Update(SceneObject& object) override;

		void Init(SceneObject& object) override;
		void Receive(int message) override;
		void SetTexture(SDL_Texture* texture);

	private:
		std::shared_ptr<Texture2D> m_pTexture;

		//No ownership
		Transform* m_pTransformRef = nullptr;
	};
}
