#include <Eremita.h>

#include "Logger.h"
#include "WidgetRenderer.h"
#include "GamemodeManager.h"
using namespace dae;

void LoadGame();

int main(int, char* []) {
	const auto* gamemodeManager = new GamemodeManager();
	WidgetRenderer::GetInstance().AddWidget((IWidget*)gamemodeManager);

	Eremita engine;
	engine.Initialize();
	engine.Run();

	delete gamemodeManager;
	return 0;
}

void LoadGame()
{
	//To-Do: remove this
	//Logger::GetInstance().ClearLog(); // To clear the warning in case the teachers only plug in one controller.
	Logger::GetInstance().Print("\n-----------------------------------------------------------");
	Logger::GetInstance().Print("All D-Pad buttons add a certain amount of points, \nthe A button costs one life. Keyboard input isn't used yet");
	Logger::GetInstance().Print("controller-B plays a sound, X mutes/unmutes");
	Logger::GetInstance().Print("-----------------------------------------------------------");
}