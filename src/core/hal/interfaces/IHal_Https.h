#pragma once 
#include "common/types.h"

namespace LoopMax::Core::Hal {
    class IHal_Https {
    public:
        
        virtual Types::HttpsResponse request(Types::HttpsData httpsData, const std::string& body,Types::WMethod method,const std::string& contentType) = 0;
        virtual bool downloadFile(Types::HttpsData httpsData,
                          const std::string& body,
                          Types::WMethod method,
                          const std::string& contentType,
                          std::function<bool(uint8_t*, size_t)> onChunk) = 0;


       
        virtual ~IHal_Https() = default;
    };
}
