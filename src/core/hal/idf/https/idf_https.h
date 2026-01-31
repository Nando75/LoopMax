#pragma once
#include "core/hal/interfaces/IHal_Https.h"
#include <WiFiClientSecure.h>


namespace LoopMax::Core::Hal {

    class idf_https : public IHal_Https {

    public:
        
        Types::HttpsResponse request(Types::HttpsData httpsData, const std::string& body,Types::WMethod method,const std::string& contentType) override { return Types::HttpsResponse{-1, "Must be implemented"}; }
        bool downloadFile(Types::HttpsData httpsData,const std::string& body,Types::WMethod method,const std::string& contentType, std::function<bool(uint8_t*, size_t)> onChunk
) override { return false; }

    private:
        Types::HttpsData _httpsData;
        
    
    };

}
