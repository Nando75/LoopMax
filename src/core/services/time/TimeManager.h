
#pragma once
#include "core/hal/hal.h"
#include "core/services/IService.h"
#include "core/hal/interfaces/IHal_Timer.h"
#include "common/iservices/ITimer.h"
#include "core/SystemContext.h"


namespace LoopMax {
    namespace Core {
        using namespace Services;
        using namespace Hal;
        struct SystemContext;

            class TimeManager : public IService,  public IHal_Timer, public ITimer {
                public:
                    TimeManager();
                    void initContext(Core::SystemContext& context) { ctx = &context; }
                    //IService:
                    void start() override;
                    void loop() override;
                    void stop() override;
                    const char* name() override { return "timer"; }
                    const  char* icon() override { return "🕥"; }
                    const std::vector<Types::SystemMode>& modes() override { return _modes; }
                    const std::vector<const char*>& dependencyNames() override { return _deps; }
                    Types::ServiceState& state() override { return currentState; }
                    const std::vector<WebCommand>& webCommands() const override { return _webCommands; } 
                    void registerEndpoints() override;

                    //ITimer
                    uint64_t millis() override; // restituisce millisecondi
                    uint64_t micros() override; // opzionale se ti serve più precisione
                    
                    int64_t Unix() override;
                    std::string Timezone() override;
                    int64_t Tz_Offset() override;

                    void setUnix(int64_t unix) override;
                    void setTimezone(const std::string& tz) override;
                    void setTzOffset(int64_t offset) override;
                    const std::string ntpServer() const override { return _timer.ntpServer(); }
                    const bool isSyncronized() const override { return _timer.isSyncronized(); };
                    void checkInternetTime() override { _timer.checkInternetTime(); };
                    
                    

                private:
                    bool setTimeFromClient(std::string unix, std::string timezone, std::string offset);
                    Types::ServiceState currentState = Types::ServiceState::READY;
                    const std::vector<Types::SystemMode> _modes = { Types::SystemMode::AP, Types::SystemMode::LAN };
                    Hal::IHal_Timer& _timer;
                    std::vector<const char*> _deps;
                    std::vector<WebCommand> _webCommands;
                    Core::SystemContext* ctx = nullptr;
                    

                };


    }
}


