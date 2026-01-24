#pragma once
#include <string>

namespace LoopMax::Types {
    class IHttpContext {
    public:
        virtual ~IHttpContext() {}
        virtual void addHeader(const std::string& name, const std::string& value) = 0;
        virtual std::string getHeader(const std::string& name) = 0;

        virtual std::string getParam(const std::string& name) = 0;
        virtual bool hasParam(const std::string& name) = 0;
        virtual void send(int code, const std::string& contentType, const std::string& content) = 0;
        virtual void redirect(const std::string& url) = 0;
        virtual std::string methodToStr() = 0;
        virtual void sendFile(const std::string& path, const std::string& contentType) = 0;
        virtual std::string getBody() = 0;
        virtual std::string url() = 0;


    };
}