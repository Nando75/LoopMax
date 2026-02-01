#include "ConfigManager.h"
#include "common/tools/Helper.h" 
#include "core/services/time/TimeManager.h"
#include "core/services/logs/LogsManager.h"
#include "core/services/serial/SerialManager.h"
#include "core/services/system/SystemManager.h"
#include "core/services/pins/PinsManager.h"
#include "core/services/reset/ResetManager.h"
#include "core/services/geo/GeoManager.h"
#include "core/services/update/UpdateManager.h"
#include "core/services/storage/StorageManager.h"
#include "core/services/wifi/WifiManager.h"
#include "core/services/web/WebManager.h"
#include "core/ServicesManager.h"
#include "core/ModuleRegistry.h"
#include <memory>
#include <ArduinoJson.h>
namespace LoopMax::Core {
    using namespace LoopMax::Utils;
    using namespace LoopMax::Types;
    using namespace LoopMax::Core::Hal;
    
                ConfigManager::ConfigManager()
                : _deps{"storage"}
                {}

                //IService
                void ConfigManager::start() {
                    
                    StorageResult res1 = loadMainConfig();
                    ctx->logs.write("Config Ready",LogType::INFO,this->name(), this->icon());
                }
                
                void ConfigManager::loop() {  }


                void ConfigManager::stop() { 

                    currentState = ServiceState::STOPPED;
                 }

             

                StorageResult ConfigManager::loadMainConfig()
                {
                    std::string configFile;
                    //bool exists = ctx->storage.getFile(devFileName, configFile);
                    bool exists = ctx->storage.getNVSConfig("device", configFile);


                    bool isCorrect = true;

                    //Carico l'hostname dal primo modulo:
                    if(exists)
                    { isCorrect = loadJsonConfigFile(configFile); }
                    if (!exists || !isCorrect) {
                         std::string msg = "Load Defaults, config file not found ...";
                         if(!isCorrect) msg = "Load Defaults, config file not valid ...";
                         ctx->logs.write(msg.c_str(),LogType::DEBUG, this->name(), this->icon());

                         
                         char key[keyLen+1];
                         Helper::getRandomString(key, sizeof(key));
                         _settings.key = key;
                         _settings.apSsid += std::to_string(Helper::getRandomNumber(1000));
                        bool saveRes=saveConfig();
                        if (!saveRes) {
                            ctx->logs.write("Failed to write default config",
                                        LogType::ERROR, this->name(), this->icon());
                            return StorageResult::IO_ERROR;
                        }
                        return StorageResult::OK;
                    }
                     
                    // ctx->logs.write("Config loaded from file",LogType::DEBUG,this->name(),this->icon());
                    
                    //SET OPERATING MODE                                            
                    if(_settings.mode!=SystemMode::AP)
                    {
                        ctx->system.setMode(_settings.mode);
                    }

                    
                    return StorageResult::OK;
                }



                void ConfigManager::buildJsonConfig(std::string &outJson)
                    {
                        outJson.clear();
                        JsonDocument doc;
                        JsonObject device = doc["device"].to<JsonObject>();
                        device["apSsid"]  = _settings.apSsid;
                        device["apPsw"]   = _settings.apPsw;
                        device["mode"]    = SystemModeToStr(_settings.mode);
                        device["key"]     = _settings.key;
                        device["wfSsid"]  = _settings.wfSsid;
                        device["wfPsw"]   = _settings.wfPsw;
                        outJson.reserve(256);
                        serializeJson(doc, outJson);
                    }



