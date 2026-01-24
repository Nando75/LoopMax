
#pragma once
#include "core/hal/hal.h"
#include "core/services/IService.h"
#include "core/hal/interfaces/IHal_Reset.h"
#include "common/iservices/IReset.h"
#include "core/SystemContext.h"


namespace LoopMax {
    namespace Core {
        using namespace Services;
        using namespace Hal;

            class ResetManager : public IService, public IHal_Reset, public IReset {
                public:
                    
                    ResetManager();
                    void initContext(SystemContext& context) { ctx = &context; }

                    //IService:
                    void start() override;
                    void loop() override;
                    void stop() override;
                    const char* name() override { return "reset"; }
                    const  char* icon() override { return "🔄"; }
                    const std::vector<Types::SystemMode>& modes() override { return _modes; }
                    const std::vector<const char*>& dependencyNames() override { return _deps; }
                    Types::ServiceState& state() override { return currentState; }
                    const std::vector<WebCommand>& webCommands() const override { return _webCommands; } 
                    void registerEndpoints() override;


                    //IHal_Reset:
                    uint32_t pinNumber() const override { return _pinNum; }
                    PinMode pinMode() const override { return _pinMode; }
                    void Reset() override;

                    

                private:
                    Types::ServiceState currentState = Types::ServiceState::READY;
                    const std::vector<Types::SystemMode> _modes = { Types::SystemMode::AP, Types::SystemMode::LAN };
                    SystemContext* ctx = nullptr;
                    std::vector<const char*> _deps;
                    void checkStartUpReset();
                    void checkSLoopReset();
                    
                    bool resetTriggered = false;
                    unsigned long resetPressTime = 0;
                    uint32_t _pinNum = 33;
                    PinMode _pinMode = PinMode::InPullUp;
                    std::vector<WebCommand> _webCommands;
                    
                     

                };


    }
}


