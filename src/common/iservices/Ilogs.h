//#include "BaseConfig.h"
#pragma once
#include "common/types.h"
#include "common/iservices/ILogSink.h"
#include <cstddef>  
#include <cstdint>  

using namespace LoopMax::Types;
namespace LoopMax::Services{


                class ILogs {
                public:
                    
                    virtual void write(const char* msg, Types::LogType type = Types::LogType::INFO, const char* source = "logs", 
                                            const char* sourceIcon = "✏️" , const char* payload = "{}") = 0;

                    virtual void write(std::string msg, Types::LogType type = Types::LogType::INFO, std::string source = "logs", 
                                            std::string sourceIcon = "✏️" , std::string payload = "{}") = 0;
                                       
                    virtual  void registerSink(Services::ILogSink* sink) = 0;
                    virtual std::vector<Types::LogTypeInfo> logTypes() = 0;
                    virtual const std::vector<LogEntry>& getLogs() const =0 ;
                    
                    virtual ~ILogs() {}
                };


 }
