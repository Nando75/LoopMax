
#pragma once
#include "core/hal/hal.h"
#include "core/services/IService.h"
#include "core/hal/interfaces/IHal_Update.h"
#include "common/iservices/IUpdate.h"
#include "core/SystemContext.h"



namespace LoopMax {
    namespace Core {
        using namespace Services;
        using namespace Hal;

            class UpdateManager : public IService,  public IHal_Update, public IUpdate {
                public:
                    UpdateManager();
                    void initContext(SystemContext& context) { ctx = &context; }

                    //IService:
                    void start() override;
                    void loop() override;
                    void stop() override;
                    const char* name() override { return "update"; }
                    const  char* icon() override { return "⏬"; }
                    const std::vector<Types::SystemMode>& modes() override { return _modes; }
                    const std::vector<const char*>& dependencyNames() override { return _deps; }
                    Types::ServiceState& state() override { return currentState; }
                    const std::vector<WebCommand>& webCommands() const override { return _webCommands; } 
                    void registerEndpoints() override {}
                    
                    

                private:
                    Types::ServiceState currentState = Types::ServiceState::READY;
                    const std::vector<Types::SystemMode> _modes = { Types::SystemMode::AP, Types::SystemMode::LAN };
                    Hal::IHal_Update& _update;
                    std::vector<const char*> _deps;
                    std::vector<WebCommand> _webCommands;
                    SystemContext* ctx = nullptr;


                };


    }
}


