#pragma once
#include "settings.h"
#include "modules/SmartiLabApi/SmartiLabApi.h"
#include "modules/PinSchedule/PinScheduleManager.h"


namespace LoopMax::Modules {

    using namespace Services;
    using namespace Types;
    
    

    class Zappy2 : public IModule {
    public:
        Zappy2();

        // IModule lifecycle
        void init(Services::ModuleContext* context) override;

        void begin() override;
        void loop() override;

        // IModule info
        std::string name()  override { return MODULE_NAME; };
        std::string icon()  override { return "🔌"; };

        bool enabled() const override { return true; };

        void registerEndpoints() override;
        void publishLogs(const char* logs) override;
        
        void onSystemReset() override;

        // Module data
        void initModuleData();
        //void registerEndpoints();
        Types::IModuleData& Data() override;

    private:
        void initPins();
        void printPins();
        bool haveMqttsData = false;
        bool setChannelFromClient(std::string channel, std::string value);
        bool setLineName(std::string channel, std::string name);
        
        

        void checkServer();
        void initMqtts();
        void processMqtts(const std::string& topic, const std::string& payload);

        void getJsonPins(std::string& out);
        // Core services
        ModuleContext* ctx = nullptr;
        void buildJsonPayload(std::string& out);
        bool loadJsonPayload();

        // Module metadata
        IModuleData moduleData;
        SmartiLabApi* smartiLabApi;
        PinScheduleManager* scheduler;

        //Debounce saving to flash ...
        unsigned long lastChangeMillis = 0;
        bool pendingSave = false;
        static const unsigned long SAVE_DEBOUNCE_MS = 30000; // 30 secondi
        void stopMqttService();
        

        

    };

} // namespace LoopMax::Modules
