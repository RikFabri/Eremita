#pragma once
#include "Singleton.h"
#include "IWidget.h"
#include <vector>

namespace dae
{
	class WidgetManager : public Singleton<WidgetManager>
	{
	public:
		void Render();

		void AddWidget(IWidget*);
		void RemoveWidget(IWidget*);
	private:
		WidgetManager();
		bool m_Rendering;

		std::vector<IWidget*> m_pWidgets;
		
		std::vector<IWidget*> m_pWidgetsToBeRemoved;
		std::vector<IWidget*> m_pWidgetsToBeAdded;
	};
}