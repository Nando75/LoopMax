
#pragma once
#include "core/hal/hal.h"
#include "core/services/IService.h"
#include "core/hal/interfaces/IHal_System.h"
#include "common/iservices/ISystem.h"
#include "core/SystemContext.h"


namespace LoopMax::Core {
    using namespace Hal;
    using namespace Services;

            class SystemManager : public IService,  public IHal_System, public ISystem {
                public:
                   void initContext(SystemContext& context) { ctx = &context; }

                    SystemManager();

                    //IService:
                    void start() override;
                    void loop() override;
                    void stop() override;
                    const char* name() override { return "system"; }
                    const  char* icon() override { return "🚀"; }
                    const std::vector<Types::SystemMode>& modes() override { return _modes; }
                    const std::vector<const char*>& dependencyNames() override { return _deps; }
                    Types::ServiceState& state() override { return currentState; }
                    const std::vector<WebCommand>& webCommands() const override { return _webCommands; } 
                    void registerEndpoints() override;    


                    //IHal_System
                     uint64_t uptime() override { return sys.uptime(); }
                     const char* ChipModel() override { return sys.ChipModel(); }
                     const uint8_t ChipCores() override { return sys.ChipCores(); }
                     const uint8_t ChipRevision() override { return sys.ChipRevision(); }
                     const uint32_t FlashSize() override { return sys.FlashSize(); }
                     const uint32_t FlashSpeed() override { return sys.FlashSpeed(); }
                     const uint32_t FreeHeap() override { return sys.FreeHeap(); }
                     const uint32_t HeapSize() override { return sys.FlashSize(); }
                     const uint32_t CpuFreq() override { return sys.CpuFreq(); }
                     const char* buildDate()  override {  return sys.buildDate(); }
                     const char* buildTime()  override {  return sys.buildTime(); }
                    
                     ///Types::ServiceState status() override { return Types::ServiceState::READY; }
                     Types::SystemMode mode() override { return currentMode; }
                     void setMode(Types::SystemMode mode) override { currentMode = mode;}

                     void restart() override { sys.restart(); }

                    const std::string Api() override { return _api; }
                    void setApi(std::string api) { _api = api; }

                    const std::string ApiVersion() override { return _apiVers; }
                    void setApiVersion(std::string version) { _apiVers = version; }


                     //ISystem
                     /*
                      virtual uint64_t uptime() = 0;
                    virtual const char* ChipModel() = 0;
                    virtual const uint8_t ChipCores() = 0;
                    virtual const uint8_t ChipRevision() = 0;
                    virtual const uint32_t FlashSize() = 0;
                    virtual const uint32_t FlashSpeed() = 0;
                    virtual const uint32_t FreeHeap() = 0;
                    virtual const uint32_t HeapSize() = 0;
                    virtual const uint32_t CpuFreq() = 0;
                     */


                     

                    

                private:
                    Types::SystemMode currentMode = Types::SystemMode::AP;
                    Types::ServiceState currentState = Types::ServiceState::READY;
                    const std::vector<Types::SystemMode> _modes = { Types::SystemMode::AP, Types::SystemMode::LAN };
                    IHal_System& sys;
                    std::vector<const char*> _deps;
                    IHal_Serial* _serial;
                    std::vector<WebCommand> _webCommands;
                    SystemContext* ctx = nullptr;
                    std::string _api;
                    std::string _apiVers;

                };
    
}


