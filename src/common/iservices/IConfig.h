#pragma once 
#include "common/Types.h"
#include "common/iservices/IResetSink.h"
#include <cstddef>

namespace LoopMax::Services {
    using namespace LoopMax::Types;

    class IConfig {
        public:

        virtual bool isReady() const = 0;
        virtual bool ExistSavedConfig(const std::string& moduleName) = 0;
        virtual void loadConfig(const std::string& moduleName,std::vector<modulePin>& pins,std::string& JsonConfig) = 0;
        virtual bool saveModuleConfig(const std::string& moduleName,const std::vector<modulePin>& pins,const std::string& JsonConfig) = 0;
        virtual bool removeConfig(const std::string& moduleName) = 0;
        virtual std::string getKey() = 0;

        virtual  void registerSink(Services::IResetSink* sink) = 0;
        
        virtual ~IConfig() {}
    };

}
