#include "MiniginPCH.h"
#include "GameTime.h"

void dae::GameTime::SetDeltaTime(const std::chrono::duration<float>& dt)
{
	m_Dt = dt;
}

double dae::GameTime::GetFPS() const
{
	return 1 / m_Dt.count();
}

double dae::GameTime::GetDeltaTime() const
{
	return m_Dt.count();
}
