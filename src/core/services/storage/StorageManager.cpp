#include "StorageManager.h"
#include "BaseConfig.h"
#include "core/services/logs/LogsManager.h"


namespace LoopMax::Core {

                StorageManager::StorageManager()
                    : _storage(Hal::storage()),  _deps{"logs"}
                {}

                void StorageManager::begin() {
                      _storage.begin(); 
                }


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

                        ctx->logs.write("FsOffset: " + std::to_string(_storage.FS_OFFSET()) + 
                                        " FsSize: " + std::to_string(_storage.FS_SIZE()),
                                    LoopMax::Types::LogType::DEBUG,
                                    this->name(),
                                    this->icon());

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