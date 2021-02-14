#pragma once
namespace dae
{
	class BaseComponent;
	
	class SceneObject
	{
	public:
		virtual void Update();
		virtual void Render() const;

		void AddComponent(BaseComponent* component, bool isGraphical = false);
		
		SceneObject(const std::vector<BaseComponent*> components);
		virtual ~SceneObject();
		SceneObject(const SceneObject& other) = delete;
		SceneObject(SceneObject&& other) = delete;
		SceneObject& operator=(const SceneObject& other) = delete;
		SceneObject& operator=(SceneObject&& other) = delete;

	private:
		std::vector<BaseComponent*> m_Components;
		std::vector<BaseComponent*> m_GraphicalComponents;

	};
}
