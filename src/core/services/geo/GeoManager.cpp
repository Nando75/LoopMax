#include "GeoManager.h"
#include "core/services/logs/LogsManager.h"


namespace LoopMax::Core {

                GeoManager::GeoManager()
                    : _geo(Hal::geo()), _deps{"logs"} 
                {}
                //IService
                void GeoManager::start() {
                    ctx->logs.write("GeoInfo Ready",LoopMax::Types::LogType::INFO,this->name(), this->icon());
                }
                void GeoManager::loop() {  }
                void GeoManager::stop() { 
                    currentState = Types::ServiceState::STOPPED;
                 }
                
                 





}