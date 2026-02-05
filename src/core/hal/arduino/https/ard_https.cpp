#include "ard_https.h"
#include <HTTPClient.h>
#include <esp_task_wdt.h>


namespace LoopMax::Core::Hal {
    using namespace LoopMax::Types;


    Types::HttpsResponse ard_https::request(Types::HttpsData httpsData, const std::string& body,Types::WMethod method,const std::string& contentType) {
        
        if (httpsData.Https_ApiUrl.empty()) return Types::HttpsResponse{-1, "Api url empty"};
        
       WiFiClientSecure client;
       /* 
       if (httpsData.Https_UseCA && !httpsData.Https_CA.empty()) {
            client.setCACert(httpsData.Https_CA.c_str());
        } else {
            client.setInsecure();
        }
        */
        client.setInsecure();

        client.setTimeout(httpsData.Https_TimeOut);

        HTTPClient https;

        https.begin(client, httpsData.Https_ApiUrl.c_str());

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
        client.stop();
        return Types::HttpsResponse{httpCode, responseBody};
    }

        bool ard_https::downloadFile(Types::HttpsData httpsData,
                             const std::string& body,
                             Types::WMethod method,
                             const std::string& contentType,
                             std::function<bool(uint8_t*, size_t)> onChunk)
                {
                    WiFiClientSecure client;

                    if (httpsData.Https_UseCA && !httpsData.Https_CA.empty()) {
                        client.setCACert(httpsData.Https_CA.c_str());
                    } else {
                        client.setInsecure();
                    }

                    client.setTimeout(httpsData.Https_TimeOut);

                    HTTPClient https;

                    if (!https.begin(client, httpsData.Https_ApiUrl.c_str())) {
                        return false;
                    }

                    if (!contentType.empty()) {
                        https.addHeader("Content-Type", contentType.c_str());
                    }

                    int httpCode = (method == Types::WMethod::POST)
                                    ? https.POST(body.c_str())
                                    : https.GET();

                    if (httpCode != HTTP_CODE_OK) {
                        https.end();
                        return false;
                    }

                    WiFiClient* stream = https.getStreamPtr();
                    uint8_t buff[512];

                    /*
                    while (https.connected()) {
                        size_t available = stream->available();
                        if (!available) {
                            delay(1);
                            continue;
                        }

                        size_t read = stream->readBytes(buff, std::min(available, sizeof(buff)));
                        if (read == 0) break;

                        if (!onChunk(buff, read)) {
                            https.end();
                            return false; // il chiamante ha deciso di interrompere
                        }
                    }
                    */
                   while (https.connected()) {
                            esp_task_wdt_reset();
                            yield();
                            delay(1);

                            size_t available = stream->available();
                            if (!available) {
                                delay(1);
                                continue;
                            }

                            size_t read = stream->readBytes(buff, std::min(available, sizeof(buff)));
                            if (read == 0) break;

                            if (!onChunk(buff, read)) {
                                https.end();
                                return false;
                            }
                        }



                    https.end();
                    return true;
                }









        

}
