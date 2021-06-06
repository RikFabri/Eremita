#pragma once
#include "Transform.h"
#include "Logger.h"
#include <string>
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
		// Add components after update finished, they don't automatically get initialized 
		void AddComponentAfterUpdate(BaseComponent* component, bool isGraphical = false);
		void RemoveComponent(BaseComponent* component, bool isGraphical = false);
		
		template<typename ComponentType>
		ComponentType* GetFirstComponentOfType() const;

		const std::string& GetTag() const;
		Scene* GetScene() const;
		const Transform* GetTransform() const;
		void SetScene(Scene* pScene);
		void SetTag(const std::string& newTag);
		void SetPosition(const glm::vec3& pos);
		void SetPosition(float x, float y, float z = 0);

		bool RegisterAsObserver(ObserverInterface* pObserver);
		
		SceneObject(const std::vector<BaseComponent*>& components = {}, const glm::vec3& position = { 0,0,0 }, const glm::vec2& scale = { 1, 1 }, const std::string & tag = "defaultTag");
		~SceneObject();
		SceneObject(const SceneObject& other) = delete;
		SceneObject(SceneObject&& other) = delete;
		SceneObject& operator=(const SceneObject& other) = delete;
		SceneObject& operator=(SceneObject&& other) = delete;

	private:
		Transform* m_pTransform;
		std::string m_Tag;
		Scene* m_pScene;
		
		std::vector<BaseComponent*> m_Components; //To-Do: find better solution
		std::vector<BaseComponent*> m_GraphicalComponents;

		std::vector<std::pair<BaseComponent*, bool>> m_ComponentsToBeRemoved;
		std::vector<std::pair<BaseComponent*, bool>> m_ComponentsToBeAdded;
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

#if _DEBUG
		// You're getting a component that doesn't exist, this forces a breakpoint (in debug) for your convenience
		Logger::GetInstance().Print(std::string("Couldn't find component ") + typeid(ComponentType).name());
		Logger::GetInstance().SaveLog("Log.txt");
		__debugbreak();
#endif // _DEBUG

		return nullptr;
	}
}
