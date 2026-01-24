#pragma once 
#include "common/Types.h"
#include <cstddef>
#include <vector>
#include <common/types.h>
#include <ESPAsyncWebServer.h>

namespace LoopMax::Services {
    //using namespace LoopMax::Types;

    class IWeb {
        public:

         //using Handler = std::function<void(AsyncWebServerRequest*)>;
         //virtual void registerEndpoint(const std::string& uri,WebRequestMethod method,Handler handler) = 0;
            


            


        
        virtual ~IWeb() {}
    };

}
