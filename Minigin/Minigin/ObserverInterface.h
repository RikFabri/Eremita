#pragma once
#include <string>

namespace dae
{
	class BaseComponent;
	
	class ObserverInterface
	{
	public:
		virtual ~ObserverInterface() = default;
		virtual void OnNotify(const BaseComponent*, const std::string& message) = 0;
	};
}
