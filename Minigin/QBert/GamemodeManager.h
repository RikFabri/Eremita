#pragma once
#include <IWidget.h>

class GamemodeManager : public dae::IWidget
{
public:
	GamemodeManager();

	virtual void Render() override;

private:
	enum class Gamemode
	{
		eNone,
		eAI,
		eCoop,
		eVersus
	};

	void LoadAI();
	void LoadCoop();
	void LoadVersus();

	bool m_IsOpen;

	Gamemode m_CurrentGamemode;
};