               bool ConfigManager::loadJsonConfigFile(const std::string &jsonData)
                        {
                            if (jsonData.empty())
                                return false;
                            JsonDocument doc;
                            DeserializationError err = deserializeJson(doc, jsonData);
                            if (err)
                                return false; // JSON invalido
                            JsonObject device = doc["device"];
                            if (device.isNull())
                                return false; // Formato errato

                            if (device["apSsid"].is<const char*>()) {
                                _settings.apSsid = device["apSsid"].as<const char*>();
                            } else { 
                                ctx->logs.write("apSsid not found",
                                    LogType::WARNING, this->name(), this->icon());
                                return false; 
                            }
                           
                            if (device["apPsw"].is<const char*>()) {
                                _settings.apPsw = device["apPsw"].as<const char*>();
                            } else { 
                                ctx->logs.write("apPsw not found",
                                    LogType::WARNING, this->name(), this->icon());
                                return false; 
                            }
                           // --- STRINGHE ---
                            if (device["mode"].is<const char*>())
                                { _settings.mode = SystemModeFromStr(device["mode"].as<const char*>()); }
                              else { 
                                ctx->logs.write("mode not found",
                                    LogType::WARNING, this->name(), this->icon());
                                return false; 
                            }

                            if (device["key"].is<const char*>())
                                { _settings.key = device["key"].as<const char*>(); }
                                else { 
                                ctx->logs.write("key not found",
                                    LogType::WARNING, this->name(), this->icon());
                                return false; 
                            }

                            if (device["wfSsid"].is<const char*>())
                                { _settings.wfSsid = device["wfSsid"].as<const char*>(); }
                                else { 
                                ctx->logs.write("wfSsid not found",
                                    LogType::WARNING, this->name(), this->icon());
                                return false; 
                            }

                            if (device["wfPsw"].is<const char*>())
                                { _settings.wfPsw = device["wfPsw"].as<const char*>(); }
                                else { 
                                ctx->logs.write("wfPsw not found",
                                    LogType::WARNING, this->name(), this->icon());
                                return false; 
                            }
                            return true;
                        }


        bool ConfigManager::saveConfig()
            {
                std::string configFile;
                buildJsonConfig(configFile);
                //Save default config:
                //return ctx->storage.saveFile(devFileName, configFile);
                return ctx->storage.saveNVSConfig("device",configFile);
            }

           std::string ConfigManager::buildTimeJson() {
                JsonDocument doc;

                JsonObject time = doc["time"].to<JsonObject>();
                time["unix"]      = ctx->time.Unix();
                time["tz_offset"] = ctx->time.Tz_Offset();
                time["timezone"]  = ctx->time.Timezone();

                std::string out;
                out.reserve(128);
                serializeJson(doc, out);

                return out;
            }



 



        std::string ConfigManager::buildSystemJson(bool isLogged) {
            JsonDocument doc;

            JsonObject sys = doc["system"].to<JsonObject>();

            sys["debug"]        = IS_DEBUG;
            sys["name"]         = _settings.name;
            sys["key"]          = _settings.key;
            sys["FwVersion"]    = _settings.FwVersion;
            sys["Company"]      = _settings.Company;
            sys["url"]          = _settings.CompanyUrl;

            sys["core"]         = ctx->system.ChipCores();
            sys["chipRevision"] = ctx->system.ChipRevision();
            sys["cpuFreq"]      = ctx->system.CpuFreq();
            sys["flashSize"]    = ctx->system.FlashSize();
            sys["flashSpeed"]   = ctx->system.FlashSpeed();
            sys["heapSize"]     = ctx->system.HeapSize();
            sys["heapFree"]     = ctx->system.FreeHeap();
            sys["uptime"]       = ctx->system.uptime();
            sys["api"]       = ctx->system.Api();
            sys["apiVersion"]       = ctx->system.ApiVersion();
            const char* modelPtr = ctx->system.ChipModel();
            const char* modePtr  = SystemModeToStr(ctx->system.mode());
            sys["chipModel"]    = modelPtr ? modelPtr : "unknown";
            sys["mode"]         = modePtr ? modePtr : "unknown";
            sys["isLogged"]     = isLogged;
            std::string out;
            out.reserve(512);
            serializeJson(doc, out);
            return out;
        }





        std::string ConfigManager::buildLogsJson() {
            JsonDocument doc;

            JsonObject logsObj = doc["logs"].to<JsonObject>();

            // --- types ---
            JsonArray typesArr = logsObj["types"].to<JsonArray>();
            for (const auto& t : ctx->logs.logTypes()) {
                JsonObject typeEntry = typesArr.add<JsonObject>();
                typeEntry["id"]   = static_cast<int>(t.type);
                typeEntry["name"] = t.name;
                typeEntry["icon"] = t.icon;
            }

            // --- list ---
            JsonArray listArr = logsObj["list"].to<JsonArray>();
            {
                std::lock_guard<std::mutex> lock(ctx->logs.getMutex());
                const auto& logsVector = ctx->logs.getLogs();

                for (const auto& l : logsVector) {
                    if (l.message.empty()) continue;

                    JsonObject e = listArr.add<JsonObject>();
                    e["millis"] = l.millis;
                    e["type"]   = l.logType;
                    e["icon"]   = l.logIcon;
                    e["msg"]    = l.message;
                    e["src"]    = l.source;
                    e["sIcon"]  = l.sourceIcon;
                    e["pld"]    = l.payload;
                }
            }

            std::string out;
            out.reserve(2048);
            serializeJson(doc, out);
            return out;
        }






