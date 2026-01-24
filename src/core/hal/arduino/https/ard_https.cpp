#include "ard_https.h"
#include <HTTPClient.h>



namespace LoopMax::Core::Hal {
    using namespace LoopMax::Types;


    Types::HttpsResponse ard_https::request(Types::HttpsData httpsData, const std::string& body,Types::WMethod method,const std::string& contentType) {
        
        if (httpsData.Https_ApiUrl.empty()) return Types::HttpsResponse{-1, "Api url empty"};
        WiFiClientSecure client;
        if (httpsData.Https_UseCA && !httpsData.Https_CA.empty()) {
            client.setCACert(httpsData.Https_CA.c_str());
        } else {
            client.setInsecure();
        }
        client.setTimeout(httpsData.Https_TimeOut);

        HTTPClient https;
        if (!httpsData.Https_Port) {
            https.begin(client, httpsData.Https_ApiUrl.c_str());
        } else {
            https.begin(client, httpsData.Https_ApiUrl.c_str(), httpsData.Https_Port,"/");
        }

        if (!contentType.empty()) {
            https.addHeader("Content-Type", contentType.c_str());
        }

        int httpCode = -1;
        if (method == Types::WMethod::POST) {
            httpCode = https.POST(body.c_str());
        } else {
            httpCode = https.GET();
        }

        std::string responseBody = "";
        if (httpCode > 0) {
            responseBody = https.getString().c_str();
        }

        https.end();
        return Types::HttpsResponse{httpCode, responseBody};
    }






        

}
