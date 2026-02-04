#pragma once
#include <cstdint>
#include <stdio.h>
#include <string>
#include <cstring>
#include <vector>
#include <functional>


#include "IHttpContext.h"


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



    //WEB


    // Enumeratore completo dei metodi Web
    enum class WMethod : int {
        GET     = 0b00000001, // 0x01
        POST    = 0b00000010, // 0x02
        DELETE  = 0b00000100, // 0x04
        PUT     = 0b00001000, // 0x08
        PATCH   = 0b00010000, // 0x10
        HEAD    = 0b00100000, // 0x20
        OPTIONS = 0b01000000, // 0x40
        ANY     = 0b01111111  // 0x7F (Tutti i precedenti)
    };

    // Definiamo il tipo esatto della funzione
    using WebHandler = std::function<void(IHttpContext&)>;

    struct WebCommand {
        std::string uri;
        int method;         // Cambiato in int per compatibilità totale
        WebHandler handler; 
    };



   inline const char* WebMethodToStr(WMethod method) {
        switch (method) {
            case WMethod::GET:     return "GET";
            case WMethod::POST:    return "POST";
            case WMethod::PUT:     return "PUT";
            case WMethod::DELETE:  return "DELETE";
            case WMethod::PATCH:   return "PATCH";
            case WMethod::HEAD:    return "HEAD";
            case WMethod::OPTIONS: return "OPTIONS";
            case WMethod::ANY:     return "ANY";
            default:               return "UNKNOWN";
        }
    }


            inline const char* getMime(const std::string& path)
                {
                    const char* p = path.c_str();
                    const char* ext = strrchr(p, '.');
                    if (!ext || *(ext + 1) == '\0')
                        return "application/octet-stream";

                    ext++; // skip '.'

                    if (!strcasecmp(ext, "html") || !strcasecmp(ext, "htm")) return "text/html";
                    if (!strcasecmp(ext, "css"))  return "text/css";
                    if (!strcasecmp(ext, "js"))   return "application/javascript";
                    if (!strcasecmp(ext, "json")) return "application/json";
                    if (!strcasecmp(ext, "txt"))  return "text/plain";
                    if (!strcasecmp(ext, "xml"))  return "application/xml";

                    if (!strcasecmp(ext, "png"))  return "image/png";
                    if (!strcasecmp(ext, "jpg") || !strcasecmp(ext, "jpeg")) return "image/jpeg";
                    if (!strcasecmp(ext, "svg"))  return "image/svg+xml";
                    if (!strcasecmp(ext, "ico"))  return "image/x-icon";
                    if (!strcasecmp(ext, "pdf"))  return "application/pdf";

                    return "application/octet-stream";
                }


            struct HttpsResponse {
                int code;
                std::string body;
            };

            struct HttpsBinaryResponse {
                int code;
                std::vector<uint8_t> data;
                std::string error;
            };



            //HTTPS:
            
            struct HttpsData {
                bool Https_ShowData = false;
                std::string Https_ApiUrl = "";
                uint32_t Https_Port = 0;
                std::string Https_User = "";
                std::string Https_Psw = "";
                bool Https_UseCA = false;     
                std::string Https_CA = "";
                uint32_t Https_TimeOut = 0;   
            };



             struct IModuleData {
                std::string FwVersion = "";
                bool UpdateAvalaible = false;
                std::string newFwVersion = "";
                std::string Name = "";
                std::string DeviceName = "";
                
                std::string Icon = "🧩";
                std::string Code = "";
                std::string Label = "";
                std::string Company = "";
                std::string Url = "";
                bool isRegistered = false;

                HttpsData httpsData;
                MqttsData mqttsData;

                bool SaveConfig = false;
                std::vector<modulePin> pins;
                std::string JsonConfig = "";

                std::string JsUIClass = "";
                std::vector<std::string> JsFiles;


                std::vector<WebCommand> webCommands;

            };


}

