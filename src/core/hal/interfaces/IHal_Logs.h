#pragma once 
#include <cstdint>
#include "common/types.h"
#include "common/iservices/ILogSink.h"
#include <mutex>


namespace LoopMax::Core::Hal {
    using namespace LoopMax::Types;    

    class IHal_Logs {
    public:
        virtual void write(const char* msg, Types::LogType type = Types::LogType::INFO, const char* source = "logs", 
                                            const char* sourceIcon = "✏️" , const char* payload = "{}") = 0;
                                            
        virtual  void registerSink(Services::ILogSink* sink) = 0;
        virtual std::vector<Types::LogTypeInfo> logTypes() = 0;
        virtual const std::vector<LogEntry>& getLogs() const =0 ;
        
        //virtual void initContext(SystemContext* ctx) = 0;
        virtual std::mutex& getMutex() const  =0 ;
        
        virtual ~IHal_Logs() {}
    };

}
