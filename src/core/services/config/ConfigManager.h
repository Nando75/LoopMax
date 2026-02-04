
#pragma once
#include "BaseConfig.h"
#include "core/hal/hal.h"
#include "core/services/IService.h"
#include "common/iservices/IConfig.h"
#include "core/hal/interfaces/IHal_Config.h"
#include "common/iservices/IResetSink.h"


#include "core/services/time/TimeManager.h"
#include "core/services/logs/LogsManager.h"
#include "core/services/system/SystemManager.h"
#include "core/services/storage/StorageManager.h"
                       
#include <cstring>
#include <cstdlib>


namespace LoopMax {
    namespace Core {
            using namespace Services;
            using namespace Hal;
            using namespace Modules;

            class ConfigManager : public IService,  public IHal_Config, public IConfig  {
                public:
                    ConfigManager();
                    void initContext(Core::SystemContext& context) { ctx = &context; }
                        
                    //IService:
                    void start() override;
                    void loop() override;
                    void stop() override;
                    const char* name() override { return "conf"; }
                    const  char* icon() override { return "🛠"; }
                    const std::vector<Types::SystemMode>& modes() override { return _modes; }
                    const std::vector<const char*>& dependencyNames() override { return _deps; }
                    Types::ServiceState& state() override { return currentState; }
                    const std::vector<WebCommand>& webCommands() const override { return _webCommands; } 
                    void registerEndpoints() override;
                    
                    //IHal_Config
                    Settings& settings() override { return _settings; }
                    const Settings& settings() const override { return _settings; }
                    std::string&  getLanguage() override { return _settings.lng; }
                    
                    
                    
                    std::string buildTimeJson() override;
                    std::string buildSystemJson(bool isLogged=true) override;
                    std::string buildLogsJson() override;
                    std::string buildGeoJson() override;
                    std::string buildResetJson() override;
                    std::string buildWifiJson() override;
                    std::string buildWebJson() override;
                    std::string buildServicesJson() override;
                    std::string buildModulesJson() override;

                    //IConfig
                    bool isReady() const override;
                    bool ExistSavedConfig(IModuleData& moduleData) override;
                    void loadConfig(IModuleData& moduleData) override;
                    bool saveModuleConfig(IModuleData& moduleData) override;
                    bool removeConfig(const std::string& moduleName) override;

                    bool resetSystem();
                    std::string getKey() override { return _settings.key; }

                    //IResetSink
                    void registerSink(Services::IResetSink* sink) override;
                    

                private:
                 
                    Settings _settings;
                    SystemContext* ctx = nullptr;
                    int keyLen = 30;
                    std::vector<WebCommand> _webCommands;
                    const char* devFileName = "/loopmax_config.json";
                    const char* modFileName = "/loopmax_modules.json";

                    
                    StorageResult loadMainConfig();
                    void buildJsonConfig(std::string &outJson);
                    bool loadJsonConfigFile(const std::string &jsonData);
                    bool saveConfig();
                    

                    Types::ServiceState currentState = Types::ServiceState::READY;
                    const std::vector<Types::SystemMode> _modes = { Types::SystemMode::AP, Types::SystemMode::LAN };
                    
                    std::vector<const char*> _deps;
                    std::string buildModuleFile(IModuleData& moduleData);
                    
                    std::vector<Services::IResetSink*> sinks;

                };


    }
}


