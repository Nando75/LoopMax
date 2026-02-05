#pragma once 
#include "common/Types.h"
#include "common/iservices/IResetSink.h"

#include <cstddef>

namespace LoopMax::Services {
    using namespace LoopMax::Types;
    
    class IConfig {
        public:

        virtual bool isReady() const = 0;
        virtual bool ExistSavedConfig(IModuleData& moduleData) = 0;
        virtual void loadConfig(IModuleData& moduleData) = 0;
        virtual bool saveModuleConfig(IModuleData& moduleData) = 0;
        virtual bool removeConfig(const std::string& moduleName) = 0;
        virtual std::string getKey() = 0;
        virtual std::string&  getLanguage() = 0;

        virtual std::string  buildTimeJson() = 0;
        virtual std::string  buildSystemJson(bool isLogged=true) = 0;
        virtual std::string buildLogsJson() = 0;
        virtual std::string buildGeoJson() = 0;
        virtual std::string buildResetJson() = 0;
        virtual std::string buildWifiJson() = 0;
        virtual std::string buildWebJson() = 0;
        virtual std::string buildServicesJson() = 0;
        virtual std::string buildModulesJson() = 0;

        virtual  void registerSink(Services::IResetSink* sink) = 0;
        
        virtual ~IConfig() {}
    };

}
