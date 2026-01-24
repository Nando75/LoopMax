#pragma once
#include "core/hal/interfaces/IHal_Web.h"
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>


namespace LoopMax::Core::Hal {

class AsyncHttpContextWrapper : public LoopMax::Types::IHttpContext {
private:
    AsyncWebServerRequest* _req;
    std::string _body;

public:
    AsyncHttpContextWrapper(AsyncWebServerRequest* req) : _req(req) {}


    AsyncWebServerRequest* getRequest() const {
            return _req;
        }


    void setRequest(AsyncWebServerRequest* req) { _req = req; }

    void appendBody(const uint8_t* data, size_t len) {
        _body.append((const char*)data, len);
    }

    std::string getBody() override { return _body; }

    void addHeader(const std::string&, const std::string&) override {}

    std::string getHeader(const std::string& name) override {
            if (!_req) return "";
            return _req->header(name.c_str()).c_str();
        }


    std::string getParam(const std::string& name) override {
        if (_req->hasParam(name.c_str(), true)) {
            return _req->getParam(name.c_str(), true)->value().c_str();
        }
        return "";
    }

    bool hasParam(const std::string& name) override {
        return _req->hasParam(name.c_str(), true);
    }


    void send(int code, const std::string& type, const std::string& content) override {
        _req->send(code, type.c_str(), content.c_str());
    }

    void redirect(const std::string& url) override {
        _req->redirect(url.c_str());
    }

    std::string methodToStr() override {
        return _req->methodToString();
    }

    void sendFile(const std::string& path, const std::string& contentType) override {
        _req->send(LittleFS, path.c_str(), contentType.c_str());
    }

    std::string url() override {
        return _req->url().c_str();
    }



};






    // IL SERVER HAL
    class ard_esp32_asyncweb : public IHal_Web {
    public:
        void begin() override {}
        void run() override;
        void tick() override {}
        void end() override;
        bool isAlive() override;
        std::string Type() override { return "ESPAsyncWebServer"; }

        void on(const std::string& uri, int method, Handler handler) override;
        void serveStatic(const std::string& uri, const std::string& fsPath, const std::string& cache = "") override;
        void onNotFound(Handler handler) override;

    private:
        AsyncWebServer server{80};
        bool running = false;
        
    };
}