#pragma once 
#include <cstdint>
namespace LoopMax::Services {

    class ITimer {
    public:
        virtual uint64_t millis() = 0; // restituisce millisecondi
        virtual uint64_t micros() = 0; // opzionale se ti serve più precisione
        virtual int64_t Unix() = 0;
        virtual std::string Timezone() = 0;
        virtual int64_t Tz_Offset() = 0;

        virtual const std::string ntpServer() const = 0;
        virtual const bool isSyncronized() const = 0;

        virtual ~ITimer() {}
    };

}
