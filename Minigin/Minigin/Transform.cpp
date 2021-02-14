#include "MiniginPCH.h"
#include "Transform.h"

dae::Transform::Transform(const glm::vec3& position)
	: m_Position(position)
{
}

void dae::Transform::SetPosition(const float x, const float y, const float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}
