#include "HttpsManager.h"
#include "BaseConfig.h"
#include "core/services/logs/LogsManager.h"
#include "core/services/system/SystemManager.h"


namespace LoopMax::Core {

                HttpsManager::HttpsManager()
                    : _https(Hal::https()),  _deps{"logs"}
                {}

                //IService
                void HttpsManager::start() {
                        if(ctx->system.mode() != SystemMode::LAN)
                        {
                            ctx->logs.write("Https stopped",
                                    LoopMax::Types::LogType::INFO,
                                    this->name(),
                                    this->icon());

                            currentState = Types::ServiceState::STOPPED;
                            return;
                        }
                        
                        ctx->logs.write("Https Ready",
                                    LoopMax::Types::LogType::INFO,
                                    this->name(),
                                    this->icon());

                        currentState = Types::ServiceState::READY;


                }

                void HttpsManager::loop() { 

                 }

                void HttpsManager::stop() { 
                    currentState = Types::ServiceState::STOPPED;
                }

                
                Types::HttpsResponse HttpsManager::request(Types::HttpsData httpsData, const std::string& body,Types::WMethod method,const std::string& contentType) 
                {
                    if(ctx->system.mode() != SystemMode::LAN) return Types::HttpsResponse{-1, "Only in CLOUD mode."};
                    return _https.request(httpsData, body, method, contentType); 
                }

                



}