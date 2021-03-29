#include "SoundLogService.h"
#include "Logger.h"

void dae::SoundLogService::PlaySound(const std::string& filename, int volume)
{
	Logger::GetInstance().Print(filename + " played at " + std::to_string(volume) + " volume");
}

void dae::SoundLogService::StopAllSounds()
{
	Logger::GetInstance().Print("Stopped all sounds");
}

void dae::SoundLogService::SetMuted(bool isMuted)
{
	Logger::GetInstance().Print(isMuted ? "Muted sound" : "Unmuted sound");
}

bool dae::SoundLogService::IsMuted()
{
	return false;
}
