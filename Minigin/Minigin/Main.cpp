#include "MiniginPCH.h"

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#include <vld.h>
#endif

#include "SDL.h"
#include "Eremita.h"

int main(int, char*[]) {
	dae::Eremita engine;
	engine.Run();
    return 0;
}