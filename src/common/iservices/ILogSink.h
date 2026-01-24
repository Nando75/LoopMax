#pragma once
#include "common/types.h"



namespace LoopMax::Services {

    class ILogSink {
    public:
        virtual void publishLogs(const char* logs) = 0;
        virtual ~ILogSink() = default;
    };

}    