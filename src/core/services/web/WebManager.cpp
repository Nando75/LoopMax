#include "WebManager.h"
#include "BaseConfig.h"
#include "common/tools/Helper.h" 
#include <LittleFS.h>
#include "core/services/serial/SerialManager.h"
#include "core/services/logs/LogsManager.h"
#include "core/services/config/ConfigManager.h"
#include "core/ServicesManager.h"

namespace LoopMax::Core {

    WebManager::WebManager()
        : _web(Hal::web()), _deps{"timer", "logs", "storage", "conf", "wifi"}
    {}

    // IService
    void WebManager::start() {
        
            _web.end();
            initCoreRoutes();

            initServicesRoutes();
            initModulesRoutes();
            
            _web.run(); // Parte immediatamente
            currentState = Types::ServiceState::READY;
            ctx->logs.write("Web Server Started", LogType::INFO, name(), icon());
    }

        void WebManager::loop() {  
            // Se il manager ha spento il servizio, non fare nulla e non loggare errori!
            if (currentState != Types::ServiceState::READY) return;

            // Fai il check della HAL solo se siamo davvero in READY e dopo un po' di tempo
            static uint32_t lastCheck = 0;
            if (millis() - lastCheck < 5000) return;
            lastCheck = millis();

            if (!_web.isAlive()) {
                // Se la HAL cade, segnalalo ma non bloccare tutto, 
                // lascia che sia il check() del manager a decidere se resettare.
                ctx->logs.write("Web HAL connection lost", LogType::WARNING, name(), icon());
            }

            
        }

    void WebManager::stop() { 
        _web.end();
        currentState = Types::ServiceState::STOPPED;
        ctx->logs.write("Web Server Stopped", LogType::INFO, name(), icon());
    }

    // IHal_Web (Implementazione dell'interfaccia nel manager)
    void WebManager::begin() {}
    
    void WebManager::run() { _web.run(); }

    void WebManager::tick() {}
    void WebManager::end() { _web.end(); }
    bool WebManager::isAlive() { return _web.isAlive(); }

