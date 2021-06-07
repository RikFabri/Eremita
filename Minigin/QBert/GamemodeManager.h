#pragma once
#include <IWidget.h>
#include <ObserverInterface.h>

class GamemodeManager : public dae::IWidget, public dae::ObserverInterface
{
public:
	GamemodeManager();

	virtual void Render() override;
	virtual void OnNotify(const dae::BaseComponent*, const std::string& message) override;

private:
	enum class Gamemode
	{
		eNone,
		eAI,
		eCoop,
		eVersus,
		eGameOver
	};

	void LoadAI();
	void LoadCoop();
	void LoadVersus();
	void StopPlaying();
	void LoadGameOver();

	bool m_IsOpen;
	bool m_GameOver;

	Gamemode m_CurrentGamemode;

};

