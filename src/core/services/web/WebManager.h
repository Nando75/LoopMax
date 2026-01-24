
#pragma once
#include "core/hal/hal.h"
#include "core/services/IService.h"
#include "core/hal/interfaces/IHal_Web.h"
#include "common/iservices/IWeb.h"
#include "core/hal/interfaces/IHal_Timer.h"
#include "core/hal/interfaces/IHal_Logs.h"
#include "core/hal/interfaces/IHal_Serial.h"
#include "core/hal/interfaces/IHal_Config.h"
#include "core/hal/interfaces/IHal_Wifi.h"
#include "core/hal/interfaces/IHal_Storage.h"
#include "core/SystemContext.h"



namespace LoopMax::Core {
        using namespace Services;
        using namespace Hal;

            class WebManager : public IService,  public IHal_Web, public IWeb {
                public:
                    //using Handler = std::function<void(AsyncWebServerRequest*)>;

                    WebManager();
                    void initContext(SystemContext& context) { ctx = &context; }
                    //IService:
                    void start() override;
                    void loop() override;
                    void stop() override;
                    const char* name() override { return "web"; }
                    const  char* icon() override { return "🌐"; }
                    const std::vector<Types::SystemMode>& modes() override { return _modes; }
                    const std::vector<const char*>& dependencyNames() override { return _deps; }
                    Types::ServiceState& state() override { return currentState; }
                    const std::vector<WebCommand>& webCommands() const override { return _webCommands; } 
                    void registerEndpoints() override;

                    

                    //IHal_Web
                     void begin()  override;
                     void run() override;
                     void tick() override;
                     void end() override;
                     bool isAlive() override;
                     std::string Type() override { return _web.Type(); }

                     // 🔹 IMPLEMENTAZIONI
                    void on(const std::string& uri, int method, Handler handler) override {}
                    void serveStatic(const std::string& uri, const std::string& fsPath, const std::string& cache = "") override {} 
                    void onNotFound(Handler handler) override {}  

                    
                  

                    //Controllo login 
                    bool isLogged(LoopMax::Types::IHttpContext &httpCtx);

                private:
                    void initCoreRoutes();
                    void initModulesRoutes();
                    void initServicesRoutes();
                    

                    Types::ServiceState currentState = Types::ServiceState::READY;
                    const std::vector<Types::SystemMode> _modes = { Types::SystemMode::AP, Types::SystemMode::LAN };
                    std::vector<WebCommand> _webCommands;
                    Hal::IHal_Web& _web;
                    std::vector<const char*> _deps;
                    SystemContext* ctx = nullptr;
                    std::vector<StorageFile> _staticFiles;

                    //CheckLogin:
                    using HandlerType = std::function<void(LoopMax::Types::IHttpContext&)>;
                    HandlerType checkLogin(HandlerType handler);
                    
                    static constexpr size_t MAX_WEB_TOKENS = 20;
                    std::array<std::array<char, 11>, MAX_WEB_TOKENS> webTokens{};
                    size_t webTokenIndex = 0;
                    size_t webTokenCount = 0;


                    //std::vector<std::array<char, 11>> webTokens;


                    bool addWebToken(const char* token);
                    bool webTokenExists(const char* token);
                    bool removeWebToken(const char* token);
                    std::string findToken(LoopMax::Types::IHttpContext &httpCtx);
                    void logOut(LoopMax::Types::IHttpContext &httpCtx);


                    




                    
                };


    }



