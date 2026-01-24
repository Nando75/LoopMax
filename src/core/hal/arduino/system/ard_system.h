#pragma once

#if defined(HAL_FRAMEWORK_ARDUINO)

            #include "core/hal/interfaces/IHal_System.h"
            #include <Arduino.h>

            namespace LoopMax::Core::Hal {

                class ard_system : public IHal_System {
                public:
                    uint64_t uptime() override {
                        return static_cast<uint64_t>(millis());
                    }

                    const char* ChipModel() override {
                    #if defined(HAL_ESP_API)
                        return ESP.getChipModel();
                    #else
                        return nullptr;
                    #endif
                    }

                    const uint8_t ChipCores() override {
                    #ifdef HAL_CORES
                        return static_cast<uint8_t>(HAL_CORES);
                    #else
                        return 1;
                    #endif
                    }

                    const uint8_t ChipRevision() override {
                    #if defined(HAL_ESP_API) 
                        return static_cast<uint8_t>(ESP.getChipRevision());
                    #else
                        return 0;
                    #endif
                    }

                    const uint32_t FlashSize() override {
                    #if defined(HAL_ESP_API)
                        return ESP.getFlashChipSize();
                    #else
                        return 0;
                    #endif
                    }

                    const uint32_t FlashSpeed() override {
                    #if defined(HAL_ESP_API)
                        return ESP.getFlashChipSpeed();
                    #else
                        return 0;
                    #endif
                    }

                    const uint32_t FreeHeap() override {
                    #if defined(HAL_ESP_API)
                        return ESP.getFreeHeap();
                    #else
                        return 0;
                    #endif
                    }

                    const uint32_t HeapSize() override {
                    #if defined(HAL_ESP_API)
                        return ESP.getHeapSize();
                    #else
                        return 0;
                    #endif
                    }

                    const uint32_t CpuFreq() override {
                    #if defined(HAL_ESP_API)
                        return ESP.getCpuFreqMHz();
                    #else
                        return F_CPU / 1000000UL;
                    #endif
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
