

#pragma once
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "core/hal/interfaces/ITask_hal.h"

namespace LoopMax {
    namespace Core {

        namespace Hal {
                                

                    class idf_task_rtos : public ITask_hal {
                    private:
                        TaskHandle_t handle = nullptr;

                    public:
                        // implementazione di ITask
                        void start(void (*entry)(void*), void* arg, const char* name,
                                uint32_t stack, uint32_t priority, int core) override {
                            BaseType_t res =
                                xTaskCreatePinnedToCore(
                                    entry,
                                    name,
                                    stack,
                                    arg,
                                    priority,
                                    &handle,
                    #if HAL_CORES == 2
                                    (core >= 0 ? core : tskNO_AFFINITY)
                    #else
                                    tskNO_AFFINITY
                    #endif
                                );

                            if(res != pdPASS) {
                                handle = nullptr;
                                // eventualmente log
                            }
                        }

                        void stop() override {
                            if (handle) {
                                vTaskDelete(handle);
                                handle = nullptr;
                            }
                        }

                        void delayMs(uint32_t ms) override {
                            if(handle) vTaskDelay(pdMS_TO_TICKS(ms));
                        }

                        TaskHandle_t getHandle() const override { return handle; }
                    };

        }
    }
}                    