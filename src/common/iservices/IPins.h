
#pragma once
#include "common/types.h"
#include <cstddef>  
#include <cstdint>  
#include "common/types.h"

using namespace LoopMax::Types;

namespace LoopMax::Services {

        class IPins {
        public:
        
        // --- configurazione ---
        virtual PinResult setMode(int pin, PinMode mode) = 0;

        // --- digital ---
        virtual PinResult write(int pin, PinLevel level) = 0;
        virtual PinResult read(int pin, PinLevel& level) = 0;

        // --- PWM ---
        virtual PinResult setupPwm(int pin, const PwmConfig& cfg) = 0;
        virtual PinResult writePwm(int pin, uint32_t duty) = 0;

        // --- utility ---
        virtual bool isValidPin(int pin) const = 0;
        virtual bool supportsPwm(int pin) const = 0;
            

            virtual ~IPins() {}
        };

}
 