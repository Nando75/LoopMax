#pragma once
#include "core/hal/interfaces/IHal_Https.h"
#include <WiFiClientSecure.h>


namespace LoopMax::Core::Hal {

    class ard_https : public IHal_Https {

    public:
        Types::HttpsResponse request(Types::HttpsData httpsData, const std::string& body,Types::WMethod method,const std::string& contentType) override;

    private:
        Types::HttpsData _httpsData;
        
    
    };

}
