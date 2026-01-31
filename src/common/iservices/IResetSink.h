#pragma once

namespace LoopMax::Services {

    class IResetSink {
    public:
        virtual void onSystemReset() = 0;
        virtual ~IResetSink() = default;
    };

}    