#pragma once 
#include "common/types.h"
namespace LoopMax::Services {
    class IMqtts {
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

        virtual void loop() = 0;

        virtual bool isConnected() = 0;

        virtual ~IMqtts() {}
    };

}
