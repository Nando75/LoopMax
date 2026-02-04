#include "LogsManager.h"
#include "BaseConfig.h"
#include "core/services/time/TimeManager.h"
#include "core/services/config/ConfigManager.h"


namespace LoopMax::Core {

                LogsManager::LogsManager()
                    : 
                    _deps{"timer"},
                    _logIndex(0),
                    _logsWrapped(false),
                    _logsSize(0)
                {
                }


                //IService
              void LogsManager::start() {
                _logsSize = IS_DEBUG ? DEBUG_MAX_LOGS : RELEASE_MAX_LOGS;
                _logsHistory.clear();
                _logsHistory.resize(_logsSize);
                _logIndex = 0;
                _logsWrapped = false;

                this->write("Time Ready",Types::LogType::INFO,this->name(),this->icon());
                this->write("Logs Ready",Types::LogType::INFO,this->name(),this->icon());

            }


                void LogsManager::loop() { }
                void LogsManager::stop() { }

/*
                void LogsManager::write(const char* msg, Types::LogType type, const char* source, const char* sourceIcon, const char* payload) {
                    if (!IS_DEBUG && type == Types::LogType::DEBUG) return;
                    if (!msg) return; // Sicurezza extra

                    std::lock_guard<std::mutex> lock(_logsMutex); 

                    // 1. Salva nel vettore (già pronto per WebJson)
                    this->saveLog(msg, LogTypeToStr(type), LogTypeToIcon(type), source, sourceIcon, ctx->time.micros() / 1000ULL, payload);

                    // 2. Genera per i sink usando il buffer statico
                    // Niente malloc, niente rischi di NULL pointer
                    logToJson(_logBuffer, sizeof(_logBuffer), msg, 
                            LogTypeToStr(type), LogTypeToIcon(type), 
                            source, sourceIcon, 
                            ctx->time.micros() / 1000ULL, 
                            payload ? payload : "{}");

                    for (auto* sink : sinks) {
                        if (sink) sink->publishLogs(_logBuffer);
                    }
                }
*/

/*
                void LogsManager::write(const char* msg,
                                        Types::LogType type,
                                        const char* source,
                                        const char* sourceIcon,
                                        const char* payload) 
                {
                    if (!IS_DEBUG && type == Types::LogType::DEBUG) return;
                    if (!msg) return;

                    std::lock_guard<std::mutex> lock(_logsMutex);

                    // Normalizza payload
                    const char* safePayload = (payload && payload[0] != '\0') ? payload : "";

                    // 1. Salva nel vettore
                    this->saveLog(msg,
                                LogTypeToStr(type),
                                LogTypeToIcon(type),
                                source,
                                sourceIcon,
                                ctx->time.micros() / 1000ULL,
                                safePayload);

                    // 2. Genera JSON per i sink
                    logToJson(_logBuffer, sizeof(_logBuffer),
                            msg,
                            LogTypeToStr(type),
                            LogTypeToIcon(type),
                            source,
                            sourceIcon,
                            ctx->time.micros() / 1000ULL,
                            safePayload);

                    for (auto* sink : sinks) {
                        if (sink) sink->publishLogs(_logBuffer);
                    }
                }
*/



void LogsManager::write(const char* msg,
                        Types::LogType type,
                        const char* source,
                        const char* sourceIcon,
                        const char* payload) {
    if (!IS_DEBUG && type == Types::LogType::DEBUG) return;
    if (!msg) return;

    std::lock_guard<std::mutex> lock(_logsMutex);

    const char* safePayload = payload ? payload : "{}";

    saveLog(msg,
            LogTypeToStr(type),
            LogTypeToIcon(type),
            source,
            sourceIcon,
            ctx->time.micros() / 1000ULL,
            safePayload);

    logToJson(_logBuffer, sizeof(_logBuffer),
              msg,
              LogTypeToStr(type),
              LogTypeToIcon(type),
              source,
              sourceIcon,
              ctx->time.micros() / 1000ULL,
              safePayload);

    for (auto* sink : sinks) {
        if (sink) sink->publishLogs(_logBuffer);
    }
}






                
                //ILogSink
                 void LogsManager::registerSink(Services::ILogSink* sink) {
                    sinks.push_back(sink);
                }


               void LogsManager::saveLog(const char* message,const char* logType,const char* logIcon,
                                            const char* source,const char* sourceIcon,uint64_t millis,const char* payload) {
                            if (_logsHistory.empty()) return;

                        if (_logIndex >= _logsHistory.size()) {
                            _logIndex = 0;
                            _logsWrapped = true;
                        }

                        LogEntry& e = _logsHistory[_logIndex];
                        e.millis     = millis;
                        e.logType    = logType ? logType : "";
                        e.logIcon    = logIcon ? logIcon : "";
                        e.message    = message ? message : "";
                        e.source     = source ? source : "";
                        e.sourceIcon = sourceIcon ? sourceIcon : "";
                        e.payload    = payload ? payload : "{}";

                        _logIndex++;


                       // Serial.printf("LOG SAVED idx=%u wrapped=%d\n", _logIndex, _logsWrapped);


                    }


                void LogsManager::resetHist()
                    {
                        for (auto& e : _logsHistory) {
                            e = LogEntry{};
                        }
                        _logIndex = 0;
                        _logsWrapped = false;
                    }


                void LogsManager::logToJson(char* out, size_t outSize,
                            const char* message,
                            const char* logType,
                            const char* logIcon,
                            const char* source,
                            const char* sourceIcon,
                            uint64_t millis,
                            const char* payload)
                    {
                        const char* jsonPayload = payload ? payload : "{}";

                        snprintf(out, outSize,
                                "{\"millis\":%llu,\"logType\":\"%s\",\"logIcon\":\"%s\","
                                "\"message\":\"%s\",\"payload\":%s,\"source\":\"%s\",\"sourceIcon\":\"%s\"}",
                                millis, logType, logIcon, message, jsonPayload, source, sourceIcon);
                    }


                    std::vector<LogTypeInfo> LogsManager::logTypes()
                    {
                        return {
                            { LogType::INFO,    LogTypeToStr(LogType::INFO),    LogTypeToIcon(LogType::INFO) },
                            { LogType::WARNING, LogTypeToStr(LogType::WARNING), LogTypeToIcon(LogType::WARNING) },
                            { LogType::ERROR,   LogTypeToStr(LogType::ERROR),   LogTypeToIcon(LogType::ERROR) },
                            { LogType::DEBUG,   LogTypeToStr(LogType::DEBUG),   LogTypeToIcon(LogType::DEBUG) }
                        };
                    }

                            /*
                       std::vector<LogEntry> LogsManager::getLogs() const {
                            return _logsHistory; 
                   
                        }
                            */

 
                        const std::vector<LogEntry>& LogsManager::getLogs() const {
                            return _logsHistory; 
                        }
                      

                      


                        
                void LogsManager::registerEndpoints() { 
                    
                   
                     _webCommands.push_back({
                            "logs/reset",
                            0x01, // GET
                            [this](IHttpContext& httpCtx){
                                this->resetHist();
                                httpCtx.send(200, "application/json", this->ctx->config.buildLogsJson());
                            }
                        });




                   


                }




                
                


 
}