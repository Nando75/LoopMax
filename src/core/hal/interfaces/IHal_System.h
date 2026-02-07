//#include "BaseConfig.h"
#pragma once
#include "common/types.h"
#include <cstddef>  
#include <cstdint>  
namespace LoopMax {
    namespace Core {
        namespace Hal {

                class IHal_System {
                public:
                    virtual uint64_t uptime() = 0;
                    virtual const char* ChipModel() = 0;
                    virtual const uint8_t ChipCores() = 0;
                    virtual const uint8_t ChipRevision() = 0;
                    virtual const uint32_t FlashSize() = 0;
                    virtual const uint32_t FlashSpeed() = 0;
                    virtual const uint32_t FreeHeap() = 0;
                    virtual const uint32_t HeapSize() = 0;
                    virtual const uint32_t CpuFreq() = 0;
                    
                    //virtual Types::ServiceState status()=0;
                    // getter e setter per la modalità
                    virtual Types::SystemMode mode() = 0;
                    virtual void setMode(Types::SystemMode mode) = 0;
                    
                    virtual void restart() = 0;

                    virtual const char* buildDate() = 0;
                    virtual const char* buildTime() = 0;
                    
                    virtual ~IHal_System() {}
                };

        }
    }
}