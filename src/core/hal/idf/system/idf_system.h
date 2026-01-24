#pragma once

#if defined(HAL_FRAMEWORK_IDF)

        #include "core/hal/interfaces/IHal_System.h"

        extern "C" {
            #include "esp_system.h"
            #include "esp_timer.h"
        }

     namespace LoopMax::Core::Hal {

        class system_idf : public IHal_System {
        public:
            uint64_t uptime() override {
                return esp_timer_get_time();
            }

            const char* ChipModel() override {
                return esp_get_idf_version();
            }

            const uint8_t ChipCores() override {
            #ifdef HAL_CORES
                return static_cast<uint8_t>(HAL_CORES);
            #else
                return 1;
            #endif
            }

            const uint8_t ChipRevision() override {
                return static_cast<uint8_t>(esp_get_chip_revision());
            }

            const uint32_t FlashSize() override {
                uint32_t size = 0;
                esp_flash_get_size(nullptr, &size);
                return size;
            }

            const uint32_t FlashSpeed() override {
                return 0; // non sempre disponibile
            }

            const uint32_t FreeHeap() override {
                return esp_get_free_heap_size();
            }

            const uint32_t HeapSize() override {
                return heap_caps_get_total_size(MALLOC_CAP_8BIT);
            }

            const uint32_t CpuFreq() override {
                return esp_clk_cpu_freq() / 1000000UL;
            }

            void restart() override { 
                #if defined(ESP32) || defined(ESP8266)
                    esp_restart();
                #endif
            }


            Types::SystemMode mode() override { return Types::SystemMode::AP; }
            void setMode(Types::SystemMode mode) override {}
        };

    } 

#endif
