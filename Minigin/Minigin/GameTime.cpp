#include "MiniginPCH.h"
#include "GameTime.h"

void dae::GameTime::SetDeltaTime(const std::chrono::duration<float>& dt)
{
	m_Dt = dt;
}

double dae::GameTime::GetElapsedSec() const
{
	return 1 / m_Dt.count();
}
