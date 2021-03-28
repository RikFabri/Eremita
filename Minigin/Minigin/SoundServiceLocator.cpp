#include "SoundServiceLocator.h"

NullSound dae::SoundServiceLocator::m_NullSound;
ISoundService* dae::SoundServiceLocator::m_pSoundService = &m_NullSound;

ISoundService* dae::SoundServiceLocator::GetSoundService()
{
	return m_pSoundService;
}

void dae::SoundServiceLocator::RegisterSoundService(ISoundService* pSoundService)
{
	if(m_pSoundService != &m_NullSound)
		delete m_pSoundService;
	
	m_pSoundService = pSoundService ? pSoundService : &m_NullSound;
}