#include "ard_esp32_asyncweb.h"

namespace LoopMax::Core::Hal {


void ard_esp32_asyncweb::end() {
    server.end();
    running = false;
}

bool ard_esp32_asyncweb::isAlive() {
    return running;
}

void ard_esp32_asyncweb::run() {
    server.begin();
    running = true;
}





void ard_esp32_asyncweb::on(const std::string& uri, int method, Handler handler) {
    server.on(uri.c_str(), (WebRequestMethod)method,

        // HANDLER PRINCIPALE
        [handler](AsyncWebServerRequest *request) {
            if (!request) return;

            // wrapper associato alla request
            auto* wrapper = static_cast<AsyncHttpContextWrapper*>(request->_tempObject);
            if (!wrapper) {
                // crea e attacca wrapper
                wrapper = new AsyncHttpContextWrapper(request);
                request->_tempObject = wrapper;
            } else {
                wrapper->setRequest(request);
            }

            handler(*wrapper);

            // cleanup DOPO la risposta
            if (request->_tempObject) {
                delete static_cast<AsyncHttpContextWrapper*>(request->_tempObject);
                request->_tempObject = nullptr;
            }
        },

        // UPLOAD HANDLER (non usato)
        nullptr,

        // BODY HANDLER
        [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
            auto* wrapper = static_cast<AsyncHttpContextWrapper*>(request->_tempObject);
            if (!wrapper) {
                wrapper = new AsyncHttpContextWrapper(request);
                request->_tempObject = wrapper;
                
            }
            wrapper->appendBody(data, len);
        }
    );
}



    void ard_esp32_asyncweb::serveStatic(const std::string& uri, const std::string& fsPath, const std::string& cache) {
    server.serveStatic(uri.c_str(), LittleFS, fsPath.c_str())
          .setTryGzipFirst(true);
          //.setTryGzipFirst(false);
          //.setCacheControl(cache.empty() ? "max-age=86400" : cache.c_str())
          //.setLastModified("Fri, 10 Jan 2025 00:00:00 GMT"); 
          
    }


void ard_esp32_asyncweb::onNotFound(Handler handler) {
    server.onNotFound([handler](AsyncWebServerRequest *request) {

        if (!request) return;

        const String& url = request->url();

        // 🔥 IGNORA source maps
        if (url.endsWith(".map") || url.endsWith(".map.gz")) {
            request->send(204, "text/plain", "");
            return;
        }

        // 🔥 IGNORA gzip fantasma
        if (url.endsWith(".gz")) {
            request->send(404, "text/plain", "");
            return;
        }
        


        // 🔁 passa al WebManager
        AsyncHttpContextWrapper wrapper(request);
        handler(wrapper);
    });
}







}