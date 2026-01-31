#pragma once

#include <string>
#include "types.h"
#include "types_web.h"
#include "common/iservices/ITimer.h"
#include "common/iservices/ILogs.h"
#include "common/iservices/ISerial.h"
#include "common/iservices/ISystem.h"
#include "common/iservices/IPins.h"
#include "common/iservices/IConfig.h"
#include "common/iservices/IWifi.h"
#include "common/iservices/ILogSink.h"
#include "common/iservices/IResetSink.h"

#include "common/iservices/IWeb.h"

namespace LoopMax::Services { struct ModuleContext; }




namespace LoopMax::Types {

    struct IModuleData {
        std::string FwVersion = "";
        std::string Name = "";
        std::string DeviceName = "";
        
        std::string Icon = "🧩";
        std::string Code = "";
        std::string Label = "";
        std::string Company = "";
        std::string Url = "";
        
        HttpsData httpsData;
        MqttsData mqttsData;

        bool SaveConfig = false;
        std::vector<modulePin> pins;
        std::string JsonConfig = "";

        std::string JsUIClass = "";

        std::vector<WebCommand> webCommands;

    };
}            



namespace LoopMax::Modules {
    using namespace Services;

           class IModule : public Services::ILogSink, public Services::IResetSink {
                public:
                    virtual void init(Services::ModuleContext* LoopMaxCore) = 0;
                    

                    virtual void begin() = 0;
                    virtual void loop() = 0;

                    virtual  std::string name() = 0;
                    virtual  std::string icon() = 0;
                    virtual bool enabled() const = 0;

                    //virtual const Types::IModuleData& Data() const = 0;
                    virtual Types::IModuleData& Data() = 0;

                    virtual void registerEndpoints() = 0;
                    virtual void publishLogs(const char* logs) override = 0;
                    virtual void onSystemReset() override = 0;

                    virtual ~IModule() = default;
                };


}