#include "SoundLogService.h"
#include "Logger.h"

void dae::SoundLogService::PlaySound(const std::string& filename, int volume)
{
	Logger::GetInstance().Print(filename + " played at " + std::to_string(volume) + " volume");
}

void dae::SoundLogService::StopSound()
{
}

void dae::SoundLogService::StopAllSounds()
{
}

void dae::SoundLogService::SetMuted(bool)
{
}
