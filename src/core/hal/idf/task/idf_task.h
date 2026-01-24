#ifndef HAL_HAS_RTOS
    #pragma once
    #include "core/hal/interfaces/ITask_hal.h"
    #include <cstdint>
    #include <esp32-hal.h>
namespace LoopMax {
    namespace Core {

        namespace Hal {

            class idf_task : public ITask_hal {
            public:

                // Start senza RTOS → nessun argomento
                void start() override {
                    // Nessuna task da creare, il TaskManager gestirà le chiamate a loop()
                }

                void stop() override {
                    // Nessuna task da fermare
                }

                void loop() override {
                    // eventualmente implementa un loop se serve
                }

                void delayMs(uint32_t ms) override {
                    ::delay(ms);  // Arduino/IDF delay
                }
            };


        }
    }

}

#endif    