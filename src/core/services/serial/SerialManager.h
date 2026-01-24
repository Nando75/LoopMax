
#pragma once
#include "core/hal/hal.h"
#include "core/services/IService.h"
#include "core/hal/interfaces/IHal_Serial.h"
#include "core/hal/interfaces/IHal_Timer.h"
#include "core/hal/interfaces/IHal_Logs.h"
#include "common/iservices/ILogSink.h"
#include "common/iservices/ISerial.h"
#include "core/SystemContext.h"



namespace LoopMax {
    namespace Core {
    using namespace Services;
    using namespace Hal;

            class SerialManager : public IService,  public IHal_Serial, public Services::ILogSink, public ISerial {
                public:
                    //const Serviceinfo& info() const override { return class_info; }
                   SerialManager();
                    void initContext(SystemContext& context) { ctx = &context; }
                    //IService:
                    void start() override;
                    void loop() override;
                    void stop() override;
                    const char* name() override { return "serial"; }
                    const  char* icon() override { return "💻"; }
                    
                    const std::vector<Types::SystemMode>& modes() override { return _modes; }
                    /*
                    const std::vector<IService*>& dependency() override {
                            static const std::vector<IService*> _empty;
                            return _empty;
                        }
                    */
                    //const std::vector<IService*>& dependency() override { return _deps; }
                    const std::vector<const char*>& dependencyNames() override { return _deps; }

                    Types::ServiceState& state() override { return currentState; }
                    const std::vector<WebCommand>& webCommands() const override { return _webCommands; } 
                    void registerEndpoints() override {}

                    //bool registry() override;

                    //ISerial
                     bool begin(int baud) override { return true;}
                     void printLn(std::string value) override;
                     void printLn(unsigned long value) override;
                     void printLn(const char* str) override;
                     void printLn(char c) override;
                     void printLn(int value) override;
                     void print(unsigned long value) override;
                     void print(const char* str) override;
                     void print(char c) override;
                     void print(int value) override;

                     bool available() override;
                     int  read() override;
                     void write(const uint8_t* data, size_t len) override;
                     void write(char c) override;
                     void flush() override;


                     //ILogSink
                     void publishLogs(const char* logs) override;
                    

                private:
                    Types::ServiceState currentState = Types::ServiceState::READY;
                    const std::vector<Types::SystemMode> _modes = { Types::SystemMode::AP, Types::SystemMode::LAN };
                    bool serialIsActive = false;
                    IHal_Serial& serial;
                    uint64_t lastRetry = 0;
                    const uint64_t retryInterval = 60000; // 1 minuto
                    void checkSerial();
                    uint64_t baud = HAL_MONITOR_SPEED;
                    std::vector<const char*> _deps;
                    std::vector<WebCommand> _webCommands;
                    SystemContext* ctx = nullptr;

                };


    }
}


