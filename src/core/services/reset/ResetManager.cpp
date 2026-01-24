#include <Arduino.h>
#include "ResetManager.h"
#include "BaseConfig.h"
#include "core/services/serial/SerialManager.h"
#include "core/services/pins/PinsManager.h"
#include "core/services/logs/LogsManager.h"
#include "core/services/system/SystemManager.h"
#include "core/services/time/TimeManager.h"




namespace LoopMax::Core {

                ResetManager::ResetManager()
                    : _deps{"timer","logs","pins","system"}
                {}
                //IService
                void ResetManager::start() {
                    _pinNum = PIN_RESET;
                    
                     ctx->pins.setMode(_pinNum,_pinMode);
                    this->resetPressTime = 0;
                    this->resetTriggered = false;

                     checkStartUpReset();
                     ctx->logs.write("Reset Ready",LoopMax::Types::LogType::INFO,this->name(), this->icon());
                }
               
                void ResetManager::stop() { 
                    currentState = Types::ServiceState::STOPPED;
                 }
                
                 void ResetManager::checkStartUpReset() {
                    const unsigned long debounceDelay = 50;
                    unsigned long startTime = ctx->time.millis();
                    bool stableLow = false;
                    while (ctx->time.millis() - startTime < 100) {
                        if (digitalRead(PIN_RESET) == LOW) {
                            delay(debounceDelay);
                            if (digitalRead(PIN_RESET) == LOW) {
                                stableLow = true;
                                break;
                            }
                        }
                    }

                    if (stableLow) Reset();

                    
                }

                void ResetManager::loop() { 
                        // 1. CONTROLLO DI STATO (Gestito dal ServicesManager)
                        // Se il manager mi spegne, esco immediatamente.
                        if (currentState != Types::ServiceState::READY) return;

                        // 2. CONTROLLO DI FREQUENZA (Gestito dal Servizio)
                        // Anche se sono READY, non serve controllare il bottone fisico 
                        // 1 milione di volte al secondo. 20 volte al secondo (50ms) bastano.
                        static uint32_t lastCheck = 0;
                        if (millis() - lastCheck < 50) return; 
                        lastCheck = millis();

                        // 3. LOGICA REALE
                        checkSLoopReset();
                 }




                 void ResetManager::checkSLoopReset() {

                    static bool lastState = HIGH;
                        static unsigned long lastDebounce = 0;
                        const unsigned long debounceDelay = 50;
                        bool reading = digitalRead(PIN_RESET);
                        if (reading != lastState) lastDebounce = ctx->time.millis();
                        if (ctx->time.millis() - lastDebounce > debounceDelay) {
                            // Bottone premuto
                            if (reading == LOW && !resetTriggered) {
                                if (resetPressTime == 0) {
                                    resetPressTime = ctx->time.millis();
                                }
                                else if ((ctx->time.millis() - resetPressTime) >= RESET_HOLD_TIME) {
                                    resetTriggered = true;
                                    resetPressTime = 0;
                                    Reset();
                                }
                            }
                            // Button relesed
                            else if (reading == HIGH) {
                                resetPressTime = 0;
                                resetTriggered = false;
                            }
                        }
                        lastState = reading;
                    
                    
                }




                  void ResetManager::Reset() { 
                    
                    ctx->logs.write("System reset ...",LoopMax::Types::LogType::INFO,this->name(), this->icon());
                    ctx->system.setMode(SystemMode::RESET);
                                    
                 }


                 void ResetManager::registerEndpoints() { 

                    _webCommands.push_back({
                        "core/reset",
                        0x01, // GET
                        [this](IHttpContext& httpCtx){
                            
                           
                           ctx->serial.printLn("SERVIZIO COMANDO RICEVUTO");
                           

                            // Esempio lettura parametri: http://ip/zappy/setlng?lang=it
                            if(httpCtx.hasParam("lang")) {
                                std::string lang = httpCtx.getParam("lang");
                                // Salva lingua...
                            }
                            
                            httpCtx.send(200, "text/plain", "OK");
                        }
                    });
                }


}