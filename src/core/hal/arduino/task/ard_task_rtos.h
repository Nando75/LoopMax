


#pragma once
#include "core/hal/interfaces/IHal_Task.h"

#ifdef HAL_HAS_RTOS
    #include "freertos/FreeRTOS.h"
    #include "freertos/task.h"
#endif

namespace LoopMax {
    namespace Core {
        namespace Hal {



            class ard_task_rtos : public IHal_Task {
            private:
            #ifdef HAL_HAS_RTOS
                TaskHandle_t handle = nullptr;
            #endif

            #ifdef HAL_HAS_RTOS
            void start(void (*entry)(void*), void* arg, const char* name,
                    uint32_t stack, uint32_t priority, int core) override
            {
                xTaskCreatePinnedToCore(entry, name, stack, arg, priority, &handle, core);
            }
            #endif


                static void task_entry(void* arg) {
                    static_cast<ard_task_rtos*>(arg)->run();
                }

                void run() {
                    for (;;) {
                        // monitor / bus / poll


                        #ifdef HAL_HAS_RTOS
                                    vTaskDelay(pdMS_TO_TICKS(10));
                        #endif
                    }
                }

            public:

            #ifdef HAL_HAS_RTOS
                TaskHandle_t getHandle() const override { return handle; }
                void delayMs(uint32_t ms) {  if(handle) vTaskDelay(pdMS_TO_TICKS(ms)); }
            #endif


                void stop() override {
                    #ifdef HAL_HAS_RTOS
                            if (handle) {
                                vTaskDelete(handle);
                                handle = nullptr;
                            }
                    #endif
                }

                void loop() override {} // Non usato

            };




        }

    }
}

