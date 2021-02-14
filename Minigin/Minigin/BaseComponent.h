#pragma once

namespace dae
{
	class SceneObject;
	
	class BaseComponent
	{
	public:
		virtual ~BaseComponent() = default;
		
		virtual void Init(SceneObject&){}
		
		virtual void Update(SceneObject&){}
		virtual void FixedUpdate(SceneObject&){}

		virtual void Receive(int){}

	protected:
		BaseComponent() = default;
		
	};
	
}
