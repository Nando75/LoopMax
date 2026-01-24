#include "ard_pins.h"
#include "common/types.h"


using namespace LoopMax::Types;

namespace LoopMax::Core::Hal {

    bool ard_pins::isValidPin(int pin) const {
        return pin >= 0 && pin <= 39; // ESP32 classico
    }

        PinResult ard_pins::setMode(int pin, PinMode mode) {
            switch (mode) {
                case PinMode::Out:
                    ::pinMode(pin, OUTPUT);
                    break;

                case PinMode::In:
                    ::pinMode(pin, INPUT);
                    break;

                case PinMode::InPullUp:
                    ::pinMode(pin, INPUT_PULLUP);
                    break;

                default:
                    return PinResult::UNSUPPORTED_MODE;
            }
            return PinResult::OK;
        }


        PinResult ard_pins::write(int pin, PinLevel level) {
            ::digitalWrite(pin, level == PinLevel::High ? HIGH : LOW);
            return PinResult::OK;
        }

        PinResult ard_pins::read(int pin, PinLevel& level) {
            level = ::digitalRead(pin) ? PinLevel::High : PinLevel::Low;
            return PinResult::OK;
        }


    PinResult ard_pins::setupPwm(int pin, const PwmConfig& cfg) {
        ledcSetup(cfg.channel, cfg.frequency, cfg.resolution);
        ledcAttachPin(pin, cfg.channel);
        return PinResult::OK;
    }

    PinResult ard_pins::writePwm(int pin, uint32_t duty) {
        // pin → canale dovrebbe essere mappato (lo vediamo dopo)
        ledcWrite(0, duty);
        return PinResult::OK;
    }

    bool ard_pins::supportsPwm(int pin) const {
        return true; // su ESP32 quasi tutti
    }

}