   void WebManager::initCoreRoutes() {
        // ROOT
        _web.on("/", (int)WMethod::GET, [this](LoopMax::Types::IHttpContext& httpCtx){
                //ctx->logs.write("ROTTA CORE: " + httpCtx.url(), LogType::DEBUG, name(), icon());
                 //ctx->serial.printLn("ROOT: " + httpCtx.url());
                 //std::string token = httpCtx.getHeader("Authorization");
                 //ctx->serial.printLn(token);
                httpCtx.sendFile("/web/index.html", getMime("index.html"));
                /*
                if(ctx->system.mode() == SystemMode::AP) {
                    httpCtx.sendFile("/web/index.html", getMime("index.html"));
                    return;
                }
                std::string token =this->findToken(httpCtx);
                if (this->webTokenExists(token.c_str())) {
                    httpCtx.sendFile("/web/index.html", getMime("index.html"));
                }
                else
                {
                    httpCtx.sendFile("/web/login.html", getMime("login.html"));
                }
                */
        });

        
        // STATICI
        _web.serveStatic("/core", "/web/core");
        //_web.serveStatic("/modules", "/web/modules");
 
            _web.onNotFound([](IHttpContext& ctx) {
                const std::string url = ctx.url();
                
              if (url.rfind("/modules/", 0) == 0 ||
                    url.rfind("/core/", 0) == 0) {
                    ctx.send(404, "text/plain", "Not found");
                    return;
                }
            
                // 🔁 redirect SOLO per pagine web
                if (ctx.methodToStr() == "GET") {
                    ctx.redirect("/");
                } else {
                    ctx.send(404, "text/plain", "Not found");
                }
            });



        
    }

void WebManager::initServicesRoutes() {
    if (!ctx) return;
    
    for (auto* serv : ctx->ServMan.getServices()) {
        if (!serv) continue;

        const auto& commands = serv->webCommands(); // 🔑 riferimento
        for (const auto& cmd : commands) {
            std::string uri = "/api/" + cmd.uri;
            //_web.on(uri, (int)cmd.method, cmd.handler);

            _web.on(uri, (int)cmd.method, checkLogin(cmd.handler));

            ctx->logs.write("Service api: "+uri, LogType::DEBUG, name(), icon());
        }
    }
}


void WebManager::initModulesRoutes() {
    if (!ctx) return;

    for (auto& module : ctx->Modules()) {
        auto& modData = module->Data();
        // --- SERVE IL FILE JS DEL MODULO COME ES6 MODULE ---
        if (!modData.JsUIClass.empty()) {
            //std::string modName = modData.Name;
            //std::transform(modName.begin(), modName.end(), modName.begin(), ::tolower);
            std::string jsFile = modData.JsUIClass;
            std::transform(jsFile.begin(), jsFile.end(), jsFile.begin(), ::tolower);
            std::string urlPath = "/modules/" + jsFile;
            std::string fsPath  = "/web/modules/" + jsFile;
            _web.on(urlPath, (int)WMethod::GET, [fsPath](IHttpContext& ctx) {
                ctx.sendFile(fsPath, getMime(fsPath));
            });
            ctx->logs.write("Module JS route: " + urlPath, LogType::DEBUG, name(), icon());
            urlPath.clear();
            fsPath.clear();
            for (auto& jsFile : modData.JsFiles) {
                urlPath = "/modules/" + jsFile;
                fsPath  = "/web/modules/" + jsFile;
                _web.on(urlPath, (int)WMethod::GET, [fsPath](IHttpContext& ctx) {
                    ctx.sendFile(fsPath, getMime(fsPath));
                });
                ctx->logs.write("Module js: " + urlPath , LogType::DEBUG, name(), icon());
            }
        }

        // --- API DEL MODULO ---
        for (auto& cmd : modData.webCommands) {
            std::string uri = "/api/" + cmd.uri;
            //_web.on(uri, (int)cmd.method, cmd.handler);
            _web.on(uri, (int)cmd.method, checkLogin(cmd.handler));

            ctx->logs.write("Module api: " + uri, LogType::DEBUG, name(), icon());
        }

    }
}

WebManager::HandlerType WebManager::checkLogin(HandlerType handler) {
    return [this, handler](LoopMax::Types::IHttpContext& httpCtx) {
        //AP MODE SEMPRE ACCESSIBILE
        if (ctx->system.mode() == SystemMode::AP) {
            handler(httpCtx);
            return;
        }
        //LOGIN/LOGOUT SEMPRE ACCESSIBILE
        if (httpCtx.url() == "/api/web/login" || httpCtx.url() == "/api/web/logout" || 
            httpCtx.url() == "/api/json/system" || httpCtx.url() == "/api/json/wifi" ||
            httpCtx.url() == "/api/json/modules"|| httpCtx.url() == "/api/conf/set/language") {
            handler(httpCtx);
            return;
        }
        std::string token =this->findToken(httpCtx);
        //ctx->serial.printLn("DYNAMIMC URL: " + httpCtx.url());
        //ctx->serial.printLn(token);
        //ctx->logs.write("TOKEN: " + token, LogType::INFO, name(), icon());
        if (!this->webTokenExists(token.c_str())) {
            httpCtx.redirect("/");
            return;
        }
        handler(httpCtx);
    };
}


void WebManager::registerEndpoints()
{


    _webCommands.push_back({"web/login",0x02,
                [this](IHttpContext& httpCtx){
                    //httpCtx.getParam("unix")
                    //httpCtx.send(200, "text/plain", std::to_string(this->Unix()));
                    //ctx->serial.printLn("USER: " + httpCtx.getParam("usr"));
                    //ctx->serial.printLn("PSW: " + httpCtx.getParam("psw"));
                    const Settings& settings = ctx->config.settings();
                    //ctx->serial.printLn("ApUser: " + settings.apSsid);
                    //ctx->serial.printLn("ApPsw: " +settings.apPsw);

                    bool isCorrect = true;
                    if(httpCtx.getParam("usr") != settings.apSsid) isCorrect = false;
                    if(httpCtx.getParam("psw") != settings.apPsw) isCorrect = false;
                    //ctx->serial.print("correct: ");
                    //ctx->serial.printLn(isCorrect);

                    if(isCorrect)
                    {
                        char token[11];
                        LoopMax::Utils::Helper::getRandomString(token, sizeof(token));
                        this->addWebToken(token);
                        httpCtx.send(200, "text/plain", token);
                        return;
                    }
                    httpCtx.send(400, "text/plain", "");
                }
            });
        
            _webCommands.push_back({"web/logout",0x01,
                [this](IHttpContext& httpCtx){
                    this->logOut(httpCtx);
                    httpCtx.send(200, "text/plain", R"({"status":"ok"})");
                }
            });


        if(ctx->system.mode()!=SystemMode::AP)
        {
            
        }


}



bool WebManager::isLogged(LoopMax::Types::IHttpContext &httpCtx)
    {
        if(ctx->system.mode() == SystemMode::AP) return true;

        std::string token = this->findToken(httpCtx);
        return this->webTokenExists(token.c_str());
    }


