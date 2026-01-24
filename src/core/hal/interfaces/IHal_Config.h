#pragma once 
#include <cstdint>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>
#include <common/types.h>
#include "common/IModule.h"
namespace LoopMax::Core::Hal {
    
    using namespace Types;
    using namespace Modules;

                    struct Settings {
                        std::string name       = "LoopMax";
                        std::string apSsid     = "LoopMaxAP";
                        std::string apPsw      = "loopmaxap";
                        std::string FwVersion  = "1.0.0";
                        std::string Company    = "SmartiLab";
                        std::string CompanyUrl = "https://SmartiLab.it";
                        std::string ApiUrl     = "https://smartilab.it/api/devices.php";
                        SystemMode mode = SystemMode::AP;
                        std::string  key = "";
                        std::string wfSsid     = "";
                        std::string wfPsw      = "";
                        int64_t unix = 0;
                        std::string timezone   = "UTC";
                        int64_t tz_offset = 0;
                        double latitude = 0.0;
                        double longitude = 0.0;
                        bool hasModules = false;
                    };


               
    class IHal_Config {
        public:
            // Settings
            virtual Settings& settings() = 0;
            virtual const Settings& settings() const = 0;
            virtual std::string  buildTimeJson() = 0;
            virtual std::string  buildSystemJson(bool isLogged=true) = 0;
            virtual std::string buildLogsJson() = 0;
            virtual std::string buildGeoJson() = 0;
            virtual std::string buildResetJson() = 0;
            virtual std::string buildWifiJson() = 0;
            virtual std::string buildWebJson() = 0;
            virtual std::string buildServicesJson() = 0;
            virtual std::string buildModulesJson() = 0;




            //virtual void initContext(SystemContext* ctx) = 0;
            virtual ~IHal_Config() = default;
            
        };
    

}
