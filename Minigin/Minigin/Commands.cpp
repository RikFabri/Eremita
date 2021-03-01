#include "Commands.h"

dae::ExecuteFunction::ExecuteFunction(std::function<void()> callback)
	: m_Callback(std::move(callback))
{
}

void dae::ExecuteFunction::Execute()
{
	m_Callback();
}
