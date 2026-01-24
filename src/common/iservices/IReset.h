
#pragma once
#include "common/types.h"
#include <cstddef>  
#include <cstdint>  
#include "common/types.h"

using namespace LoopMax::Types;

namespace LoopMax::Services {

        class IReset {
        public:
        
            virtual uint32_t pinNumber() const = 0;
            virtual PinMode pinMode() const = 0;
            virtual void Reset() = 0;
            

            virtual ~IReset() {}
        };

}
 