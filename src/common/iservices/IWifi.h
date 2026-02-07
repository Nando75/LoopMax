#pragma once 
#include <string>
#include "common/Types.h"

namespace LoopMax::Services {
    //using namespace LoopMax::Types;

    class IWifi {
        public:
            virtual bool isConnected() const = 0;
            virtual std::string getPublicIP()  = 0;
            virtual std::string Isp()  = 0;
            virtual bool internet() = 0;
            virtual std::string MacAddress() = 0;
                   
        virtual ~IWifi() {}
    };

}
