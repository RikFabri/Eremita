#pragma once

namespace dae
{
	class BaseComponent
	{
	public:
		virtual ~BaseComponent() = default;
		
		virtual void Receive(int message) = 0;
		virtual void FixedUpdate() = 0;
		virtual void Update() = 0;

	protected:
		BaseComponent() = default;
		
	};
	
}
