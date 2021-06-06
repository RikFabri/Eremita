#pragma once
#include <BaseComponent.h>
#include <memory>
#include <Transform.h>


namespace dae {
	class RenderComponent;
}


class DiskComponent : public dae::BaseComponent
{
public:
	virtual void Init(dae::SceneObject&) override;
	virtual void Update(dae::SceneObject& parent) override;
	void MoveToSpawn(dae::SceneObject* qbert, const glm::vec2& targetPos);
private:
	float m_Percentage;
	bool m_Moving;
	glm::vec2 m_BezierControlPoint;
	glm::vec2 m_StartPos;
	glm::vec2 m_TargetPos;

	glm::vec2 GetPointOnBezier(float percentage) const;

	// No ownership
	dae::RenderComponent* m_pRenderCompRef;
	dae::SceneObject* m_pQbertRef;
};

