

#include "ServicesManager.h"
#include "core/services/IService.h"
#include <algorithm>
#include <cstring>

namespace LoopMax::Core {
        ServicesManager::ServicesManager()
        {}

        void ServicesManager::registerService(IService* service) {
            if (!service) return;
            service->registerEndpoints();
            services.push_back(service);
        }

        const std::vector<IService*>& ServicesManager::getServices() {
            return services;
        }
        
             void ServicesManager::check(Core::SerialManager* serialService) {
                for (auto* service : services) {
                    if (!service) continue;

                    // Se è il WebManager, controlliamo SOLO il WiFi
                    if (strcmp(service->name(), "web") == 0) {
                        bool wifiReady = false;
                        
                        // Cerchiamo il servizio wifi nella lista
                        for (auto* s : services) {
                            if (strcmp(s->name(), "wifi") == 0) {
                                if (s->state() == Types::ServiceState::READY) wifiReady = true;
                                break;
                            }
                        }

                        // LOGICA DI AZIONE
                        if (!wifiReady && service->state() == Types::ServiceState::READY) {
                            serialService->printLn("<< MANAGER: WiFi down, fermo Web...");
                            service->stop(); // Questo deve mettere lo stato a STOPPED
                        } 
                        else if (wifiReady && service->state() == Types::ServiceState::STOPPED) {
                            serialService->printLn(">> MANAGER: WiFi tornato, riavvio Web...");
                            service->start(); // Questo deve mettere lo stato a READY
                        }
                    }
                }
            }



        bool ServicesManager::isModeAllowed(IService* srv, Types::SystemMode currentMode) {

                    for (auto m : srv->modes()) {
                        if (m == currentMode) return true;
                    }
                    return false;
                }

            bool ServicesManager::areDepsReady(IService* srv) {
                auto& deps = srv->dependencyNames();
                if (deps.empty()) return true;

                for (const char* depName : deps) {
                    bool found = false;
                    for (auto* s : services) {
                        if (s && strcmp(s->name(), depName) == 0) {
                            found = true;
                            // Se la dipendenza NON è ready, il servizio padre non deve girare
                            if (s->state() != Types::ServiceState::READY) return false;
                            break;
                        }
                    }
                    // Se la dipendenza richiesta non è nemmeno registrata, non rischiare: ferma tutto
                    if (!found) return false;
                }
                return true;
            }


}

