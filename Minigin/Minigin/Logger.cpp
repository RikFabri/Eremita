#include "Logger.h"

#include <fstream>
#include "imgui.h"


dae::Logger::Logger()
	: m_IsOpen(true)
{
}

void dae::Logger::Render()
{
	if (!m_IsOpen)
		return;
	
	ImGui::Begin("Logger", &m_IsOpen, ImGuiWindowFlags_AlwaysVerticalScrollbar);
	ImGui::Text(m_Log.str().c_str());
	
	ImGui::End();
}

void dae::Logger::Print(const std::string& message, const std::string& lineEnd)
{
	m_Log << message << lineEnd;

	m_IsOpen = true;
}

void dae::Logger::ClearLog()
{
	m_Log.str("");
}

void dae::Logger::SaveLog(const std::string& path)
{
	std::ofstream logFile{ path };
	if (logFile)
		logFile << m_Log.str();
	else
		Print("Failed to save logFile to: " + path);
}
