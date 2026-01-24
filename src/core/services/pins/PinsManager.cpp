#include "PinsManager.h"
#include "BaseConfig.h"
#include "core/services/logs/LogsManager.h"


namespace LoopMax::Core {

                PinsManager::PinsManager()
                    : _pins(Hal::pins()), _deps{"timer","logs"}
                {}
                
                //IService
                void PinsManager::start() {
                    ctx->logs.write("Pins ready",LoopMax::Types::LogType::INFO,this->name(), this->icon());
                }

                void PinsManager::loop() {  }
                void PinsManager::stop() { 
                    currentState = Types::ServiceState::STOPPED;
                 }
               
                 //IPins
                 /*
                PinResult PinsManager::setMode(int pin, PinMode mode) {
                if (!_pins.isValidPin(pin)) return PinResult::INVALID_PIN;
                    return _pins.setMode(pin, mode);

                    std::string msg = "Pin " + std::String(pin)


                    ctx->logs.write("Pins ready",LoopMax::Types::LogType::INFO,this->name(), this->icon());
                    
                }
                */
               PinResult PinsManager::setMode(int pin, PinMode mode) {
                    if (!_pins.isValidPin(pin)) return PinResult::INVALID_PIN;
                    PinResult result = _pins.setMode(pin, mode);
                    if(IS_DEBUG)
                    {
                        const char* modeStr = PinModeToStr(mode);
                        std::string msg = "Set mode on pin " + std::to_string(pin) + " → " + modeStr;
                        ctx->logs.write(msg, LoopMax::Types::LogType::DEBUG, this->name(), this->icon());
                    }
                    return result;
                }






                PinResult PinsManager::write(int pin, PinLevel level) {
                    if (!_pins.isValidPin(pin)) return PinResult::INVALID_PIN;
                    if(IS_DEBUG)
                    {
                        const int lv = PinLevelToInt(level);
                        std::string msg = "Pin " + std::to_string(pin) + " level → " + std::to_string(lv);
                        ctx->logs.write(msg, LoopMax::Types::LogType::DEBUG, this->name(), this->icon());
                    }
                    return _pins.write(pin, level);
                }

                PinResult PinsManager::read(int pin, PinLevel& level) {
                    if (!_pins.isValidPin(pin)) return PinResult::INVALID_PIN;
                    return _pins.read(pin, level);
                }

                PinResult PinsManager::setupPwm(int pin, const PwmConfig& cfg) {
                    if (!_pins.supportsPwm(pin)) return PinResult::UNSUPPORTED_OPERATION;
                    return _pins.setupPwm(pin, cfg);
                }

                PinResult PinsManager::writePwm(int pin, uint32_t duty) {
                    if (!_pins.supportsPwm(pin)) return PinResult::UNSUPPORTED_OPERATION;
                    return _pins.writePwm(pin, duty);
                }

                bool PinsManager::isValidPin(int pin) const {
                    return _pins.isValidPin(pin);
                }

                bool PinsManager::supportsPwm(int pin) const {
                    return _pins.supportsPwm(pin);
                }



}