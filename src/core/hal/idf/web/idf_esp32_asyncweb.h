#pragma once
#include "core/hal/interfaces/IHal_Web.h"
#include "common/types.h"
#include <ESPAsyncWebServer.h>
#include <unordered_map>
#include <DNSServer.h>


namespace LoopMax::Core::Hal {

            class idf_esp32_asyncweb : public IHal_Web {
               public:
                    using Handler = std::function<void(AsyncWebServerRequest*)>;

                    void begin() override;
                    void run() override;
                    void tick() override;
                    void end() override;
                    bool isAlive() override;

                    // 🔹 statici
                    void serveStatic(
                        const std::string& uri,
                        const std::string& path,
                        const std::string& cacheControl = ""
                    ) override;

                    // 🔹 dinamici (ESP32 only)
                    void on(const std::string& uri, WebRequestMethod method, Handler handler);
                    void onNotFound(Handler handler);

                private:
                    AsyncWebServer server{80};
                    bool running = false;
                    



            };



}






/*

web.begin();
void WebManager::registerCoreRoutes() {
    auto file = storage.read("/index.html");
    web.serve(
        "/",
        "text/html",
        file.data,
        file.size,
        "no-cache"
    );
    auto css = storage.read("/style.css");
    web.serve("/style.css", "text/css", css.data, css.size, "max-age=86400");
}
registerCoreRoutes();
registerModules();
web.run();










void setupWeb() {

    IPAddress captiveIP(192,168,4,1);
    std::string hostname = "loopmax01";
    web.begin(captiveIP, hostname);



    

    web.run();
}

void loop() {
    web.loop(); // mantiene attivo DNS captive portal
}


Come WebManager registra i comandi
void WebManager::registerModule(IModuleData& module) {
    for (auto& cmd : module.commands) {
        web.on("/" + module.name + cmd.path, cmd.method, cmd.handler);
    }
}






*/