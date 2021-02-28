#pragma once
#include "Transform.h"
#include <vector>

namespace dae
{
	class Scene;
	class BaseComponent;
	class ObserverInterface;
	
	class SceneObject final
	{
	public:
		void Update();
		void FixedUpdate();
		void Render();

		void Init();
		void AddComponent(BaseComponent* component, bool isGraphical = false);
		
		template<typename ComponentType>
		ComponentType* GetFirstComponentOfType() const;

		const std::string& GetTag() const;
		Scene* GetScene() const;
		void SetScene(Scene* pScene);
		void SetTag(const std::string& newTag);

		bool RegisterAsObserver(ObserverInterface* pObserver);
		
		SceneObject(const std::vector<BaseComponent*>& components = {}, const glm::vec3& position = { 0,0,0 }, const std::string& tag = "defaultTag");
		~SceneObject();
		SceneObject(const SceneObject& other) = delete;
		SceneObject(SceneObject&& other) = delete;
		SceneObject& operator=(const SceneObject& other) = delete;
		SceneObject& operator=(SceneObject&& other) = delete;

	private:
		std::string m_Tag;
		Scene* m_pScene;
		
		std::vector<BaseComponent*> m_Components; //To-Do: find better solution
		std::vector<BaseComponent*> m_GraphicalComponents;
	};

	// Templated functions
	template <typename ComponentType>
	ComponentType* SceneObject::GetFirstComponentOfType() const
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
