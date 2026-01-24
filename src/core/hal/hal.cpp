#include "hal.h"
// ===============================
// Framework selection
// ===============================




            #if defined(HAL_FRAMEWORK_ARDUINO)
                #include <Arduino.h>
                
                // TASK/RTOS
                #if defined(HAL_HAS_RTOS)
                    #include "core/hal/arduino//task/ard_task_rtos.h"
                    static LoopMax::Core::Hal::ard_task_rtos task_impl;
                #else // NO RTOS
                    #include "core/hal/arduino//task/ard_task.h"
                    static LoopMax::Core::Hal::ard_task task_impl;
                #endif 
                
                
                //TIME:
                #include "core/hal/arduino/timer/ard_timer.h"
                static LoopMax::Core::Hal::ard_timer time_impl;

                //LOGS:
                //#include "core/services/logs/LogsManager.h"
                //static LoopMax::Core::LogsManager logs_impl;

                //SYSTEM:
                #include "core/hal/arduino/system/ard_system.h"
                static LoopMax::Core::Hal::ard_system system_impl;

                //PINS:
                #include "core/hal/arduino/pins/ard_pins.h"
                static LoopMax::Core::Hal::ard_pins pins_impl;


                //SERIAL:
                #include "core/hal/arduino/serial/ard_serial.h"
                static LoopMax::Core::Hal::ard_serial serial_impl;

                //STORAGE:
                #if defined(HAL_USE_LITTLEFS)
                    #include "core/hal/arduino/storage/ard_storage_littlefs.h"
                    static LoopMax::Core::Hal::ard_storage_littlefs storage_impl;
                
                //#elif defined(DHAL_MCU_ESP8266)
                    
                #endif

                //CRYPTO:
                //#include "sys/hal/ard/crypt/ard_aesCrypto.h"
                //static LoopMax::Core::ard_aesCrypto crypto_impl;


                //WIFI:
                #if defined(HAL_MCU_ESP32)
                    #include "core/hal/arduino/wifi/ard_esp32wifi.h"
                    static LoopMax::Core::Hal::ard_esp32wifi wifi_impl;
                //#elif defined(DHAL_MCU_ESP8266)
                    
                #endif

                //WEB:
                #if defined(HAL_MCU_ESP32)
                    #include "core/hal/arduino/web/ard_esp32_asyncweb.h"
                    static LoopMax::Core::Hal::ard_esp32_asyncweb web_impl;
                //#elif defined(DHAL_MCU_ESP8266)

                    
                #endif

                //GEO INFO:
                #include "core/hal/arduino/geo/ard_geoinfo.h"
                static LoopMax::Core::Hal::ard_geoInfo geo_impl;

                //UPDATE:
                #include "core/hal/arduino/update/ard_update.h"
                static LoopMax::Core::Hal::ard_update update_impl;


                //HTTPS:
                #include "core/hal/arduino/https/ard_https.h"
                static LoopMax::Core::Hal::ard_https https_impl;


                //MQTTS:
                #include "core/hal/arduino/mqtts/ard_mqtts.h"
                static LoopMax::Core::Hal::ard_mqtts mqtts_impl;



            #elif defined(HAL_FRAMEWORK_IDF)
                #include "esp_system.h"
                #include "esp_log.h"
                #include "esp_mac.h"
                
                //TASK/RTOS
                #if defined(HAL_HAS_RTOS)
                    #include "freertos/FreeRTOS.h"
                    #include "freertos/task.h"
                    #include "core/hal/idf//task/idf_task_rtos.h"
                    static LoopMax::Core::Hal::idf_task_rtos task_impl;
                #else  // NO RTOS
                    #include "core/hal/idf//task/idf_task.h"
                    static LoopMax::Core::Hal::idf_task task_impl;
                #endif   

                //TIME:
                #include "core/hal/idf/timer/idf_timer.h"
                static LoopMax::Core::Hal::idf_timer time_impl;
                
                //LOGS:
                //#include "core/services/logs/LogsManager.h"
                //static LoopMax::Core::LogsManager logs_impl;

                //SYSTEM:
                #include "core/hal/idf/system/idf_system.h"
                static LoopMax::Core::Hal::idf_system system_impl;

                //PINS
                #include "core/hal/idf/pins/idf_pins.h"
                static LoopMax::Core::Hal::idf_pins pins_impl;

                //SERIAL:
                #include "core/hal/idf/serial/idf_serial.h"
                static LoopMax::Core::Hal::idf_serial  _impl;

                //STORAGE:
                #if defined(HAL_USE_LITTLEFS)
                    #include "core/hal/idf/storage/idf_storage_littlefs.h"
                    static LoopMax::Core::Hal::idf_storage_littlefs storage_impl;
                
                //#elif defined(DHAL_MCU_ESP8266)
                    
                #endif

                //CRYPTO:
                //#include "sys/hal/idf/crypt/idf_aesCrypto.h"
                //static idf_aesCrypto crypto_impl;

               
               
                
                //WIFI:
                #if defined(HAL_MCU_ESP32C6)
                    #include "core/hal/idf/wifi/idf_esp32wifi.h"
                    static LoopMax::Core::Hal::idf_esp32wifi wifi_impl;
                #endif
                
                //WEB:
                #if defined(HAL_MCU_ESP32C6)
                    #include "core/hal/idf/web/idf_esp32_asyncweb.h"
                    static LoopMax::Core::Hal::idf_esp32_asyncweb web_impl;
                //#elif defined(DHAL_MCU_ESP8266)
                    
                #endif
                //GEO INFO:
                #include "core/hal/idf/geo/idf_geoinfo.h"
                static LoopMax::Core::Hal::idf_geoInfo geo_impl;

                //UPDATE:
                #include "core/hal/idf/update/idf_update.h"
                static LoopMax::Core::Hal::idf_update update_impl;

                //HTTPS:
                #include "core/hal/idf/https/idf_https.h"
                static LoopMax::Core::Hal::idf_https https_impl;

                //MQTTS:
                #include "core/hal/idf/mqtts/idf_mqtts.h"
                static LoopMax::Core::Hal::idf_mqtts mqtts_impl;
                

            #endif


            // ===============================
            // Core / threading configuration
            // ===============================
            #ifdef HAL_HAS_RTOS
                #if defined(HAL_CORES) && (HAL_CORES == 2)
                    //const int MAX_CORES = 2;
                #else
                    const int MAX_CORES = 1;
                #endif
            #else
                const int MAX_CORES = 1;
            #endif



                #ifndef HAL_MONITOR_SPEED
                    #define HAL_MONITOR_SPEED 115200
                #endif
                /*
                #ifdef HAL_MONITOR_SPEED
                    #define MONITOR_SPEED HAL_MONITOR_SPEED
                #else
                    #define MONITOR_SPEED 115200
                #endif
                #define STR2(x) #x
                #define STR(x) STR2(x)
                #pragma message("Monitor speed resolved to: " STR(MONITOR_SPEED))
                const long monitor_speed_value = MONITOR_SPEED;
                */
                //unsigned long MONITOR_SPEED = HAL_MONITOR_SPEED;


    /*
    ICrypto& crypto() {
        return crypto_impl;
    }

    IStorage& storage() {
        return storage_impl;
    }
    
    IWifi& wifi() {
        return wifi_impl;
    }

IWifi& Sys::wifi()
{
    return wifi_impl;
}
*/


