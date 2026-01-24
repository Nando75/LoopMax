#pragma once
#include "core/hal/interfaces/IHal_Https.h"
#include <WiFiClientSecure.h>


namespace LoopMax::Core::Hal {

    class idf_https : public IHal_Https {

    public:
        
        Types::HttpsResponse request(Types::HttpsData httpsData, const std::string& body,Types::WMethod method,const std::string& contentType) override { return Types::HttpsResponse{-1, "Must be implemented"}; }

    private:
        Types::HttpsData _httpsData;
        
    
    };

}
