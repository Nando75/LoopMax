#pragma once
#include "core/hal/interfaces/IHal_Pins.h"

extern "C" {
    #include "driver/gpio.h"
    #include "driver/ledc.h"
}

namespace LoopMax::Core::Hal {

    class idf_pins : public IHal_Pins {
    public:
        PinResult setMode(int pin, PinMode mode) override;
        PinResult write(int pin, PinLevel level) override;
        PinResult read(int pin, PinLevel& level) override;

        PinResult setupPwm(int pin, const PwmConfig& cfg) override;
        PinResult writePwm(int pin, uint32_t duty) override;

        bool isValidPin(int pin) const override;
        bool supportsPwm(int pin) const override;
    };

}
