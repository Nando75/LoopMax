
#pragma once
#include "core/hal/hal.h"
#include "core/services/IService.h"
#include "core/hal/interfaces/IHal_Storage.h"
#include "core/SystemContext.h"

namespace LoopMax {
    namespace Core {
        using namespace Hal;
        

            class StorageManager : public IService,  public IHal_Storage {
                public:
                    StorageManager();
                    void initContext(SystemContext& context) { ctx = &context; }

                    //IService:
                    void start() override;
                    void loop() override;
                    void stop() override;
                    const char* name() override { return "storage"; }
                    const  char* icon() override { return "💾"; }
                    const std::vector<Types::SystemMode>& modes() override { return _modes; }
                    const std::vector<const char*>& dependencyNames() override { return _deps; }
                    Types::ServiceState& state() override { return currentState; }
                    const std::vector<WebCommand>& webCommands() const override { return _webCommands; } 
                    void registerEndpoints() override {}


                    //IHalStorage
                    void begin()  override {  _storage.begin(); }
                    bool isReady() const override {  return _storage.isReady(); }
                    void end()  override {  _storage.end(); }
                    bool getFile(const std::string &path, std::string &outData) override {  return _storage.getFile(path, outData); }
                    bool saveFile(const std::string &path, const std::string &data) override {  return _storage.saveFile(path,data); }

                private:
                    Types::ServiceState currentState = Types::ServiceState::READY;
                    const std::vector<Types::SystemMode> _modes = { Types::SystemMode::AP, Types::SystemMode::LAN };
                    IHal_Storage& _storage;
                    std::vector<const char*> _deps;
                    SystemContext* ctx = nullptr;
                    std::vector<WebCommand> _webCommands;


                };


    }
}


