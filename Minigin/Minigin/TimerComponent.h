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
		void SetWaitTime(float time);
		bool TimerCompleted() const;
		float GetWaitTime() const;
	private:
		float m_WaitTime;
		float m_PassedTime;

		bool m_TimerFinished;
	};
}