          std::string ConfigManager::buildGeoJson() {
                JsonDocument doc;
                JsonObject geo = doc["geo"].to<JsonObject>();
                geo["latitude"]  = ctx->geo.getLatitude();
                geo["longitude"] = ctx->geo.getLongitude();
                geo["altitude"]  = ctx->geo.getAltitude();
                geo["accuracy"]  = ctx->geo.getAccuracy();
                geo["hasFix"]    = ctx->geo.hasFix();
                geo["continent"]    = ctx->geo.Continent();
                geo["continentCode"]    = ctx->geo.ContinentCode();
                geo["country"]    = ctx->geo.Country();
                geo["countryCode"]    = ctx->geo.CountryCode();
                geo["region"]    = ctx->geo.Region();
                geo["city"]    = ctx->geo.City();
                std::string out;
                out.reserve(128);
                serializeJson(doc, out);
                return out;
            }




              std::string ConfigManager::buildResetJson() {
                    const int   pinCopy  = ctx->reset.pinNumber();
                    const auto  modeEnum = ctx->reset.pinMode();
                    const char* modePtr  = PinModeToStr(modeEnum);
                    const std::string modeCopy = modePtr ? modePtr : "unknown";
                    JsonDocument doc;
                    JsonObject reset = doc["reset"].to<JsonObject>();
                    reset["pinNumber"] = pinCopy;
                    reset["pinMode"]   = modeCopy;
                    std::string out;
                    out.reserve(128);
                    serializeJson(doc, out);
                    return out;
                }




              std::string ConfigManager::buildWifiJson() {
                    JsonDocument doc;

                    JsonObject wifi = doc["wifi"].to<JsonObject>();

                    // 1. Credenziali
                    wifi["apSsid"]  = _settings.apSsid;
                    wifi["apPsw"]  = _settings.apPsw;
                    wifi["wfSsid"] = _settings.wfSsid;
                    wifi["wfPsw"]  = _settings.wfPsw;
                    wifi["wfRssi"]  = ctx->wifi.CurrentRSSI();
                    // 2. Stato attuale
                    wifi["isConnected"] = ctx->wifi.isConnected();
                    wifi["ip"]          = ctx->wifi.StringIP();
                    const char* modePtr = WifiModeToStr(ctx->wifi.getMode());
                    wifi["mode"]        = modePtr ? modePtr : "unknown";
                    wifi["dns"]         = ctx->wifi.dnsActive();
                    wifi["mdns"]        = ctx->wifi.mdnsActive();
                    wifi["publicIp"]    = ctx->wifi.getPublicIP();
                    wifi["isp"]         = ctx->wifi.Isp();
                    wifi["internet"]    = ctx->wifi.internet();
                    // 3. Array reti locali
                    JsonArray localWifi = wifi["localWifi"].to<JsonArray>();
                    for (const auto& w : ctx->wifi.getLocalWifi()) {
                        JsonObject wObj = localWifi.add<JsonObject>();
                        wObj["ssid"] = w.ssid;
                        wObj["rssi"] = w.rssi;
                    }

                    std::string out;
                    out.reserve(512);
                    serializeJson(doc, out);
                    return out;
                }






