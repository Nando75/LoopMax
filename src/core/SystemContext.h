#pragma once

#include "common/IModule.h"
#include "core/ModuleRegistry.h"   // <-- NECESSARIO per usare modules()

#include <cstdint>
#include <vector>

namespace LoopMax::Core {

    class TimeManager;    
    class LogsManager;    
    class SerialManager;
    class SystemManager;
    class PinsManager;
    class ResetManager;
    class GeoManager;
    class UpdateManager;
    class StorageManager;
    class ConfigManager;
    class WifiManager;
    class WebManager;
    class HttpsManager;
    class MqttsManager;
    class ServicesManager;

    class SystemContext {
    public:

        SystemContext(Core::ServicesManager &Serv, Core::TimeManager &timeS, Core::LogsManager &logsS,
                      Core::SerialManager  &serialS, Core::SystemManager  &systemS,
                      Core::PinsManager &pinsS, Core::ResetManager &resetS, Core::GeoManager &geoS,
                      Core::UpdateManager &updateS, Core::StorageManager &storageS,
                      Core::ConfigManager  &configS, Core::WifiManager &wifiS,
                      Core::WebManager &webS, Core::HttpsManager &httpsS, Core::MqttsManager &mqttsS)
            : ServMan(Serv), time(timeS), logs(logsS), serial(serialS),
              system(systemS), pins(pinsS), reset(resetS), geo(geoS),
              update(updateS), storage(storageS), config(configS),
              wifi(wifiS), web(webS), https(httpsS), mqtts(mqttsS)
        {}

        // Riferimenti ai servizi
        Core::ServicesManager &ServMan;
        Core::TimeManager    &time;
        Core::LogsManager    &logs;
        Core::SerialManager  &serial;
        Core::SystemManager  &system;
        Core::PinsManager    &pins;
        Core::ResetManager   &reset;
        Core::GeoManager     &geo;
        Core::UpdateManager  &update;
        Core::StorageManager &storage;
        Core::ConfigManager  &config;
        Core::WifiManager    &wifi;
        Core::WebManager     &web;
        Core::HttpsManager     &https;
        Core::MqttsManager &mqtts;

        // Inizializza il registry
        void initModules(Core::ModuleRegistry &registry) {
            _moduleRegistry = &registry;
        }

        // Ritorna i moduli
        const std::vector<LoopMax::Modules::IModule*>& Modules() const {
            assert(_moduleRegistry && "ModuleRegistry not initialized");
            return _moduleRegistry->modules();
        }

        /*
        std::string getDeviceName() {
            assert(_moduleRegistry && "ModuleRegistry not initialized");

            for (auto* m : _moduleRegistry->modules()) {
                if (m) {
                    return m->Data().DeviceName;
                }
            }
            return {};
        }
        */

        std::string getFirstName() {
            assert(_moduleRegistry && "ModuleRegistry not initialized");

            for (auto* m : _moduleRegistry->modules()) {
                if (m) {
                    return m->name();
                }
            }
            return {};
        }



    private:
        Core::ModuleRegistry *_moduleRegistry = nullptr;
    };

}
