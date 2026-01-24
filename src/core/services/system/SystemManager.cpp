#include "SystemManager.h"
#include "BaseConfig.h"
#include "core/services/logs/LogsManager.h"

namespace LoopMax::Core {

                SystemManager::SystemManager()
                    : sys(Hal::sys()), _deps{"timer","logs","serial"}
                { }

                //IService
                void SystemManager::start(){
                    if(!IS_DEBUG) ctx->logs.write("System Ready",Types::LogType::INFO,this->name(),this->icon());
                    if(IS_DEBUG) ctx->logs.write("System Ready ##DEBUG##",Types::LogType::INFO,this->name(),this->icon());
                }
                void SystemManager::loop() {}
                void SystemManager::stop() {}


                
                void SystemManager::registerEndpoints() { 

                    _webCommands.push_back({
                            "system/restart",
                            0x01,
                            [this](IHttpContext& httpCtx){
                                httpCtx.send(200, "application/json", "{\"status\":\"ok\"}");
                                this->restart();
                            }
                        });
                }
    
}