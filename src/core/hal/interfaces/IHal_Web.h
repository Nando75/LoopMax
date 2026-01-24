#pragma once 
#include <string>
#include <functional>
#include "common/IHttpContext.h"

namespace LoopMax::Core::Hal {
    class IHal_Web {
    public:
        // L'handler ora riceve l'interfaccia universale, NON AsyncWebServerRequest
        using Handler = std::function<void(LoopMax::Types::IHttpContext&)>;

        virtual void begin() = 0;
        virtual void run() = 0;
        virtual void tick() = 0;
        virtual void end() = 0;
        virtual bool isAlive() = 0;
        virtual std::string Type() = 0;

        virtual void on(const std::string& uri, int method, Handler handler) = 0;
        virtual void serveStatic(const std::string& uri, const std::string& fsPath, const std::string& cache = "") = 0;
        virtual void onNotFound(Handler handler) = 0;

        virtual ~IHal_Web() = default;
    };
}
