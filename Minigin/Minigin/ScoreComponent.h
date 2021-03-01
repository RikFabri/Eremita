#pragma once
#include "BaseComponent.h"

namespace dae
{
	class SubjectComponent;
	
	class ScoreComponent final : public BaseComponent
	{
	public:
		ScoreComponent();
		void Init(SceneObject& parent) override;

		void IncreaseScore(int amount);
		int GetScore() const;
	private:
		int m_Score;

		//No ownership
		SubjectComponent* m_SubjectComponentRef;
	};
}