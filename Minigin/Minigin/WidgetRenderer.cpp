#include "WidgetRenderer.h"
#include <iterator>

void dae::WidgetRenderer::Render()
{
	m_Rendering = true;

	for (auto widget : m_pWidgets)
		widget->Render();

	m_Rendering = false;

	if (!m_pWidgetsToBeRemoved.empty())
	{
		auto endIt = m_pWidgets.end();
		for (auto widget : m_pWidgetsToBeRemoved)
		{
			endIt = std::remove(m_pWidgets.begin(), endIt, widget);
		}
		m_pWidgets.erase(endIt, m_pWidgets.end());
		m_pWidgetsToBeRemoved.clear();
	}

	if (!m_pWidgetsToBeAdded.empty())
	{
		std::move(m_pWidgetsToBeAdded.begin(), m_pWidgets.end(), std::back_inserter(m_pWidgets));
		m_pWidgetsToBeAdded.clear();
	}
}

void dae::WidgetRenderer::AddWidget(IWidget* widget)
{
	if (!m_Rendering)
		m_pWidgets.push_back(widget);
	else
	{
		m_pWidgetsToBeAdded.push_back(widget);
	}
}

void dae::WidgetRenderer::RemoveWidget(IWidget* widget)
{
	if (!m_Rendering)
	{
		auto it = std::find(m_pWidgets.begin(), m_pWidgets.end(), widget);
		if (it != m_pWidgets.end())
			m_pWidgets.erase(it);
	}
	else
	{
		m_pWidgetsToBeRemoved.push_back(widget);
	}
}

dae::WidgetRenderer::WidgetRenderer()
	: m_Rendering(false)
{
}