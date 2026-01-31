#pragma once
#include <cstdint>
#include <stdio.h>
#include <string>
#include <cstring>
#include <vector>
#include <functional>




namespace LoopMax::Types {
        
        
       // enum class ComponentState { STOPPED, INITIALIZING, READY, ERROR };

        enum class LogType : uint8_t {
            INFO,
            WARNING,
            ERROR,
            DEBUG

        };

        inline const char* LogTypeToStr(LogType t) {
            switch(t) {
                case LogType::INFO:    return "info";
                case LogType::WARNING: return "warning";
                case LogType::ERROR:   return "error";
                case LogType::DEBUG:   return "debug";
                default:               return "info";
            }
        }

        inline const char* LogTypeToIcon(LogType t) {
            switch(t) {
                case LogType::INFO:    return "ℹ";
                case LogType::WARNING: return "⚠";
                case LogType::ERROR:   return "✖️";
                case LogType::DEBUG:   return "#️⃣";
                default:               return "ℹ";
            }
        }

        struct LogTypeInfo {
            LogType type;
            const char* name;
            const char* icon;
        };

            struct LogEntry {
                uint64_t millis;
                std::string logType;
                std::string logIcon;
                std::string message;
                std::string payload;     // JSON raw
                std::string source;
                std::string sourceIcon;
            };




        enum class SystemMode : uint8_t {
            AP,
            LAN
        };

        inline const char* SystemModeToStr(SystemMode m) {
            switch(m) {
                case SystemMode::AP:    return "AP";
                case SystemMode::LAN:   return "LAN";
                default:                return "AP";
            }
        }

        
       

        inline SystemMode SystemModeFromStr(const char* mode) {
            if (strcmp(mode, "AP") == 0)    return SystemMode::AP;
            if (strcmp(mode, "LAN") == 0)   return SystemMode::LAN;
            return SystemMode::AP; // default
        }



        enum class ServiceState : uint8_t {
            STOPPED,
            INITIALIZING,
            READY,
            ERROR
        };

        inline const char* ServiceStateToStr(ServiceState s) {
            switch(s) {
                case ServiceState::STOPPED:      return "STOPPED";
                case ServiceState::INITIALIZING: return "INITIALIZING";
                case ServiceState::READY:        return "READY";
                case ServiceState::ERROR:        return "ERROR";
                default:                         return "UNKNOWN";
            }
        }




            //PIN MANAGER
           enum class PinMode {
                In,
                Out,
                InPullUp,
                InPullDown,
                Analog,
                Pwm,
                OpenDrain
            };

           inline const char* PinModeToStr(PinMode p) {
                switch(p) {
                    case PinMode::In:    return "In";
                    case PinMode::Out: return "Out";
                    case PinMode::InPullUp:   return "InPullUp";
                    case PinMode::InPullDown:   return "InPullDown";
                    case PinMode::Analog:   return "Analog";
                    case PinMode::Pwm:   return "Pwm";
                    case PinMode::OpenDrain:   return "OpenDrain";
                    default:               return "In";
                }
            }


           enum class PinLevel {
                Low = 0,
                High = 1
            };

            inline int PinLevelToInt(PinLevel p) {
                switch(p) {
                    case PinLevel::Low:    return 0;
                    case PinLevel::High: return 1;
                    default:             return 0;
                }
            }

            inline PinLevel PinLevelFromStr(std::string level) {
                if(level=="0") return PinLevel::Low;
                if(level=="1") return PinLevel::High;
                return PinLevel::Low;
            }

            


            enum class PinResult : uint8_t {
                OK,
                INVALID_PIN,
                UNSUPPORTED_MODE,
                UNSUPPORTED_OPERATION,
                NOT_INITIALIZED,
                ERROR
            };

            struct PwmConfig {
                uint32_t frequency = 1000;
                uint8_t  resolution = 8;
                uint8_t  channel = 0;   // utile soprattutto su IDF
            };



            
                //STORAGE MANAGER
                enum class StorageResult : uint8_t {
                    OK,
                    NOT_READY,
                    NOT_FOUND,
                    ALREADY_EXISTS,
                    OUT_OF_SPACE,
                    INVALID_ARG,
                    IO_ERROR,
                    UNSUPPORTED,
                    ERROR
                };

                enum class StorageOpenMode : uint8_t {
                    Read,
                    Write,
                    ReadWrite,
                    Append,
                    Create
                };

                enum class StorageSeek : uint8_t {
                    Set,
                    Cur,
                    End
                };

                enum class StorageType : uint8_t {
                    FileSystem,
                    BlockDevice,
                    KeyValue,
                    Raw
                };

                struct StorageInfo {
                    size_t totalBytes = 0;
                    size_t usedBytes  = 0;
                    StorageType type  = StorageType::Raw;
                };

                struct StorageFile {
                    std::vector<uint8_t> data;
                    std::string contentType;
                    std::string cacheControl;
                };

                inline std::string mimeFromPath(const std::string& path) {
                    auto dot = path.find_last_of('.');
                    if (dot == std::string::npos)
                        return "application/octet-stream";

                    std::string ext = path.substr(dot + 1);

                    if (ext == "html") return "text/html";
                    if (ext == "css")  return "text/css";
                    if (ext == "js")   return "application/javascript";
                    if (ext == "json") return "application/json";
                    if (ext == "png")  return "image/png";
                    if (ext == "jpg")  return "image/jpeg";
                    if (ext == "svg")  return "image/svg+xml";
                    if (ext == "woff2") return "font/woff2";

                    return "application/octet-stream";
                }


        

                //WIFI
                enum class WifiMode {
                    OFF,
                    STA,
                    AP,
                    AP_STA
                };

            inline const char* WifiModeToStr(WifiMode mode) {
                if (mode == WifiMode::AP) return "AP";
                if (mode == WifiMode::AP_STA) return "AP_STA";
                if (mode == WifiMode::OFF) return "OFF";
                if (mode == WifiMode::STA) return "STA";
                return "AP";
            }

               struct WifiNetwork {
                    std::string ssid;
                    int32_t rssi;
                };


                //MODULES
                struct modulePin
                {
                    std::string name = "";
                    uint32_t number = -1;
                    PinMode mode = PinMode::Out;
                    PinLevel level = PinLevel::Low;
                };

            struct TestConnectionResult {
                bool success;
                uint32_t ip; 
            };


        struct MqttsData {
            bool Mqtts_ShowData = false;
            bool Mqtts_UseCA = false;
            std::string Mqtts_Server = "";
            uint32_t Mqtts_Port = 80;
            std::string Mqtts_UniqueId = "";
            std::string Mqtts_User = "";
            std::string Mqtts_Psw = "";
            std::string Mqtts_CA = "";
            uint32_t Mqtts_Qos = 0;
            uint32_t Mqtts_Retain = 0;
            std::string Mqtts_BaseTopic = "";
            uint32_t Mqtts_TimeOut = 0;   

        };
        
        using MqttCallback = std::function<void(
            const std::string& topic,
            const std::string& payload
        )>;


}

