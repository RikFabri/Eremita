#pragma once
#include "Singleton.h"
#include <sstream>

namespace dae
{
    class Logger : public Singleton<Logger>
    {
    public:
        Logger();
    	
        void Render();

        void Print(const std::string& message, const std::string& lineEnd = "\n");
        void ClearLog();
        void SaveLog(const std::string& path);
    private:
        bool m_IsOpen;
    	
        std::stringstream m_Log;
    	
    };
}
