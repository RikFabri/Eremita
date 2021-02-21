#pragma once
#include "Transform.h"

namespace dae
{
	class BaseComponent;
	
	class SceneObject final
	{
	public:
		void Update();
		void FixedUpdate();
		void Render();

		void Init();
		void AddComponent(BaseComponent* component, bool isGraphical = false);
		
		template<typename ComponentType>
		ComponentType* GetFirstComponentOfType();
		
		SceneObject(const std::vector<BaseComponent*>& components, const glm::vec3& position = {0,0,0});
		~SceneObject();
		SceneObject(const SceneObject& other) = delete;
		SceneObject(SceneObject&& other) = delete;
		SceneObject& operator=(const SceneObject& other) = delete;
		SceneObject& operator=(SceneObject&& other) = delete;

	private:		
		std::vector<BaseComponent*> m_Components; //To-Do: find better solution
		std::vector<BaseComponent*> m_GraphicalComponents;

	};

	// Templated functions
	template <typename ComponentType>
	ComponentType* SceneObject::GetFirstComponentOfType()
	{
		for (auto* pComponent : m_Components)
		{
			auto* castObject = dynamic_cast<ComponentType*>(pComponent);
			if (castObject != nullptr)
				return castObject;
		}
		
		for (auto* pComponent : m_GraphicalComponents)
		{
			auto* castObject = dynamic_cast<ComponentType*>(pComponent);
			if (castObject != nullptr)
				return castObject;
		}

		return nullptr;
	}
}
