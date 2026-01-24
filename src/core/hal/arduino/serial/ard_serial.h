#pragma once
#include <Arduino.h>
#include "core/hal/interfaces/IHal_Serial.h"


namespace LoopMax::Core::Hal {

                class ard_serial : public IHal_Serial {
                    public:
                        ard_serial();
                        bool begin(int baud) override;
                        bool available() override;
                        int read() override;
                        void write(const uint8_t* data, size_t len) override;
                        void write(char c) override;
                        void flush() override;
                        
                        void printLn(const char* str) override;
                        void printLn(char c) override;
                        void printLn(int value) override;
                        void printLn(unsigned long value) override;

                        void print(const char* str) override;
                        void print(char c) override;
                        void print(int value) override;
                        void print(unsigned long value) override;

                    private:
                    #ifdef HAL_HAS_RTOS
                        SemaphoreHandle_t mutex;
                    #endif

                        template<typename F>
                        void with_lock(F fn) {
                    #ifdef HAL_HAS_RTOS
                            xSemaphoreTake(mutex, portMAX_DELAY);
                    #endif
                            fn();
                    #ifdef HAL_HAS_RTOS
                            xSemaphoreGive(mutex);
                    #endif
                        }

                        template<typename T>
                        void println_impl(T value) {
                            with_lock([&] {
                                Serial.println(value);
                            });
                        }


                        template<typename T>
                        void print_impl(T value) {
                            with_lock([&] {
                                Serial.print(value);
                            });
                        }

                };

 
}