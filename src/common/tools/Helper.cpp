#include "Helper.h"
#include <stdio.h>


#if defined(ESP_PLATFORM) || defined(ARDUINO_ARCH_ESP32)
    #include <esp_random.h>                    
#endif

namespace LoopMax::Utils {
   
            void Helper::addPin(
                std::vector<modulePin>& pins,
                std::string name,
                uint32_t number,
                PinMode mode,
                PinLevel level
            ) {
                pins.emplace_back();
                auto& p = pins.back();
                p.name   = name;
                p.number = number;
                p.mode   = mode;
                p.level  = level;
            }

                int Helper::getRandomNumber(int max)
                {
                    if (max <= 0) return 0;
                    return getRandomU32() % max;
                }


                void Helper::getRandomString(char* out, size_t length)
                    {
                        if (!out || length < 2) return;

                        static const char charset[] =
                            "abcdefghijklmnopqrstuvwxyz"
                            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                            "0123456789";

                        const size_t charsetLen = sizeof(charset) - 1;

                        for (size_t i = 0; i < length - 1; ++i) {
                            out[i] = charset[getRandomU32() % charsetLen];
                        }

                        out[length - 1] = '\0';
                    }


                uint32_t Helper::getRandomU32()
                {
                    #if defined(ESP_PLATFORM)
                        // ESP-IDF
                        return esp_random();

                    #elif defined(ARDUINO_ARCH_ESP32)
                        // Arduino ESP32 (usa comunque HW RNG)
                        return esp_random();

                    #elif defined(ARDUINO)
                        // Arduino generico (weak RNG)
                        static uint32_t seed = 0xA5A5A5A5;
                        seed = seed * 1664525UL + 1013904223UL;
                        return seed;

                    #else
                        // Host / test
                        static uint32_t seed = 123456789;
                        seed = seed * 1103515245 + 12345;
                        return seed;
                    #endif
                }




}