                        std::string ConfigManager::buildWebJson() {
                            JsonDocument doc;

                            JsonObject web = doc["web"].to<JsonObject>();

                            // 1. URL mDNS
                            web["url1"] = "http://" + _settings.apSsid + ".local";

                            // 2. IP URL + stato WiFi
                            const std::string ip = ctx->wifi.StringIP().c_str();
                            web["url2"]    = "http://" + ip;
                            web["cPortal"] = ctx->wifi.dnsActive();
                            web["dns"]     = ctx->wifi.dnsActive();
                            web["mdns"]    = ctx->wifi.mdnsActive();

                            // 3. Stato Web Server
                            web["isAlive"] = ctx->web.isAlive();
                            web["type"]    = ctx->web.Type();
                            web["ui"]      = "Bootstrap Bundle";

                            std::string out;
                            out.reserve(256);
                            serializeJson(doc, out);
                            return out;
                        }


                        std::string ConfigManager::buildServicesJson() {
                            JsonDocument doc;

                            JsonObject servicesRoot = doc["services"].to<JsonObject>();

                            for (auto* serv : ctx->ServMan.getServices()) {
                                if (!serv) continue;

                                const char* namePtr = serv->name();
                                if (!namePtr) continue;

                                JsonObject serviceObj = servicesRoot[namePtr].to<JsonObject>();

                                // 1. Dati base
                                serviceObj["name"]  = namePtr;
                                serviceObj["icon"]  = serv->icon();

                                const char* stateStr = ServiceStateToStr(serv->state());
                                serviceObj["state"]  = stateStr ? stateStr : "UNKNOWN";

                                // 2. Dipendenze
                                JsonArray depsArr = serviceObj["dependencies"].to<JsonArray>();
                                for (const char* dep : serv->dependencyNames()) {
                                    if (dep) depsArr.add(dep);
                                }

                                // 3. Web Commands
                                JsonArray cmdsArr = serviceObj["webCommands"].to<JsonArray>();
                                for (const auto& cmd : serv->webCommands()) {
                                    JsonObject cmdObj = cmdsArr.add<JsonObject>();
                                    cmdObj["uri"] = cmd.uri;

                                    const char* methodStr = WebMethodToStr(static_cast<LoopMax::Types::WMethod>(cmd.method));
                                    cmdObj["method"] = methodStr ? methodStr : "GET";
                                }
                            }

                            std::string out;
                            out.reserve(2048);
                            serializeJson(doc, out);
                            return out;
                        }


                        std::string ConfigManager::buildModulesJson() {
                            JsonDocument doc;

                            JsonObject modulesRoot = doc["modules"].to<JsonObject>();

                            for (auto* mod : ctx->Modules()) {
                                if (!mod) continue;

                                const std::string& modName = mod->name();
                                if (modName.empty()) continue;

                                JsonObject modObj = modulesRoot[modName].to<JsonObject>();
                                auto& data = mod->Data();

                                modObj["FwVersion"]   = data.FwVersion;
                                modObj["UpdateAvalaible"] = data.UpdateAvalaible;
                                modObj["newFwVersion"] = data.newFwVersion;
                                modObj["Name"]        = data.Name;
                                modObj["DeviceName"]  = data.DeviceName;
                                modObj["Icon"]        = data.Icon;
                                modObj["Code"]        = data.Code;
                                modObj["Company"]     = data.Company;
                                modObj["Url"]         = data.Url;
                                modObj["SaveConfig"]  = data.SaveConfig;
                                modObj["JsonConfig"]  = data.JsonConfig;
                                modObj["JsUIClass"]   = data.JsUIClass;

                                modObj["Https_ShowData"]   = data.httpsData.Https_ShowData;
                                if(data.httpsData.Https_ShowData)
                                {
                                    modObj["Https_ApiUrl"]     = data.httpsData.Https_ApiUrl;
                                    modObj["Https_Port"]       = data.httpsData.Https_Port;
                                    modObj["Https_TimeOut"]    = data.httpsData.Https_TimeOut;
                                    modObj["Https_UseCA"]      = data.httpsData.Https_UseCA;
                                    //modObj["Https_User"]      = data.httpsData.Https_User;
                                    //modObj["Https_Psw"]      = data.httpsData.Https_Psw;
                                    
                                }
                                
                                modObj["Mqtts_ShowData"]   = data.mqttsData.Mqtts_ShowData;
                                if(data.mqttsData.Mqtts_ShowData)
                                {
                                    modObj["Mqtts_Port"]       = data.mqttsData.Mqtts_Port;
                                    modObj["Mqtts_Server"]     = data.mqttsData.Mqtts_Server;
                                    modObj["Mqtts_UseCA"]      = data.mqttsData.Mqtts_UseCA;
                                    //modObj["Mqtts_User"]       = data.mqttsData.Mqtts_User;
                                    //modObj["Mqtts_Psw"]        = data.mqttsData.Mqtts_Psw;
                                }

                                JsonArray pinsArr = modObj["pins"].to<JsonArray>();
                                for (const auto& p : data.pins) {
                                    JsonObject pObj = pinsArr.add<JsonObject>();
                                    pObj["number"] = p.number;
                                    pObj["name"]   = p.name;
                                    pObj["level"]  = PinLevelToInt(p.level);
                                }

                                JsonArray cmdsArr = modObj["webCommands"].to<JsonArray>();
                                for (const auto& cmd : data.webCommands) {
                                    JsonObject cmdObj = cmdsArr.add<JsonObject>();
                                    cmdObj["uri"] = cmd.uri;

                                    const char* methodStr = WebMethodToStr(static_cast<WMethod>(cmd.method));
                                    cmdObj["method"] = methodStr ? methodStr : "GET";
                                }
                            }

                            std::string out;
                            out.reserve(2048);
                            serializeJson(doc, out);
                            return out;
                        }



