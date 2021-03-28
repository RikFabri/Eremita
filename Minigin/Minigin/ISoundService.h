#pragma once
#include <string>

class ISoundService
{
public:
	virtual ~ISoundService() = default;
	
	virtual void PlaySound(const std::string& filename, int volume) = 0;
	virtual void StopSound() = 0;
	virtual void StopAllSounds() = 0;

	virtual void SetMuted(bool isMuted) = 0;
private:
	
};

