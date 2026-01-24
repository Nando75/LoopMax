#ifndef HAL_HAS_RTOS
    #pragma once
    #include "core/hal/interfaces/IHal_Task.h"
    #include <cstdint>
    #include <Arduino.h> // per delay()
namespace LoopMax {
    namespace Core {
        namespace Hal {

            class ard_task : public IHal_Task {
            public:
                // in modalità NO RTOS, start() non prende parametri
                void start() override {
                    // Nessuna task, eseguito direttamente dal TaskManager
                }

                void stop() override {
                    // nulla da fare
                }

                void loop() override {
                    // Se vuoi fare polling dentro TaskManager
                }

                // Optional: helper delay
                void delayMs(uint32_t ms) {
                    delay(ms);
                }
            };

        }
    }
}
#endif