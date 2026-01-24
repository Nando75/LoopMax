#pragma once 
#include <cstdint>
namespace LoopMax::Core::Hal {
    class IHal_Timer {
    public:
        virtual uint64_t millis() = 0; // restituisce millisecondi
        virtual uint64_t micros() = 0; // opzionale se ti serve più precisione



        
        // --- getter ---
        virtual int64_t Unix() = 0;
        virtual std::string Timezone() = 0;
        virtual int64_t Tz_Offset() = 0;

        // --- setter ---
      
        virtual void setUnix(int64_t unix) = 0;
        virtual void setTimezone(const std::string& tz) = 0;
        virtual void setTzOffset(int64_t offset) = 0;

        virtual void checkInternetTime() = 0;
        virtual const std::string ntpServer() const = 0;
        virtual const bool isSyncronized() const = 0;


        
        //virtual void initContext(SystemContext* ctx) = 0;
        virtual ~IHal_Timer() {}
    };

}
