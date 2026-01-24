
#pragma once
#include "core/hal/hal.h"
#include "core/services/IService.h"
#include "common/iservices/IMqtts.h"
#include "core/hal/interfaces/IHal_Mqtts.h"
#include "core/SystemContext.h"

namespace LoopMax {
    namespace Core {
        using namespace Hal;
        

            class MqttsManager : public IService, public IHal_Mqtts,  public IMqtts {
                public:
                    MqttsManager();
                    void initContext(SystemContext& context) { ctx = &context; }

                    //IService:
                    void start() override;
                    void loop() override;
                    void stop() override;
                    const char* name() override { return "mqtts"; }
                    const  char* icon() override { return "📭"; }
                    const std::vector<Types::SystemMode>& modes() override { return _modes; }
                    const std::vector<const char*>& dependencyNames() override { return _deps; }
                    Types::ServiceState& state() override { return currentState; }
                    const std::vector<WebCommand>& webCommands() const override { return _webCommands; } 
                    void registerEndpoints() override { }

                      void begin(Types::MqttsData& config) override { _mqtts.begin(config); }

                        bool connect() override { return _mqtts.connect(); }
                        void disconnect() override { _mqtts.disconnect(); }

                        bool publish(
                            const std::string& topic,
                            const std::string& payload,
                            bool retained = false
                        ) override { return _mqtts.publish(topic,payload,retained); }

                        bool subscribe(const std::string& topic) override { return _mqtts.subscribe(topic); }

                        void onMessage(Types::MqttCallback cb) override { _mqtts.onMessage(cb); }

                        void tick() override { _mqtts.tick(); }

                        bool isConnected() override { return _mqtts.isConnected(); }


                    
                    

                private:
                    Types::ServiceState currentState = Types::ServiceState::READY;
                    const std::vector<Types::SystemMode> _modes = { Types::SystemMode::LAN };
                    IHal_Mqtts& _mqtts;
                    std::vector<const char*> _deps;
                    SystemContext* ctx = nullptr;
                    std::vector<WebCommand> _webCommands;


                };


    }
}


