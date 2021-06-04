#pragma once
#include <BaseComponent.h>
#include <SceneObject.h>
#include <memory>

class TileComponent;

class TileMapComponent : public dae::BaseComponent
{
public:
	virtual void Init(dae::SceneObject&) override;
	virtual void Update(dae::SceneObject&) override;


private:
	std::vector<std::vector<std::unique_ptr<TileComponent>>> m_Tiles;

	glm::vec2 CartesianToAxial(const glm::vec2& cartesianCoords) const;
};

