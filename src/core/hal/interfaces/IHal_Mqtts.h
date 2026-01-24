#pragma once
#include "common/types.h"
#include <string>
#include <functional>

namespace LoopMax::Core::Hal {
  class IHal_Mqtts {
    public:
        virtual void begin(Types::MqttsData& config) = 0;

        virtual bool connect() = 0;
        virtual void disconnect() = 0;

        virtual bool publish(
            const std::string& topic,
            const std::string& payload,
            bool retained = false
        ) = 0;

        virtual bool subscribe(const std::string& topic) = 0;

        virtual void onMessage(Types::MqttCallback cb) = 0;

        virtual void tick() = 0;

        virtual bool isConnected() = 0;

        virtual ~IHal_Mqtts() = default;
    };
}
