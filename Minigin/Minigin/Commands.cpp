#include "Commands.h"

dae::ExecuteFunction::ExecuteFunction(const std::function<void()>& callback)
	: m_Callback(callback)
{
}

void dae::ExecuteFunction::Execute()
{
	m_Callback();
}
