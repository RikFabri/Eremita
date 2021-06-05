#pragma once
#include "Singleton.h"
#include "IWidget.h"
#include <vector>

namespace dae
{
	class WidgetRenderer : public Singleton<WidgetRenderer>
	{
	public:

		void Render();

		void AddWidget(IWidget*);
		void RemoveWidget(IWidget*);
	private:
		friend class Singleton;
		WidgetRenderer();

		bool m_Rendering;

		std::vector<IWidget*> m_pWidgets;
		
		std::vector<IWidget*> m_pWidgetsToBeRemoved;
		std::vector<IWidget*> m_pWidgetsToBeAdded;
	};
}