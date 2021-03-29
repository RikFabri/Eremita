#include "NullSound.h"

void NullSound::PlaySound(const std::string&, int)
{
}

void NullSound::StopAllSounds()
{
}

void NullSound::SetMuted(bool)
{
}

bool NullSound::IsMuted()
{
	return false;
}
