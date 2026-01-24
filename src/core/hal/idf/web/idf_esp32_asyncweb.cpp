#include "idf_esp32_asyncweb.h"
#include <ESPmDNS.h>
#include <LittleFS.h>



using namespace LoopMax::Core::Hal;


void idf_esp32_asyncweb::begin() {}

void idf_esp32_asyncweb::run() {
    server.begin();
    running = true;
}

void idf_esp32_asyncweb::tick() {}

void idf_esp32_asyncweb::end() {
    server.end();
    running = false;
}

bool idf_esp32_asyncweb::isAlive() {
    return running;
}


void idf_esp32_asyncweb::serveStatic(
    const std::string& uri,
    const std::string& path,
    const std::string& cache
) {
    auto h = server.serveStatic(uri.c_str(), LittleFS, path.c_str());
    if (!cache.empty()) {
        h.setCacheControl(cache.c_str());
    }
}


void idf_esp32_asyncweb::on(
    const std::string& uri,
    WebRequestMethod method,
    Handler handler
) {
    server.on(uri.c_str(), method, handler);
}


void idf_esp32_asyncweb::onNotFound(Handler handler) {
    server.onNotFound(handler);
}


