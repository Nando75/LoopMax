
#include "core/TaskManager.h"
LoopMax::TaskManager task;



#if defined(HAL_FRAMEWORK_ARDUINO)
    
    void setup() {
        task.start();
    }

    void loop() {}
        #ifdef HAL_HAS_RTOS
            #if HAL_CORES == 2
                #pragma message(">>> Arduino | CORES: 2 | RTOS: YES <<<")
            #else
                #pragma message(">>> Arduino | CORES: 1 | RTOS: YES <<<")
            #endif
        #else
            #pragma message(">>> Arduino | CORES: 1 | RTOS: NO <<<")
        #endif


#elif defined(HAL_FRAMEWORK_IDF)
    #include "esp_log.h"
    extern "C" void app_main() {
        ESP_LOGI("MAIN", "HAL initialized: HAL_FRAMEWORK_IDF");
        task.start();
        while (true) {
            vTaskDelay(pdMS_TO_TICKS(10));
        }

    }
        #ifdef HAL_HAS_RTOS
            #if HAL_CORES == 2
                #pragma message(">>> IDF | CORES: 2 | RTOS: YES <<<")
            #else
                #pragma message(">>> IDF | CORES: 1 | RTOS: YES <<<")
            #endif
        #else
            #pragma message(">>> IDF | CORES: 1 | RTOS: NO <<<")
        #endif
#else
    #error "HAL ERROR: No framework selected (define HAL_FRAMEWORK_ARDUINO or HAL_FRAMEWORK_IDF)"
#endif






