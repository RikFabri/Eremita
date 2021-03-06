#pragma once
#include "ISoundService.h"

namespace dae {
	class SoundLogService : public ISoundService
	{
	public:
		void PlaySound(const std::string& filename, int volume) override;
		void StopAllSounds() override;
		void SetMuted(bool isMuted) override;
		bool IsMuted() override;
	};
}