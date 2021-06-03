#include "SimpleSoundService.h"

#include <thread>
#include "audio.h"
#include "Logger.h"

dae::SimpleSoundService::SimpleSoundService()
	: m_IsRunning(true)
	, m_AudioThread(&SimpleSoundService::ProcessSoundQueue, this)
{
}

dae::SimpleSoundService::~SimpleSoundService()
{
	m_IsRunning.store(false);
	m_ConditionVariable.notify_one();
	m_AudioThread.join();
}

void dae::SimpleSoundService::PlaySound(const std::string& filename, int volume)
{
	std::unique_lock<std::mutex> lock{ m_Mutex };
	m_SoundRequests.push(std::make_pair(filename, volume));
	m_ConditionVariable.notify_one();
}

void dae::SimpleSoundService::StopAllSounds()
{
	std::unique_lock<std::mutex> lock{ m_Mutex };
	std::queue<SoundRequest> empty{};
	m_SoundRequests.swap(empty);
}

void dae::SimpleSoundService::SetMuted(bool isMuted)
{
	m_IsMuted.store(isMuted);
}

bool dae::SimpleSoundService::IsMuted()
{
	return m_IsMuted;
}

void dae::SimpleSoundService::ProcessSoundQueue()
{
	while(m_IsRunning.load())
	{
		std::unique_lock<std::mutex> lock{ m_Mutex };
		if(!m_SoundRequests.empty())
		{
			SoundRequest request = m_SoundRequests.front();
			m_SoundRequests.pop();

			if(!m_IsMuted.load())
				playSound(request.first.c_str(), request.second);
		}

		if (m_SoundRequests.empty())
			m_ConditionVariable.wait(lock);

	}
}
