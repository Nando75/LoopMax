#pragma once
#include <vector>
#include "common/types.h"

namespace LoopMax {
    namespace Core {
    using namespace LoopMax::Types;
       class IService {
            public:
                virtual void start() = 0;
                virtual void loop() = 0;
                virtual void stop() = 0;
                
                virtual const char* name() = 0;
                virtual const char* icon() = 0;
                
                //virtual const std::vector<IService*>& dependency() = 0;
                virtual const std::vector<const char*>& dependencyNames() = 0;


                virtual const std::vector<Types::SystemMode>& modes() = 0;
                virtual Types::ServiceState& state()=0;
                
                virtual const std::vector<LoopMax::Types::WebCommand>& webCommands() const = 0;
                virtual void registerEndpoints() = 0;
                
                virtual ~IService() = default;
            private:
                

            };

    }
 }