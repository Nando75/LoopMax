#include "ard_serial.h"

namespace LoopMax {
    namespace Core {
        namespace Hal {

                ard_serial::ard_serial() {
                #ifdef HAL_HAS_RTOS
                    mutex = xSemaphoreCreateMutex();
                #endif
                }

                bool ard_serial::begin(int baud) {
                    Serial.begin(baud);
                    return true;
                }

                bool ard_serial::available() {
                    return Serial.available() > 0;
                }

                int ard_serial::read() {
                    return Serial.read();
                }

                void ard_serial::write(const uint8_t* data, size_t len) {
                    with_lock([&] {
                        Serial.write(data, len);
                    });
                }

                void ard_serial::write(char c) {
                    with_lock([&] {
                        Serial.write(c);
                    });
                }

                void ard_serial::flush() {
                    with_lock([&] {
                        Serial.flush();
                    });
                }

                void ard_serial::printLn(const char* str) {
                    println_impl(str);
                }

                void ard_serial::printLn(char c) {
                    println_impl(c);
                }

                void ard_serial::printLn(int value) {
                    println_impl(value);
                }

                void ard_serial::printLn(unsigned long value) {
                    println_impl(value);
                }

                void ard_serial::print(const char* str) {
                    print_impl(str);
                }

                void ard_serial::print(char c) {
                    print_impl(c);
                }

                void ard_serial::print(int value) {
                    print_impl(value);
                }

                void ard_serial::print(unsigned long value) {
                    print_impl(value);
                }
                
            
        }
                
    }
}