                bool ConfigManager::resetSystem()
                {
                        
                    for (auto* sink : sinks) {
                        if (sink) sink->onSystemReset();
                    }


                    _settings.apSsid = "LoopMaxAP" + std::to_string(Helper::getRandomNumber(1000));
                    _settings.apPsw = "loopmaxap";
                    _settings.mode = SystemMode::AP;
                    _settings.wfSsid     = "";
                    _settings.wfPsw      = "";
                    return this->saveConfig();
                }
                


                void ConfigManager::registerEndpoints() { 

                _webCommands.push_back({
                        "conf/reset",
                        0x01,
                        [this](IHttpContext& httpCtx){
                            
                            
                            if(this->resetSystem()) 
                            {
                                ctx->logs.write("Device reset ...",LogType::INFO,this->name(), this->icon());
                                httpCtx.send(200, "application/json", "{\"status\":\"ok\"}");
                                ctx->system.restart();
                            }
                            else httpCtx.send(200, "application/json", "{\"status\":\"ko\"}");
                            
                        }
                    });


                _webCommands.push_back({
                        "conf/save/ap",
                        0x02,
                        [this](IHttpContext& httpCtx){
                            
                            _settings.apSsid = httpCtx.getParam("apSsid");
                            _settings.apPsw = httpCtx.getParam("apPsw");
                            bool res = this->saveConfig();

                            if(res) 
                            {
                                ctx->logs.write("AP data changed",LogType::INFO,this->name(), this->icon());
                                httpCtx.send(200, "text/plain", "ok");
                            }
                            if(!res) httpCtx.send(400, "text/plain", "ko");
                        }
                    });


                 _webCommands.push_back({
                        "conf/save/wifi",
                        0x02,
                        [this](IHttpContext& httpCtx){
                            
                            _settings.mode=SystemMode::LAN;
                            _settings.wfSsid = httpCtx.getParam("ssid");
                            _settings.wfPsw = httpCtx.getParam("psw");
                            bool res = this->saveConfig();
                            if(res) 
                            {
                                ctx->logs.write("Wifi login saved, reboot to LAN mode",LogType::INFO,this->name(), this->icon());
                                httpCtx.send(200, "text/plain", "ok");
                                ctx->system.restart();
                            }
                            if(!res) httpCtx.send(400, "text/plain", "ko");
                        }
                    });


                 _webCommands.push_back({
                        "json/system",
                        0x01,
                        [this](IHttpContext& httpCtx){
                            bool isLogged = true; //PRIMA CHIAMATA CONTROLLO WEB, IN AP SEMPRE TRUE.
                            if(ctx->system.mode()!=SystemMode::AP)
                            {   isLogged = ctx->web.isLogged(httpCtx); }
                            httpCtx.send(200, "application/json", this->buildSystemJson(isLogged));
                        }
                    });
                   
                    
                    _webCommands.push_back({
                        "json/time",
                        0x01, // GET
                        [this](IHttpContext& httpCtx){
                            httpCtx.send(200, "application/json",  this->buildTimeJson());
                        }
                    });
                    


                     _webCommands.push_back({
                        "json/logs",
                        0x01, // GET
                        [this](IHttpContext& httpCtx){
                            httpCtx.send(200, "application/json", this->buildLogsJson());
                        }
                    });



                     _webCommands.push_back({
                        "json/geo",
                        0x01, // GET
                        [this](IHttpContext& httpCtx){
                            httpCtx.send(200, "application/json", this->buildGeoJson());
                        }
                    });

                     _webCommands.push_back({
                        "json/reset",
                        0x01, // GET
                        [this](IHttpContext& httpCtx){
                            httpCtx.send(200, "application/json", this->buildResetJson());
                        }
                    });

                     _webCommands.push_back({
                        "json/wifi",
                        0x01, // GET
                        [this](IHttpContext& httpCtx){
                            httpCtx.send(200, "application/json", this->buildWifiJson());
                        }
                    });

                     _webCommands.push_back({
                        "json/web",
                        0x01, // GET
                        [this](IHttpContext& httpCtx){
                            httpCtx.send(200, "application/json", this->buildWebJson());
                        }
                    });

                    
                     _webCommands.push_back({
                        "json/services",
                        0x01, // GET
                        [this](IHttpContext& httpCtx){
                            httpCtx.send(200, "application/json", this->buildServicesJson());
                        }
                    });



                     _webCommands.push_back({
                        "json/modules",
                        0x01, // GET
                        [this](IHttpContext& httpCtx){
                            httpCtx.send(200, "application/json", this->buildModulesJson());
                        }
                    });
                
                
                
                
                }


