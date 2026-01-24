// ITASK.H
#pragma once

#include <cstdint>

#ifdef HAL_HAS_RTOS
    #include <freertos/FreeRTOS.h>
    #include <freertos/task.h>
#endif

namespace LoopMax {
    namespace Core {
        namespace Hal {

                    class IHal_Task {
                    public:
                        virtual void stop() = 0;
                        virtual void loop() = 0;
                    #ifdef HAL_HAS_RTOS
                        virtual void start(void (*entry)(void*), void* arg, const char* name,
                                        uint32_t stack, uint32_t priority, int core) = 0;
                        virtual TaskHandle_t getHandle() const = 0;
                        virtual void delayMs(uint32_t ms) = 0;
                    #else
                        virtual void start() = 0;
                        virtual void delayMs(uint32_t ms) = 0;
                    #endif

                        virtual ~IHal_Task() {}
                    };

        }
    }
}