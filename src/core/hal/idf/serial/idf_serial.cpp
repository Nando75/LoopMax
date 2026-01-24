#include "idf_serial.h"
#include "driver/uart.h"
#include <cstring>
#include <cstdio>

namespace LoopMax {
    namespace Core {
        namespace Hal {

                static constexpr uart_port_t UART_PORT = UART_NUM_0;

                idf_serial::idf_serial() {
                #ifdef HAL_HAS_RTOS
                    mutex = xSemaphoreCreateMutex();
                #endif
                }

                idf_serial::~idf_serial() {}

                bool idf_serial::begin(int baud) {
                    uart_config_t cfg = {};
                    cfg.baud_rate = baud;
                    cfg.data_bits = UART_DATA_8_BITS;
                    cfg.parity    = UART_PARITY_DISABLE;
                    cfg.stop_bits = UART_STOP_BITS_1;
                    cfg.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
                    cfg.rx_flow_ctrl_thresh = 0;

                    uart_param_config(UART_PORT, &cfg);
                    uart_driver_install(UART_PORT, 2048, 0, 0, nullptr, 0);
                    return true;
                }

                bool idf_serial::available() {
                    size_t len = 0;
                    uart_get_buffered_data_len(UART_PORT, &len);
                    return len > 0;
                }

                int idf_serial::read() {
                    uint8_t c;
                    int n = uart_read_bytes(UART_PORT, &c, 1, 0);
                    return (n == 1) ? c : -1;
                }

                void idf_serial::write(const uint8_t* data, size_t len) {
                    with_lock([&] {
                        uart_write_bytes(UART_PORT, (const char*)data, len);
                    });
                }

                void idf_serial::write(char c) {
                    with_lock([&] {
                        uart_write_bytes(UART_PORT, &c, 1);
                    });
                }

                void idf_serial::flush() {
                    with_lock([&] {
                        uart_wait_tx_done(UART_PORT, pdMS_TO_TICKS(100));
                    });
                }

                /* -------- printLn = write + \r\n -------- */

                void idf_serial::print_value(const char* str) {
                    write((const uint8_t*)str, strlen(str));
                    write((const uint8_t*)"\r\n", 2);
                }

                void idf_serial::print_value(char c) {
                    write(c);
                    write((const uint8_t*)"\r\n", 2);
                }

                void idf_serial::print_value(int value) {
                    char buf[12];
                    snprintf(buf, sizeof(buf), "%d", value);
                    print_value(buf);
                }

                void idf_serial::print_value(unsigned long value) {
                    char buf[11]; // 10 cifre + '\0'
                    snprintf(buf, sizeof(buf), "%lu", value);
                    print_value(buf);
                }

                void idf_serial::printLn(const char* str) { println_impl(str); }
                void idf_serial::printLn(char c) { println_impl(c); }
                void idf_serial::printLn(int value) { println_impl(value); }
                void idf_serial::printLn(unsigned long value) { println_impl(value); }

        }
    }
}