                bool ConfigManager::isReady() const { return ctx->storage.isReady(); }

              bool ConfigManager::ExistSavedConfig(IModuleData& moduleData) {
                    if(moduleData.Name.empty()) return false;
                    std::string jsonFileModules;
                    if (!ctx->storage.getFile(modFileName, jsonFileModules))
                        return false;
                    JsonDocument doc;
                    if (deserializeJson(doc, jsonFileModules))
                        return false;
                    JsonArray modules = doc["modules"].as<JsonArray>();
                    if (modules.isNull())
                        return false;
                    for (JsonObject mod : modules) {
                        if (mod["name"] == moduleData.Name)
                        {
                            const char* deviceName = mod["deviceName"];
                            return (deviceName && deviceName[0] != '\0');

                        }
                    }

                    return false;
                }



                    void ConfigManager::loadConfig(IModuleData& moduleData) {
                        if(moduleData.Name.empty()) return;

                        std::string jsonFileModules;
                        if (!ctx->storage.getFile(modFileName, jsonFileModules))
                            return;

                        JsonDocument doc;
                        if (deserializeJson(doc, jsonFileModules))
                            return;

                        JsonArray modules = doc["modules"].as<JsonArray>();
                        if (modules.isNull())
                            return;

                        for (JsonObject mod : modules) {
                            const char* name = mod["name"];
                            if (!name || strcmp(name, moduleData.Name.c_str()) != 0)
                                continue;

                            const char* deviceName = mod["deviceName"];
                            if (deviceName != nullptr && deviceName[0] != '\0') {
                                moduleData.DeviceName = deviceName;
                            } 

                            // payload
                            if (!mod["payload"].isNull()) {
                                   serializeJson(mod["payload"], moduleData.JsonConfig);
                            }
                           
                            // pins
                            JsonArray pinsJson = mod["pins"].as<JsonArray>();
                            if (!pinsJson.isNull()) {
                                for (JsonObject pinJson : pinsJson) {
                                    uint32_t pinNumber = pinJson["number"] | UINT32_MAX;
                                    if (pinNumber == UINT32_MAX) continue;

                                    for (auto& pin : moduleData.pins) {
                                        if (pin.number != pinNumber) continue;

                                        if (pinJson["level"].is<uint32_t>())
                                            pin.level = pinJson["level"].as<uint32_t>() ? PinLevel::High : PinLevel::Low;

                                        if (pinJson["level"].is<const char*>())
                                            pin.level = PinLevelFromStr(pinJson["level"]);

                                        if (pinJson["name"].is<const char*>())
                                            pin.name = pinJson["name"].as<const char*>();

                                        break;
                                    }
                                }
                            }

                            break; // MODULO TROVATO → STOP
                        }
                    }