    void WebManager::logOut(LoopMax::Types::IHttpContext &httpCtx)
    {
        std::string token =this->findToken(httpCtx);
        this->removeWebToken(token.c_str());
    }



    std::string WebManager::findToken(LoopMax::Types::IHttpContext &httpCtx)
    {
        std::string token = httpCtx.getHeader("Authorization");
        if (token.rfind("Bearer ", 0) == 0) {
            token = token.substr(7);
        }
        else { token = ""; }
        return token;
    }

    //LOGIN:
    /*
        bool WebManager::addWebToken(const char* token) {
            if (!token || strlen(token) != 10) return false;
            if (this->webTokenExists(token)) return false;

            std::array<char, 11> t{};
            strncpy(t.data(), token, 10);
            t[10] = '\0';

            webTokens.push_back(t);
            return true;
        }
    bool WebManager::webTokenExists(const char* token) {
        if (!token || strlen(token) != 10) return false;

        for (auto& t : webTokens) {
            if (strncmp(t.data(), token, 10) == 0)
                return true;
        }
        return false;
    }
   bool WebManager::removeWebToken(const char* token) {
    if (!token || strlen(token) != 10) return false;

    for (auto it = webTokens.begin(); it != webTokens.end(); ++it) {
        if (strncmp(it->data(), token, 10) == 0) {
            webTokens.erase(it);
            return true;
        }
    }
    return false;
}
*/

bool WebManager::addWebToken(const char* token) {
    if (!token || strlen(token) != 10) return false;
    if (webTokenExists(token)) return false;

    std::array<char, 11>& slot = webTokens[webTokenIndex];

    strncpy(slot.data(), token, 10);
    slot[10] = '\0';

    webTokenIndex = (webTokenIndex + 1) % MAX_WEB_TOKENS;
    if (webTokenCount < MAX_WEB_TOKENS)
        webTokenCount++;

    return true;
}

bool WebManager::webTokenExists(const char* token) {
    if (!token || strlen(token) != 10) return false;

    for (size_t i = 0; i < webTokenCount; ++i) {
        if (strncmp(webTokens[i].data(), token, 10) == 0)
            return true;
    }
    return false;
}

bool WebManager::removeWebToken(const char* token) {
    if (!token || strlen(token) != 10) return false;

    for (size_t i = 0; i < webTokenCount; ++i) {
        if (strncmp(webTokens[i].data(), token, 10) == 0) {
            webTokens[i][0] = '\0'; // invalida
            return true;
        }
    }
    return false;
}








    

}