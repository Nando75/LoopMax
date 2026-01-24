#include "ard_mqtts.h"

namespace LoopMax::Core::Hal {

            void ard_mqtts::begin(Types::MqttsData& config) {
                _config = &config;

                if (_config->Mqtts_UseCA && !_config->Mqtts_CA.empty()) {
                    _client.setCACert(_config->Mqtts_CA.c_str());
                } else {
                    _client.setInsecure();
                }

                _mqtt.setServer(
                    _config->Mqtts_Server.c_str(),
                    _config->Mqtts_Port
                );

                _mqtt.setCallback(
                    [this](char* t, byte* p, unsigned int l) {
                        _internalCallback(t, p, l);
                    }
                );
            }

            bool ard_mqtts::connect() {
                if (!_config) return false;
                if (_mqtt.connected()) return true;

                return _mqtt.connect(
                    "esp32-client",
                    _config->Mqtts_User.c_str(),
                    _config->Mqtts_Psw.c_str()
                );
            }

            void ard_mqtts::disconnect() {
                _mqtt.disconnect();
            }

            bool ard_mqtts::publish(
                const std::string& topic,
                const std::string& payload,
                bool retained
            ) {
                if (!_mqtt.connected()) return false;
                return _mqtt.publish(
                    topic.c_str(),
                    payload.c_str(),
                    retained
                );
            }

            bool ard_mqtts::subscribe(const std::string& topic) {
                if (!_mqtt.connected()) return false;
                return _mqtt.subscribe(topic.c_str());
            }

            void ard_mqtts::onMessage(Types::MqttCallback cb) {
                _callback = cb;
            }

            void ard_mqtts::tick() {
                if (_mqtt.connected()) {
                    _mqtt.loop();
                }
            }

            bool ard_mqtts::isConnected() {
                return _mqtt.connected();
            }

            void ard_mqtts::_internalCallback(
                char* topic,
                byte* payload,
                unsigned int length
            ) {
                if (!_callback) return;

                std::string msg;
                msg.reserve(length);
                for (unsigned int i = 0; i < length; i++) {
                    msg += static_cast<char>(payload[i]);
                }

                _callback(topic, msg);
            }

}


/*
#include "core/hal/arduino/mqtts/ard_mqtts.h"

using namespace LoopMax::Core::Hal;
using namespace LoopMax::Types;

MqttsData mqttConfig {
    .Mqtts_ShowData = true,
    .Mqtts_UseCA = false,
    .Mqtts_Server = "broker.hivemq.com",
    .Mqtts_Port   = 8883,
    .Mqtts_User   = "",
    .Mqtts_Psw    = "",
    .Mqtts_CA     = ""
};





void setup() {
    Serial.begin(115200);

    // WiFi già connesso qui (AP+STA nel tuo caso)

    mqtt.begin(mqttConfig);

    mqtt.onMessage([](const std::string& topic,
                      const std::string& payload) {

        Serial.print("MQTT RX [");
        Serial.print(topic.c_str());
        Serial.print("]: ");
        Serial.println(payload.c_str());

        // qui il "registrante" fa quello che vuole
    });

    if (mqtt.connect()) {
        mqtt.subscribe("loopmax/test/in");
        mqtt.publish("loopmax/test/out", "hello from ESP32");
    }
}



void loop() {
    mqtt.loop();

    static uint32_t last = 0;
    if (millis() - last > 5000) {
        last = millis();

        if (mqtt.isConnected()) {
            mqtt.publish(
                "loopmax/test/out",
                "ping",
                false
            );
        }
    }
}






System owns MqttsData
        ↓
ard_mqtts.begin(config)
        ↓
TLS + server setup
        ↓
connect()
        ↓
subscribe()
        ↓
loop()  ← chiamata ciclica
        ↓
callback → registrante



Micro-consigli (se vuoi rifinirla)
🔹 ID client dinamico
String id = "esp32-" + String(ESP.getEfuseMac(), HEX);
_mqtt.connect(id.c_str(), ...);



🔹 Reconnect automatico (nel loop)
if (!_mqtt.connected()) {
    connect();
}



🔹 LWT
_mqtt.connect(id, user, psw, "status", 0, true, "offline");
È un messaggio che il broker MQTT pubblica AUTOMATICAMENTE se il client:

si disconnette in modo anomalo

crasha

perde WiFi

si resetta

muore senza chiamare disconnect()

👉 NON viene pubblicato se fai disconnect() volontariamente.
Esempio reale con PubSubClient
_mqtt.connect(
    clientId,
    user,
    password,
    "devices/esp32_01/status", // LWT topic
    0,                         // QoS
    true,                      // retained
    "offline"                  // messaggio
);
Se l’ESP32 crasha → il broker pubblica:
devices/esp32_01/status = "offline"
Poi TU pubblichi “online” manualmente
Subito dopo la connessione riuscita:
_mqtt.publish(
    "devices/esp32_01/status",
    "online",
    true
);



Come usarlo nella TUA architettura
Nel tuo ard_mqtts::connect():
bool ard_mqtts::connect() {
    if (!_config) return false;
    if (_mqtt.connected()) return true;

    std::string clientId = "loopmax-" + std::to_string(ESP.getEfuseMac());

    bool ok = _mqtt.connect(
        clientId.c_str(),
        _config->Mqtts_User.c_str(),
        _config->Mqtts_Psw.c_str(),
        "devices/status",  // o device-specific
        0,
        true,
        "offline"
    );

    if (ok) {
        _mqtt.publish("devices/status", "online", true);
    }

    return ok;
}









*/