// ===============================
// Public accessors
// ===============================

namespace LoopMax {
    namespace Core {
        namespace Hal {
            
             Hal::IHal_Task* createTask() {
                #ifdef HAL_HAS_RTOS
                    //una istanza per ogni chiamata
                    return new ard_task_rtos();
                #else
                    static ard_task no_rtos;
                    return &no_rtos;
               #endif
            }

            Hal::IHal_Timer& hal_timer() {
                return time_impl;
            }

            

            Hal::IHal_Serial& serial() {
                return serial_impl;
            }
            Hal::IHal_System& sys() {
                return system_impl;
            }
            
            Hal::IHal_Pins& pins() {
                return pins_impl;
            }

            Hal::IHal_Storage& storage() {
                return storage_impl;
            }

            Hal::IHal_Wifi& wifi() {
                return wifi_impl;
            }

            Hal::IHal_Web& web() {
                return web_impl;
            }

            Hal::IHal_GeoInfo& geo() {
                return geo_impl;
            }

            Hal::IHal_Update& update() {
                return update_impl;
            }

            Hal::IHal_Https& https() {
                return https_impl;
            }

            Hal::IHal_Mqtts& mqtts() {
                return mqtts_impl;
            }


        }
    }
}        





/*
    //COMPONENTI AGGIUNTIVI:
    #ifdef HAL_USE_RELEMANAGER
        #include "components/relay.h"
        static Relay relay_impl;
    #endif



*/