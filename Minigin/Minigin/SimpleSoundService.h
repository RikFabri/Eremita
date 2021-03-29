#pragma once
#include <queue>
#include "ISoundService.h"
#include <mutex>
#include <condition_variable>

namespace dae {
	class SimpleSoundService : public ISoundService
	{
		using SoundRequest = std::pair<std::string, int>;
	public:
		SimpleSoundService();
		~SimpleSoundService();

		void PlaySound(const std::string& filename, int volume) override;
		void StopAllSounds() override;
		void SetMuted(bool) override;

		bool IsMuted() override;

	private:
		void ProcessSoundQueue();
		
		std::mutex m_Mutex;
		std::condition_variable m_ConditionVariable;
		
		std::atomic<bool> m_IsRunning;
		std::atomic<bool> m_IsMuted;
		std::queue<SoundRequest> m_SoundRequests;
	};
}

