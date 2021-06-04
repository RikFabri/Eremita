#pragma once
#include "BaseComponent.h"

namespace dae
{
	class TimerComponent : public BaseComponent
	{
	public:
		TimerComponent(float waitTime);

		virtual void Update(SceneObject&) override;

		void Reset();
		bool TimerCompleted() const;
	private:
		float m_WaitTime;
		float m_PassedTime;

		bool m_TimerFinished;
	};
}
