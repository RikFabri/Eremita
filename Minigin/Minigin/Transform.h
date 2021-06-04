#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/glm.hpp>
#pragma warning(pop)
#include "BaseComponent.h"

namespace dae
{
	class Transform final : public BaseComponent
	{
	public:
		Transform(const glm::vec3& position, const glm::vec2& scale = { 1, 1 });
		
		const glm::vec3& GetPosition() const { return m_Position; }
		const glm::vec2& GetScale() const { return m_Scale; }
		void SetPosition(float x, float y, float z);
		void SetScale(float x, float y);
	private:
		glm::vec3 m_Position;
		glm::vec2 m_Scale;
	};
}
