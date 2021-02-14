#pragma once
#include "Singleton.h"
#include <chrono>

namespace dae
{
    class GameTime : public Singleton<GameTime>
    {
    public:
        void SetDeltaTime(const std::chrono::duration<float>& dt);

        double GetElapsedSec() const;
    
        static const int MsPerFrame = 16; //16 for 60 fps, 33 for 30 fps
    private:
        std::chrono::duration<float> m_Dt{};
    };
}
