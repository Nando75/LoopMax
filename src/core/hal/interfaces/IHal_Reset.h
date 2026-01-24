#pragma once 
#include <cstdint>
#include "common/types.h"

namespace LoopMax::Core::Hal {
    using namespace LoopMax::Types;

    class IHal_Reset {
    public:
        

        virtual uint32_t pinNumber() const = 0;
        virtual PinMode pinMode() const = 0;
        virtual void Reset() = 0;
        
        
        //virtual void initContext(SystemContext* ctx) = 0;

        virtual ~IHal_Reset() {}
    };

}
