#include "MiniginPCH.h"
#include "Transform.h"

dae::Transform::Transform(const glm::vec3& position, const glm::vec2& scale)
	: m_Position(position)
	, m_Scale(scale)
{
}

void dae::Transform::SetPosition(const float x, const float y, const float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}

void dae::Transform::SetScale(float x, float y)
{
	m_Scale.x = x;
	m_Scale.y = y;
}
