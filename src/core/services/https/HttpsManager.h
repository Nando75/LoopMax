
#pragma once
#include "core/hal/hal.h"
#include "core/services/IService.h"
#include "common/iservices/IHttps.h"
#include "core/hal/interfaces/IHal_Https.h"
#include "core/SystemContext.h"

namespace LoopMax {
    namespace Core {
        using namespace Hal;
        

            class HttpsManager : public IService, public IHal_Https,  public IHttps {
                public:
                    HttpsManager();
                    void initContext(SystemContext& context) { ctx = &context; }

                    //IService:
                    void start() override;
                    void loop() override;
                    void stop() override;
                    const char* name() override { return "https"; }
                    const  char* icon() override { return "💬"; }
                    const std::vector<Types::SystemMode>& modes() override { return _modes; }
                    const std::vector<const char*>& dependencyNames() override { return _deps; }
                    Types::ServiceState& state() override { return currentState; }
                    const std::vector<WebCommand>& webCommands() const override { return _webCommands; } 
                    void registerEndpoints() override { }


                    //IHalHttps /IHttps
                    Types::HttpsResponse request(Types::HttpsData httpsData, const std::string& body,Types::WMethod method,const std::string& contentType) override;
                    bool downloadFile(Types::HttpsData httpsData,
                                        const std::string& body,
                                        Types::WMethod method,
                                        const std::string& contentType,
                                        std::function<bool(uint8_t*, size_t)> onChunk) override;

                    

                private:
                    Types::ServiceState currentState = Types::ServiceState::READY;
                    const std::vector<Types::SystemMode> _modes = { Types::SystemMode::LAN };
                    IHal_Https& _https;
                    std::vector<const char*> _deps;
                    SystemContext* ctx = nullptr;
                    std::vector<WebCommand> _webCommands;


                };


    }
}


