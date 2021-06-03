#pragma once
#include "ISoundService.h"
#include "NullSound.h"

namespace dae {
	class SoundServiceLocator
	{
	public:
		SoundServiceLocator() = delete;

		static ISoundService* GetSoundService();
		static void RegisterSoundService(ISoundService* pSoundService);

	private:
		static ISoundService* m_pSoundService;
		static NullSound m_NullSound;

	};

}