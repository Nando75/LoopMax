#include "StorageManager.h"
#include "BaseConfig.h"
#include "core/services/logs/LogsManager.h"


namespace LoopMax::Core {

                StorageManager::StorageManager()
                    : _storage(Hal::storage()),  _deps{"logs"}
                {}

                //IService
                void StorageManager::start() {
                        _storage.begin();
                        bool res = _storage.isReady();
                        if (!res) {
                            ctx->logs.write("Storage mount failed",
                                        LoopMax::Types::LogType::ERROR,
                                        this->name(),
                                        this->icon());
                            currentState = Types::ServiceState::ERROR;
                            return;
                        }

                        ctx->logs.write("Storage Ready",
                                    LoopMax::Types::LogType::INFO,
                                    this->name(),
                                    this->icon());

                        currentState = Types::ServiceState::READY;
                }

                void StorageManager::loop() {  }

                void StorageManager::stop() { 
                    _storage.end();
                    currentState = Types::ServiceState::STOPPED;
                }

                



}