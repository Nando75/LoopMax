
#pragma once
#include "core/hal/hal.h"
#include "core/services/IService.h"
#include "core/hal/interfaces/IHal_Pins.h"
#include "common/iservices/IPins.h"
#include "core/SystemContext.h"



namespace LoopMax {
    namespace Core {
        using namespace Hal;
        using LoopMax::Services::IPins;
        

            class PinsManager : public IService,  public IHal_Pins, public IPins {
                public:
                    

                    PinsManager();
                    void initContext(SystemContext& context) { ctx = &context; }

                    //IService:
                    void start() override;
                    void loop() override;
                    void stop() override;
                    const char* name() override { return "pins"; }
                    const  char* icon() override { return "📍"; }
                    const std::vector<Types::SystemMode>& modes() override { return _modes; }
                    const std::vector<const char*>& dependencyNames() override { return _deps; }
                    Types::ServiceState& state() override { return currentState; }
                    const std::vector<WebCommand>& webCommands() const override { return _webCommands; } 
                    void registerEndpoints() override {}

                    //IPins
                    PinResult setMode(int pin, PinMode mode) override;
                    // --- digital ---
                    PinResult write(int pin, PinLevel level)  override;
                    PinResult read(int pin, PinLevel& level)  override;
                    // --- PWM ---
                    PinResult setupPwm(int pin, const PwmConfig& cfg)  override;
                    PinResult writePwm(int pin, uint32_t duty)  override;
                    // --- utility ---
                    bool isValidPin(int pin) const  override;
                    bool supportsPwm(int pin) const  override;
                    
                    

                private:
                    Types::ServiceState currentState = Types::ServiceState::READY;
                    const std::vector<Types::SystemMode> _modes = { Types::SystemMode::AP, Types::SystemMode::LAN };
                    IHal_Pins& _pins;
                    std::vector<const char*> _deps;
                    SystemContext* ctx = nullptr;
                    std::vector<WebCommand> _webCommands;

                };


    }
}


