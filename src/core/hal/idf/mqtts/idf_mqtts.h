#pragma once
#include "core/hal/interfaces/IHal_Mqtts.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

namespace LoopMax::Core::Hal {

    class idf_mqtts : public IHal_Mqtts {
    public:
        void begin(Types::MqttsData& config) override;

        bool connect() override;
        void disconnect() override;

        bool publish(
            const std::string& topic,
            const std::string& payload,
            bool retained = false
        ) override;

        bool subscribe(const std::string& topic) override;

        void onMessage(Types::MqttCallback cb) override;

        void tick() override;

        bool isConnected() override;

    private:
        Types::MqttsData* _config = nullptr;

        WiFiClientSecure _client;
        PubSubClient _mqtt{_client};

        Types::MqttCallback _callback = nullptr;

        void _internalCallback(char* topic, byte* payload, unsigned int length);
    };
}
