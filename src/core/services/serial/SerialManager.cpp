#include "SerialManager.h"
#include "BaseConfig.h"
#include "core/services/time/TimeManager.h"
#include "core/services/logs/LogsManager.h"



namespace LoopMax::Core {

              #include <type_traits>

                SerialManager::SerialManager()
                    : serial(Hal::serial()),
                     _deps{"timer","logs"}
                {}
                   



                //IService
                void SerialManager::start() {
                    if(!IS_DEBUG)
                    {   
                         currentState = Types::ServiceState::STOPPED;
                         ctx->logs.write("Serial stopped (release mode)",LoopMax::Types::LogType::INFO,this->name(), this->icon());
                        return;
                        
                    }

                    serialIsActive = serial.begin(baud);
                    if (serialIsActive)
                    {
                        currentState = Types::ServiceState::READY;
                        ctx->logs.registerSink(this);
                        ctx->logs.write("Serial Ready",LoopMax::Types::LogType::INFO,this->name(), this->icon());
                    }

                }




                void SerialManager::loop() { checkSerial(); }
                void SerialManager::stop() { currentState = Types::ServiceState::STOPPED; }


                 //ISerial
                void SerialManager::printLn(std::string value){ serial.printLn(value.c_str()); }
                void SerialManager::printLn(unsigned long value) { serial.printLn(value); }
                void SerialManager::printLn(const char* str){ serial.printLn(str); }
                void SerialManager::printLn(char c){ serial.printLn(c); }
                void SerialManager::printLn(int value) { serial.printLn(value); }

                void SerialManager::print(unsigned long value) { serial.print(value); }
                void SerialManager::print(const char* str){ serial.print(str); }
                void SerialManager::print(char c){ serial.print(c); }
                void SerialManager::print(int value) { serial.print(value); }

                bool SerialManager::available() { return serial.available(); }
                int  SerialManager::read() { return serial.read(); }
                void SerialManager::write(const uint8_t* data, size_t len) { serial.write(data,len); }
                void SerialManager::write(char c) { serial.write(c); }
                void SerialManager::flush() { serial.flush(); }


                //ILogSink
                void SerialManager::publishLogs(const char* logs)
                {
                    serial.printLn(logs);
                }
                

                void SerialManager::checkSerial()
                {
                    if (!IS_DEBUG) return;
                    if (serialIsActive) return;
                    uint64_t now = ctx->time.micros() / 1000ULL;
                    if (now - lastRetry < retryInterval) return;
                    lastRetry = now;
                    // Tentativo di riattivazione
                    serialIsActive = serial.begin(baud);
                    if(serialIsActive) 
                    {
                        currentState = Types::ServiceState::READY;
                        ctx->logs.registerSink(this);
                        ctx->logs.write("Serial restored");
                    } 
                }




}

