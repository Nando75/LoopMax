#include "MqttsManager.h"
#include "BaseConfig.h"
#include "core/services/logs/LogsManager.h"
#include "core/services/system/SystemManager.h"


namespace LoopMax::Core {

                MqttsManager::MqttsManager()
                    : _mqtts(Hal::mqtts()),  _deps{"logs"}
                {}

                //IService
                void MqttsManager::start() {
                        if(ctx->system.mode() != SystemMode::LAN)
                        {
                            ctx->logs.write("Mqtts stopped",
                                    LoopMax::Types::LogType::INFO,
                                    this->name(),
                                    this->icon());

                            currentState = Types::ServiceState::STOPPED;
                            return;
                        }
                        
                        ctx->logs.write("Mqtts Ready",
                                    LoopMax::Types::LogType::INFO,
                                    this->name(),
                                    this->icon());

                        currentState = Types::ServiceState::READY;


                }

                void MqttsManager::loop() { 

                 }

                void MqttsManager::stop() { 
                    currentState = Types::ServiceState::STOPPED;
                }

              
                


                



}