#include "Logger.h"

#include <fstream>
#include "imgui.h"


dae::Logger::Logger()
	: m_IsOpen(false)
{
#if _DEBUG
	m_IsOpen = true;
#endif // _DEBUG

}

void dae::Logger::Render()
{
	if (!m_IsOpen)
		return;
	
	ImGui::Begin("Logger", &m_IsOpen, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_MenuBar);

	ImGui::BeginMenuBar();
	if (ImGui::Button("Save log"))
		SaveLog("Log.txt");

	if (ImGui::Button("Clear log"))
		ClearLog();

	ImGui::EndMenuBar();
	ImGui::Text(m_Log.str().c_str());
	
	ImGui::End();
}

void dae::Logger::Print(const std::string& message, const std::string& lineEnd)
{
	m_Log << message << lineEnd;

#if _DEBUG
	m_IsOpen = true;
#endif
}

void dae::Logger::ClearLog()
{
	m_Log.str("");
}

void dae::Logger::SaveLog(const std::string& path)
{
	std::ofstream logFile{ path };
	if (logFile)
	{
		logFile << m_Log.str();
		Print("Saved log to: " + path);
	}
	else
		Print("Failed to save logFile to: " + path);
}
