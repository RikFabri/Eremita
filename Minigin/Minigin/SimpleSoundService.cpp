#include "SimpleSoundService.h"

#include "audio.h"

void SimpleSoundService::PlaySound(const std::string& filename, int volume)
{
	playSound(filename.c_str(), volume);
}