                bool ConfigManager::saveModuleConfig(IModuleData& moduleData) {
                            if(moduleData.Name.empty()||moduleData.DeviceName.empty()) return false;

                            std::string jsonFileModules;
                            JsonDocument doc;
                            if (!ctx->storage.getFile(modFileName, jsonFileModules) ||
                                deserializeJson(doc, jsonFileModules)) {
                                jsonFileModules = buildModuleFile(moduleData);
                                return ctx->storage.saveFile(modFileName, jsonFileModules);
                            }
                            JsonArray modules = doc["modules"].to<JsonArray>();
                            // 🔍 trova o crea il modulo
                            JsonObject mod;
                            for (JsonObject m : modules) {
                                if (strcmp(m["name"] | "", moduleData.Name.c_str()) == 0) {
                                    mod = m;
                                    break;
                                }
                            }
                            if (mod.isNull()) {
                                mod = modules.add<JsonObject>();
                                mod["name"] = moduleData.Name;
                            }

                            mod["deviceName"] = moduleData.DeviceName;
                            
                            // 📦 payload
                            JsonDocument payloadDoc;
                            if (!deserializeJson(payloadDoc, moduleData.JsonConfig)) {
                                mod["payload"] = payloadDoc.as<JsonObject>();
                            }

                            // 🔌 pins (riscritti sempre)
                            JsonArray pinsJson = mod["pins"].to<JsonArray>();
                            pinsJson.clear();
                            for (const auto& p : moduleData.pins) {
                                JsonObject pin = pinsJson.add<JsonObject>();
                                pin["number"] = p.number;
                                pin["name"]   = p.name;
                                pin["level"]  = PinLevelToInt(p.level);
                            }
                            // 💾 salva
                            jsonFileModules.clear();
                            serializeJson(doc, jsonFileModules);
                            ctx->logs.write(
                                "Saving Modules",
                                LogType::INFO,
                                this->name(),
                                this->icon(),
                                jsonFileModules
                            );
                           return ctx->storage.saveFile(modFileName, jsonFileModules);
                        }



            
           std::string ConfigManager::buildModuleFile(IModuleData& moduleData) {
                    JsonDocument doc;

                    JsonArray modules = doc["modules"].to<JsonArray>();
                    JsonObject mod = modules.add<JsonObject>();

                    mod["name"]    = moduleData.Name;
                    mod["deviceName"]    = moduleData.DeviceName;
                    mod["payload"] = moduleData.JsonConfig;

                    JsonArray jsonPins = mod["pins"].to<JsonArray>();
                    for (const auto& pinData : moduleData.pins) {
                        JsonObject pin = jsonPins.add<JsonObject>();
                        pin["number"] = pinData.number;
                        pin["name"]   = pinData.name;
                        pin["level"]  = PinLevelToInt(pinData.level);
                    }

                    std::string out;
                    out.reserve(2048);
                    serializeJson(doc, out);
                    return out;
                }


                bool ConfigManager::removeConfig(const std::string& moduleName) {
                    if (moduleName.empty())
                        return false;

                    std::string jsonFileModules;
                    if (!ctx->storage.getFile(modFileName, jsonFileModules))
                        return false;

                    JsonDocument doc;
                    if (deserializeJson(doc, jsonFileModules))
                        return false;

                    JsonArray modules = doc["modules"].as<JsonArray>();
                    if (modules.isNull())
                        return false;

                    bool removed = false;

                    for (size_t i = 0; i < modules.size(); ++i) {
                        JsonObject mod = modules[i];
                        const char* name = mod["name"];
                        if (!name) continue;

                        if (strcmp(name, moduleName.c_str()) == 0) {
                            modules.remove(i);
                            removed = true;
                            break; // UNA SOLA RIMOZIONE
                        }
                    }

                    if (!removed)
                        return false;

                    std::string out;
                    out.reserve(jsonFileModules.size());
                    serializeJson(doc, out);

                    return ctx->storage.saveFile(modFileName, out);
                }


                //ILogSink
                void ConfigManager::registerSink(Services::IResetSink* sink) {
                    sinks.push_back(sink);
                }



}

