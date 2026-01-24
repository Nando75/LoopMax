#include "UpdateManager.h"
#include "core/services/logs/LogsManager.h"



namespace LoopMax::Core {

                UpdateManager::UpdateManager()
                    : _update(Hal::update()), _deps{}
                {}
                //IService
                void UpdateManager::start() {
                    
                    ctx->logs.write("Update Ready",Types::LogType::INFO,this->name(),this->icon());
                }
                void UpdateManager::loop() {  }
                void UpdateManager::stop() { 
                    currentState = Types::ServiceState::STOPPED;
                 }
                 
                 
                 //IHal_Update
                

                 //IUpdate


}