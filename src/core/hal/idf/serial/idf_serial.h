#pragma once
#include "core/hal/interfaces/IHal_Serial.h"

#ifdef HAL_HAS_RTOS
    #include "freertos/FreeRTOS.h"
    #include "freertos/semphr.h"
#endif

namespace LoopMax {
    namespace Core {
            namespace Hal {
                class idf_serial : public IHal_Serial {
                public:
                    idf_serial();
                    virtual ~idf_serial() override;

                    bool begin(int baud) override;

                    void printLn(const char* str) override;
                    void printLn(char c) override;
                    void printLn(int value) override;
                    void printLn(unsigned long value) override;

                    bool available() override;
                    int  read() override;

                    void write(const uint8_t* data, size_t len) override;
                    void write(char c) override;
                    void flush() override;

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
                            print_value(value); 
                        });
                    }

                    void print_value(const char* str);
                    void print_value(char c);
                    void print_value(int value);
                    void print_value(unsigned long value);
                };

        }
    }
}