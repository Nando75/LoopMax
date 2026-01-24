#pragma once 
#include "common/types_web.h"

namespace LoopMax::Core::Hal {
    class IHal_Https {
    public:
        
        virtual Types::HttpsResponse request(Types::HttpsData httpsData, const std::string& body,Types::WMethod method,const std::string& contentType) = 0;


       
        virtual ~IHal_Https() = default;
    };
}
