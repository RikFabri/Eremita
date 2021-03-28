#pragma once
#include "ISoundService.h"

class SimpleSoundService : public ISoundService
{
public:
	void PlaySound(const std::string& filename, int volume) override;
	void StopSound() override{};
	void StopAllSounds() override{};
	void SetMuted(bool) override{}

private:
	
};

