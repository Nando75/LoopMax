
#pragma once
#include "core/hal/hal.h"
#include "core/services/IService.h"
#include "core/hal/interfaces/IHal_Logs.h"
#include "common/iservices/ILogSink.h"
#include "common/iservices/ILogs.h"
#include "core/SystemContext.h"

namespace LoopMax::Core {
    using namespace Services;
    using namespace Hal;

            class LogsManager : public IService,  public IHal_Logs, public ILogs {
                public:
                    LogsManager();
                    void initContext(SystemContext& context) { ctx = &context; }
                    //IService:
                    void start() override;
                    void loop() override;
                    void stop() override;
                    const char* name() override { return "logs"; }
                    const  char* icon() override { return "✏️"; }
                    const std::vector<Types::SystemMode>& modes() override { return _modes; }
                    const std::vector<const char*>& dependencyNames() override { return _deps; }
                    Types::ServiceState& state() override { return currentState; }
                    const std::vector<WebCommand>& webCommands() const override { return _webCommands; } 
                    void registerEndpoints() override;

                    //bool registry() override;

                    //ILogs
                    void write(const char* msg, Types::LogType type = Types::LogType::INFO,
                                const char* source = "logs", const char* sourceIcon = "✏️", const char* payload = "{}") override;

                    void write(std::string msg, Types::LogType type = Types::LogType::INFO,
                                std::string source = "logs", std::string sourceIcon = "✏️", std::string payload = "{}") override 
                                            { this->write(msg.c_str(),type,source.c_str(),sourceIcon.c_str(),payload.c_str()); }

                    std::vector<Types::LogTypeInfo> logTypes() override;
                    //std::vector<LogEntry>& getLogs() const  override;
                    const std::vector<LogEntry>& getLogs() const override;

                    //ILogsSink
                    void registerSink(Services::ILogSink* sink) override;
                    
                    std::mutex& getMutex() const override { return _logsMutex; }

                private:
    
                    char _logBuffer[4096]; // Pre-allocato all'avvio, non fallisce mai
                    std::vector<LogEntry> _logsHistory;
                    mutable std::mutex _logsMutex;
                    size_t _logIndex = 0;
                    bool _logsWrapped = false;
                    size_t _logsSize = 100; // o quello che vuoi



                    //void initHistArray();
                    //void saveLog(char* json);
                    void saveLog (const char* message,const char* logType,const char* logIcon,
                                            const char* source,const char* sourceIcon,uint64_t millis,const char* payload);
                    void resetHist();
                    Types::ServiceState currentState = Types::ServiceState::READY;
                    const std::vector<Types::SystemMode> _modes = { Types::SystemMode::AP, Types::SystemMode::LAN };
                    std::vector<const char*> _deps;
                    std::vector<Services::ILogSink*> sinks;
                    void logToJson(char* out, size_t outSize,const char* message,const char* logType,
                                        const char* logIcon,const char* source,const char* sourceIcon,uint64_t millis, const char* payload = nullptr);
                    std::vector<WebCommand> _webCommands;
                    SystemContext* ctx = nullptr;

                };


}



