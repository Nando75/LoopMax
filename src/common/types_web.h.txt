#pragma once
#include <string>
#include <cstring>
#include <vector>
#include <functional>
#include "IHttpContext.h"

namespace LoopMax::Types {

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
            
            /*
           struct HttpsData {
                bool Https_ShowData;
                std::string Https_ApiUrl;
                uint32_t Https_Port;
                std::string Https_User;
                std::string Https_Psw;
                bool Https_UseCA;
                std::string Https_CA;
                uint32_t Https_TimeOut;
            };
            */







}